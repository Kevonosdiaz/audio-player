#include "tagart.h"
#include <taglib/fileref.h>

#include <QByteArray>
#include <QPixmap>
#include <QString>

QPixmap get_image_data(const QString& filepath)
{
    TagLib::FileRef f{filepath.toUtf8().constData()};
    if(f.isNull())
        return QPixmap();
    TagLib::List<TagLib::VariantMap> pictures = f.complexProperties("PICTURE");
    TagLib::VariantMap               cover_pic;
    for(const auto& pic : pictures)
    {
        if(pic["pictureType"].value<TagLib::String>() == "Front Cover")
            cover_pic = pic;
        break;
    }
    if(cover_pic.isEmpty())
        return QPixmap();

    // Load in image data to QPixmap
    TagLib::ByteVector img_data = cover_pic["data"].value<TagLib::ByteVector>();
    QByteArray         qba{img_data.data(), img_data.size()};
    QPixmap            res;
    res.loadFromData(qba);
    return res;
}