#include "scalingpixmaplabel.h"
#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>

ScalingPixmapLabel::ScalingPixmapLabel(QWidget* parent)
{
    this->setMinimumSize(1, 1);
    setScaledContents(false);
}

QSize ScalingPixmapLabel::sizeHint() const
{
    int w = this->width();
    return QSize(w, heightForWidth(w));
}

int ScalingPixmapLabel::heightForWidth(int width) const
{
    return current_pixmap.isNull()
               ? this->height()
               : ((qreal)current_pixmap.height() * width) / current_pixmap.width();
}

QPixmap ScalingPixmapLabel::get_scaled_pixmap()
{
    auto scaled = current_pixmap.scaled(
        this->size() * devicePixelRatioF(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaled.setDevicePixelRatio(devicePixelRatioF());
    return scaled;
}

void ScalingPixmapLabel::set_pixmap(const QPixmap& pix)
{
    current_pixmap = pix;
    QLabel::setPixmap(get_scaled_pixmap());
}

void ScalingPixmapLabel::resize_event(QResizeEvent* event)
{
    if(!current_pixmap.isNull())
    {
        QLabel::setPixmap(get_scaled_pixmap());
    }
}
