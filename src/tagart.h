#include <QPixmap>
#include <QString>
#include <taglib/fileref.h>

// Take in path to an audio file and retrieve embedded art
// using TagLib library

// Expects `filepath` to be a valid filepath
// Returns empty QPixmap if null FileRef or no front cover image embedded
QPixmap get_image_data(const QString& filepath);
