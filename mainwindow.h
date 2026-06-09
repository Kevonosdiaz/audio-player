#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mpdhandler.h"
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

private slots:
    void playback_toggled();
    void song_changed();

private:
    Ui::MainWindow* ui;
    MpdHandler*     mpd_handler;
};
#endif // MAINWINDOW_H
