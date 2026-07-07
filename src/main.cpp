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

    // Hook up connections from mpd_handler to mainwindow
    QObject::connect(
        &mpd_handler, &MpdHandler::art_changed, &w, &MainWindow::handle_main_art_changed);
    QObject::connect(
        &mpd_handler, &MpdHandler::volume_changed, &w, &MainWindow::handle_volume_changed);
    QObject::connect(&mpd_handler,
                     &MpdHandler::repeat_mode_changed,
                     &w,
                     &MainWindow::handle_repeat_mode_changed);
    QObject::connect(&mpd_handler,
                     &MpdHandler::random_mode_changed,
                     &w,
                     &MainWindow::handle_random_mode_changed);
    QObject::connect(&mpd_handler,
                     &MpdHandler::single_mode_changed,
                     &w,
                     &MainWindow::handle_single_mode_changed);
    QObject::connect(&mpd_handler,
                     &MpdHandler::consume_mode_changed,
                     &w,
                     &MainWindow::handle_consume_mode_changed);

    // Initialize based on current MPD state
    mpd_handler.parse_status();

    return QApplication::exec();
}
