#include "mpdhandler.h"
#include "mpdutil.h"
#include <QByteArray>
#include <QDebug>
#include <QPixmap>
#include <iostream>
#include <mpd/client.h>

MpdHandler::MpdHandler(QObject* parent)
    : QObject{parent}
{
    conn.reset(mpd_connection_new(NULL, 0, 0));

    // Error handling
    if(conn == nullptr)
    {
        qDebug() << "MPD connection failed due to being out-of-memory";
    }

    MPD_CHECK(conn);
}

// Fetches current song and get it's embedded album art
// TODO: Check for memory leaks/errors, and use macros & error checks
QPixmap MpdHandler::get_current_art()
{
    struct mpd_song* curr_song = mpd_run_current_song(conn.get());
    MPD_CHECK(conn);
    if(!curr_song)
        return QPixmap();
    std::string curr_song_uri = mpd_song_get_uri(curr_song);
    MPD_CHECK(conn);

    int                        offset = 0;
    unsigned char              pic_chunk[BINARY_CHUNK_SIZE];
    std::vector<unsigned char> pic_buffer;
    // Read in chunks at a time into pic_chunk, then append to pic_buffer
    while(true)
    {
        int read_bytes = mpd_run_readpicture(
            conn.get(), curr_song_uri.c_str(), offset, pic_chunk, BINARY_CHUNK_SIZE);
        MPD_CHECK(conn);
        if(read_bytes <= 0)
        {
            break;
        }
        pic_buffer.insert(pic_buffer.end(), pic_chunk, pic_chunk + read_bytes);
        offset += read_bytes;
    }

    if(pic_buffer.empty())
    {
        mpd_song_free(curr_song);
        MPD_CHECK(conn);
        return QPixmap();
    }

    QPixmap res;
    res.loadFromData(pic_buffer.data(), pic_buffer.size());

    mpd_song_free(curr_song);
    MPD_CHECK(conn);
    return res;
}

// TODO: Use tags to fetch other fields like album, name, etc.
SongInfo MpdHandler::get_current_songinfo()
{
    struct mpd_song* curr_song = mpd_run_current_song(conn.get());
    MPD_CHECK(conn);
    int current_duration = mpd_song_get_duration(curr_song);
    MPD_CHECK(conn);
    // TODO: Remove redundant usage of mpd_run_current_song in get_current_art
    QPixmap art = get_current_art();

    struct SongInfo res;
    res.img      = art;
    res.duration = current_duration;

    mpd_song_free(curr_song);

    return res;
}

// QString MpdHandler::get_mpd_dir() { }

// QString MpdHandler::get_current_song_path() { }

// Read current mpd_status and tell main thread to update based on status
void MpdHandler::parse_status()
{
    mpd_status* status = mpd_run_status(conn.get());

    current_volume = mpd_run_get_volume(conn.get());
    MPD_CHECK(conn);
    emit volume_changed(current_volume);

    repeat_state = mpd_status_get_repeat(status);
    MPD_CHECK(conn);
    emit repeat_mode_changed(repeat_state);

    random_state = mpd_status_get_random(status);
    MPD_CHECK(conn);
    emit random_mode_changed(random_state);

    single_state = mpd_status_get_single_state(status);
    MPD_CHECK(conn);
    emit single_mode_changed(single_state);

    consume_state = mpd_status_get_consume_state(status);
    MPD_CHECK(conn);
    emit consume_mode_changed(consume_state);

    mpd_state state = mpd_status_get_state(status);
    // Depending on state, pass on more details to GUI (e.g. song details)
    switch(state)
    {
    case MPD_STATE_STOP:
        break;
    case MPD_STATE_PAUSE:
        // Change logo here and fallthrough to PLAY state
    case MPD_STATE_PLAY:
        current_song = get_current_songinfo();
        emit art_changed(current_song.img);
        emit song_changed(current_song.duration);
        break;
    case MPD_STATE_UNKNOWN:
        qDebug() << "MPD_STATE_UNKNOWN when checking state";
    }

    MPD_CHECK(conn);

    mpd_status_free(status);
}

void MpdHandler::handle_toggle_playback()
{
    if(!mpd_run_toggle_pause(conn.get()))
    {
        qDebug() << "Failed to toggle playback";
    }
    MPD_CHECK(conn);
}

void MpdHandler::handle_next_song()
{
    if(!mpd_run_next(conn.get()))
    {
        qDebug() << "Failed to go to next song";
    }
    MPD_CHECK(conn);
    current_song = get_current_songinfo();
    emit art_changed(current_song.img);
    emit song_changed(current_song.duration);
}

void MpdHandler::handle_prev_song()
{
    if(!mpd_run_previous(conn.get()))
    {
        qDebug() << "Failed to go to prev song";
    }
    MPD_CHECK(conn);
    current_song = get_current_songinfo();
    emit art_changed(current_song.img);
    emit song_changed(current_song.duration);
}

void MpdHandler::handle_volume(int volume)
{
    int  relative_diff = volume - current_volume;
    if(relative_diff == 0)
        return;
    bool res           = mpd_run_change_volume(conn.get(), relative_diff);
    // Do we need both checks?
    if(!res)
        emit error_occurred("Failed to execute mpd_run_change_volume");
    MPD_CHECK(conn);
    current_volume = volume;
    emit volume_changed(current_volume);
}

void MpdHandler::handle_playback_seeking(int seconds)
{
    // Note: false flag denotes seeking to absolute time stamp
    bool res = mpd_run_seek_current(conn.get(), seconds, false);
    if(!res)
        emit error_occurred("Failed to execute mpd_run_seek_current");
    MPD_CHECK(conn);
    emit playback_progress_changed(seconds);
}

void MpdHandler::handle_repeat()
{
    mpd_run_repeat(conn.get(), !repeat_state);
    MPD_CHECK(conn);
    repeat_state = !repeat_state;
    emit repeat_mode_changed(repeat_state);
}

void MpdHandler::handle_random()
{
    mpd_run_random(conn.get(), !random_state);
    MPD_CHECK(conn);
    random_state = !random_state;
    emit random_mode_changed(random_state);
}

void MpdHandler::handle_single()
{
    mpd_single_state next_state;
    switch(single_state)
    {
    case MPD_SINGLE_OFF:
        next_state = MPD_SINGLE_ONESHOT;
        break;
    case MPD_SINGLE_ONESHOT:
        next_state = MPD_SINGLE_ON;
        break;
    case MPD_SINGLE_ON:
        next_state = MPD_SINGLE_OFF;
        break;
    case MPD_SINGLE_UNKNOWN:
        qDebug() << "MPD_SINGLE_UNKNOWN when checking state";
    }

    bool res = mpd_run_single_state(conn.get(), next_state);
    if(!res)
        emit error_occurred("Failed to execute mpd_run_single_state");
    MPD_CHECK(conn);
    single_state = next_state;
    emit single_mode_changed(single_state);
}

void MpdHandler::handle_consume()
{
    mpd_consume_state next_state;
    switch(consume_state)
    {
    case MPD_CONSUME_OFF:
        next_state = MPD_CONSUME_ONESHOT;
        break;
    case MPD_CONSUME_ONESHOT:
        next_state = MPD_CONSUME_ON;
        break;
    case MPD_CONSUME_ON:
        next_state = MPD_CONSUME_OFF;
        break;
    case MPD_CONSUME_UNKNOWN:
        qDebug() << "MPD_CONSUME_UNKNOWN when checking state";
    }

    bool res = mpd_run_consume_state(conn.get(), next_state);
    if(!res)
        emit error_occurred("Failed to execute mpd_run_consume_state");
    MPD_CHECK(conn);
    consume_state = next_state;
    emit consume_mode_changed(consume_state);
}
