#ifndef MPDUTIL_H
#define MPDUTIL_H

#include <QDebug>
#include <QPixmap>
#include <QString>
#include <mpd/client.h>

// This is expected to be run after a libmpdclient call
// Check for error, log it, and clear it
// TODO: We may want to hook this up to interface for user to see MPD errors
// TODO: How should fatal MPD errors be handled?
#define MPD_CHECK(conn)                                                     \
    do                                                                      \
    {                                                                       \
        mpd_error err = mpd_connection_get_error(conn.get());               \
        if(err != MPD_ERROR_SUCCESS)                                        \
        {                                                                   \
            qDebug() << mpd_connection_get_error_message(conn.get());       \
            bool successful_clear = mpd_connection_clear_error(conn.get()); \
            if(!successful_clear)                                           \
            {                                                               \
                qDebug() << "Fatal MPD error occurred!";                    \
            }                                                               \
        }                                                                   \
    } while(0)

struct SongInfo
{
    QPixmap img;
    QString title;
    QString artist;
    QString album;
    int     duration;
};

#endif // MPDUTIL_H
