#include "mpdhandler.h"
#include "tagart.h"
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

QPixmap MpdHandler::get_current_art()
{
    // mpd_run_readpicture(conn);
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
}

void MpdHandler::handle_prev_song()
{
    if(!mpd_run_previous(conn))
    {
        std::cerr << "Failed to go to prev song" << std::endl;
    }
}
