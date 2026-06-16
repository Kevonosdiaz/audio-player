#include "mainwindow.h"
#include "mpdhandler.h"
#include <QApplication>
#include <QDebug>
#include <QObject>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    // Setup second thread and run MPD related tasks on it
    QThread    mpd_thread;
    MpdHandler mpd_handler;
    mpd_handler.moveToThread(&mpd_thread);

    // Delete worker after thread is finished
    QObject::connect(&mpd_thread, &QThread::finished, &mpd_handler, &QObject::deleteLater);


    mpd_thread.start();

    MainWindow w(&mpd_handler);
    w.show();

    QObject::connect(
        &mpd_handler, &MpdHandler::art_changed, &w, &MainWindow::handle_main_art_changed);

    mpd_handler.initialize_state();

    return QApplication::exec();
}
