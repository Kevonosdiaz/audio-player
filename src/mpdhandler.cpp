#include "mpdhandler.h"
#include "util.h"
#include <QByteArray>
#include <QDebug>
#include <QPixmap>
#include <mpd/client.h>

MpdHandler::MpdHandler(QObject* parent)
    : QObject{parent}
{
    conn = mpd_connection_new(NULL, 0, 0);

    // Error handling
    if(conn == nullptr)
    {
        qDebug() << "MPD connection failed due to being out-of-memory";
    }

    MPD_CHECK(conn);
}

MpdHandler::~MpdHandler()
{
    mpd_connection_free(conn);
}

// Fetches current song and get it's embedded album art
// TODO: Check for memory leaks/errors, and use macros & error checks
QPixmap MpdHandler::get_current_art()
{
    struct mpd_song* curr_song = mpd_run_current_song(conn);
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
            conn, curr_song_uri.c_str(), offset, pic_chunk, BINARY_CHUNK_SIZE);
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

void MpdHandler::handle_toggle_playback()
{
    if(!mpd_run_toggle_pause(conn))
    {
        qDebug() << "Failed to toggle playback";
    }
    MPD_CHECK(conn);
}

void MpdHandler::handle_next_song()
{
    if(!mpd_run_next(conn))
    {
        qDebug() << "Failed to go to next song";
    }
    MPD_CHECK(conn);
    QPixmap art = get_current_art();
    emit art_changed(art);
}

void MpdHandler::handle_prev_song()
{
    if(!mpd_run_previous(conn))
    {
        qDebug() << "Failed to go to prev song";
    }
    MPD_CHECK(conn);
}
