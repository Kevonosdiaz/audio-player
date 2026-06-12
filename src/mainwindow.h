#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mpdhandler.h"
#include "songinfo.h"
#include <QLabel>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(MpdHandler* mpd_handler, QWidget* parent = nullptr);
    ~MainWindow() override;

public slots:
    void handle_playback_toggled();
    void handle_song_changed(const SongInfo& song_info);
    void handle_main_art_changed(const QPixmap& art);

private:
    Ui::MainWindow* ui;
    QLabel*         main_album_art_display;
    MpdHandler*     mpd_handler;
};
#endif // MAINWINDOW_H
