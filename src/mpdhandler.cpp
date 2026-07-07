#include "mpdhandler.h"
#include "mpdutil.h"
#include <QByteArray>
#include <QDebug>
#include <QPixmap>
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

// Fetch MPD status, existing song playing, queue, etc.
// and emit corresponding signals to main
void MpdHandler::initialize_state() { }

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

SongInfo MpdHandler::get_current_songinfo() { }

QString MpdHandler::get_mpd_dir() { }

QString MpdHandler::get_current_song_path() { }

// Read current mpd_status and tell main thread to update based on status
void MpdHandler::parse_status()
{
    mpd_status* status = mpd_run_status(conn.get());

    emit repeat_mode_changed(mpd_status_get_repeat(status));
    MPD_CHECK(conn);
    emit random_mode_changed(mpd_status_get_random(status));
    MPD_CHECK(conn);
    emit single_mode_changed(mpd_status_get_single_state(status));
    MPD_CHECK(conn);
    emit consume_mode_changed(mpd_status_get_consume_state(status));
    MPD_CHECK(conn);
    mpd_state state = mpd_status_get_state(status);
    // Depending on state, pass on more details to GUI (e.g. song details)
    switch(state)
    {
    case MPD_STATE_STOP:
        break;
    case MPD_STATE_PAUSE:
        // Change logo here and fallthrough to PLAY state
    case MPD_STATE_PLAY:
        QPixmap art = get_current_art();
        emit    art_changed(art);
        break;
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
    QPixmap art = get_current_art();
    emit    art_changed(art);
}

void MpdHandler::handle_prev_song()
{
    if(!mpd_run_previous(conn.get()))
    {
        qDebug() << "Failed to go to prev song";
    }
    MPD_CHECK(conn);
    QPixmap art = get_current_art();
    emit    art_changed(art);
}
