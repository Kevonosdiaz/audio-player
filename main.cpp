#include "mainwindow.h"
#include <QDebug>
#include <mpd/client.h>
#include <iostream>
#include <memory>

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow   w;
    w.show();

    // Try simple MPD connection to test basic functionality
    struct mpd_connection* conn = mpd_connection_new(NULL, 0, 0);

    // Error handling
    if (conn == nullptr) {
        std::cerr << "OOM" << std::endl;
        return 1;
    }

    if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
        std::cerr << "Connection error: " << mpd_connection_get_error_message(conn) << std::endl;
        mpd_connection_free(conn);
        return 1;
    }

    // Try play/pause
    if (!mpd_run_toggle_pause(conn)) {
        std::cerr << "Failed to toggle playback" << std::endl;
    } else {
        std::cout << "Successfully toggled playback" << std::endl;
    }

    mpd_connection_free(conn);

    return QApplication::exec();
}
