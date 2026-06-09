#include "mpdhandler.h"

/**
 * @brief MpdHandler::MpdHandler
 * @param parent
 */

MpdHandler::MpdHandler(QObject* parent)
    : QObject{parent}
{
    std::cout << "In MpdHandler ctor!\n";
    // Try simple MPD connection to test basic functionality
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
    std::cout << "Freed mpd connection\n";
}

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
