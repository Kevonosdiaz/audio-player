#include "mainwindow.h"
#include "mpdhandler.h"
#include <QApplication>
#include <QDebug>
#include <QObject>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    // Setup second thread and run MPD related tasks on it
    // QThread*    mpd_thread  = new QThread();
    MpdHandler mpd_handler;
    // mpd_handler.moveToThread(mpd_thread);

    // Delete worker after thread is finished
    // QObject::connect(mpd_thread, &QThread::finished, &mpd_handler, &QObject::deleteLater);

    // Setup connections so worker can be told what to do

    // mpd_thread->start();

    MainWindow w(&mpd_handler);
    w.show();

    // delete mpd_thread;

    return QApplication::exec();
}
