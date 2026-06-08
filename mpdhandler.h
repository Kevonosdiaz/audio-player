#ifndef MPDHANDLER_H
#define MPDHANDLER_H

// Handle MPD interaction and related tasks
// Should be moved to another thread to avoid blocking main event loop

#include <QDebug>
#include <QObject>
#include <QThread>

class MpdHandler : public QObject
{
    Q_OBJECT
public:
    explicit MpdHandler(QObject* parent = nullptr);

public slots:
    void toggle_playback();
    void next_song();
    void prev_song();
    // TODO: Communicate to main thread to display large album art
    // , as well as small album art in song list(s)
    // TODO: Figure out how to communicate MPD/song info to GUI
signals:
    // Signals to GUI:
    // void display_large_art();
    void s_song_changed();
    void s_volume_changed();

    // Signals to MpdWorker:
    void s_toggle_playback();
    void s_next_song();
    void s_prev_song();
};

#endif // MPDHANDLER_H
