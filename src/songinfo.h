#ifndef SONGINFO_H
#define SONGINFO_H

#include <QPixmap>
#include <QString>

struct SongInfo
{
    // QPixmap img;
    QString title;
    QString artist;
    QString album;
    int     duration;
};

#endif // SONGINFO_H
