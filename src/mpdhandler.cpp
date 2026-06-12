#include "mpdhandler.h"
#include "tagart.h"
#include <QByteArray>
#include <QPixmap>
#include <iostream>
#include <mpd/client.h>

// TODO: Figure out error handling
MpdHandler::MpdHandler(QObject* parent)
    : QObject{parent}
{
    conn = mpd_connection_new(NULL, 0, 0);

    // Error handling
    if(conn == nullptr)
    {
        std::cerr << "OOM" << std::endl;
    }

    if(mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS)
    {
        std::cerr << "Connection error: " << mpd_connection_get_error_message(conn) << std::endl;
        mpd_connection_free(conn);
    }
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
    if(!curr_song)
        return QPixmap();
    std::string curr_song_uri = mpd_song_get_uri(curr_song);

    int                        offset = 0;
    unsigned char              pic_chunk[BINARY_CHUNK_SIZE];
    std::vector<unsigned char> pic_buffer;
    // Read in chunks at a time into pic_chunk, then append to pic_buffer
    while(true)
    {
        int read_bytes = mpd_run_readpicture(
            conn, curr_song_uri.c_str(), offset, pic_chunk, BINARY_CHUNK_SIZE);
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
        return QPixmap();
    }

    QPixmap res;
    res.loadFromData(pic_buffer.data(), pic_buffer.size());

    mpd_song_free(curr_song);
    return res;
}

SongInfo MpdHandler::get_current_songinfo() { }

QString MpdHandler::get_mpd_dir() { }

QString MpdHandler::get_current_song_path() { }

void MpdHandler::handle_toggle_playback()
{
    if(!mpd_run_toggle_pause(conn))
    {
        std::cerr << "Failed to toggle playback" << std::endl;
    }
}

void MpdHandler::handle_next_song()
{
    if(!mpd_run_next(conn))
    {
        std::cerr << "Failed to go to next song" << std::endl;
    }
    QPixmap art = get_current_art();
    emit art_changed(art);
}

void MpdHandler::handle_prev_song()
{
    if(!mpd_run_previous(conn))
    {
        std::cerr << "Failed to go to prev song" << std::endl;
    }
}
