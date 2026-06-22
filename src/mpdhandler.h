#ifndef MPDHANDLER_H
#define MPDHANDLER_H

#include "mpdutil.h"
#include <QDebug>
#include <QObject>
#include <QPixmap>
#include <QString>
#include <QThread>
#include <memory>
#include <mpd/client.h>

struct mpd_conn_freer
{
    void operator()(mpd_connection* conn) const
    {
        if(conn)
            mpd_connection_free(conn);
    }
};

using mpd_conn = std::unique_ptr<mpd_connection, mpd_conn_freer>;

// Handle MPD interaction and related tasks
// Should be moved to another thread to avoid blocking main event loop

class MpdHandler : public QObject
{
    Q_OBJECT
public:
    explicit MpdHandler(QObject* parent = nullptr);
    void initialize_state();

public slots:
    void handle_toggle_playback();
    void handle_next_song();
    void handle_prev_song();
    // TODO: Figure out how to communicate MPD/song info to GUI
signals:
    void song_changed(SongInfo& song_info);
    void error_occurred(QString msg);
    void warning_occurred(QString msg);
    void art_changed(QPixmap art);
    void mpd_state_changed(mpd_state state);
    void volume_changed(int volume);
    void repeat_mode_changed(bool repeat_on);
    void random_mode_changed(bool random_on);
    void single_mode_changed(mpd_single_state state);
    void consume_mode_changed(mpd_consume_state state);

private:
    mpd_conn conn;
    // Default size for libmpdclient to use for buffers,
    // changable with binarylimit command
    size_t   BINARY_CHUNK_SIZE = 8192;
    QPixmap  get_current_art();
    SongInfo get_current_songinfo();
    QString  get_mpd_dir();
    QString  get_current_song_path();
    void     parse_status();
};

#endif // MPDHANDLER_H
