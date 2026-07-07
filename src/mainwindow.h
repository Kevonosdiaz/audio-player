#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mpdhandler.h"
#include "scalingpixmaplabel.h"
#include <QLabel>
#include <QMainWindow>
#include <mpd/client.h>

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

    void handle_volume_changed(int volume);
    void handle_repeat_mode_changed(bool repeat_on);
    void handle_random_mode_changed(bool random_on);
    void handle_single_mode_changed(mpd_single_state state);
    void handle_consume_mode_changed(mpd_consume_state state);

private:
    Ui::MainWindow* ui;
    ScalingPixmapLabel* main_album_art_display;
    MpdHandler*     mpd_handler;
};
#endif // MAINWINDOW_H
