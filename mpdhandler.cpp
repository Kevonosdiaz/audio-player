#include "mpdhandler.h"

/**
 * @brief MpdHandler::MpdHandler
 * @param parent
 */

MpdHandler::MpdHandler(QObject* parent)
    : QObject{parent}
{
}

void MpdHandler::toggle_playback() { }

void MpdHandler::next_song() { }

void MpdHandler::prev_song() { }
