#ifndef MPDHANDLER_H
#define MPDHANDLER_H

#include <QDebug>
#include <QObject>
#include <QThread>
#include <iostream>
#include <mpd/client.h>

// Handle MPD interaction and related tasks
// Should be moved to another thread to avoid blocking main event loop

class MpdHandler : public QObject
{
    Q_OBJECT
public:
    explicit MpdHandler(QObject* parent = nullptr);
    ~MpdHandler();

public slots:
    void handle_toggle_playback();
    void handle_next_song();
    void handle_prev_song();
    // TODO: Communicate to main thread to display large album art
    // , as well as small album art in song list(s)
    // TODO: Figure out how to communicate MPD/song info to GUI
signals:
    // void display_large_art();
    void song_changed();
    void volume_changed();
    void error_occurred(QString msg);
    void warning_occurred(QString msg);

private:
    struct mpd_connection* conn;
};

#endif // MPDHANDLER_H
