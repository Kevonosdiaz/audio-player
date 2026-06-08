#include "mainwindow.h"
#include "mpdhandler.h"
#include <QApplication>
#include <QDebug>
#include <QObject>
#include <mpd/client.h>

#include <iostream>
#include <memory>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow   w;
    w.show();

    // Setup second thread and run MPD related tasks on it
    QThread*    mpd_thread  = new QThread();
    MpdHandler* mpd_handler = new MpdHandler();
    mpd_handler->moveToThread(mpd_thread);

    // Delete worker after thread is finished
    QObject::connect(mpd_thread, &QThread::finished, mpd_handler, &QObject::deleteLater);

    // Setup connections so worker can be told what to do

    mpd_thread->start();

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
