#ifndef SCALINGPIXMAPLABEL_H
#define SCALINGPIXMAPLABEL_H

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>

// QLabel for holding dynamically scaling QPixmap
// e.g. for picture in a splitter pane (album art display)
class ScalingPixmapLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ScalingPixmapLabel(QWidget* parent = nullptr);
    QSize sizeHint() const override;
    int   heightForWidth(int width) const override;

    QPixmap get_scaled_pixmap();

public slots:
    void set_pixmap(const QPixmap& pix);
    void resizeEvent(QResizeEvent* event);

private:
    QPixmap current_pixmap;
};

#endif // SCALINGPIXMAPLABEL_H
