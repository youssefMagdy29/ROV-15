#ifndef IMAGE_H
#define IMAGE_H

#include <QWidget>

namespace Ui {
class Image;
}

class Image : public QWidget
{
    Q_OBJECT

public:
    explicit Image(QImage *image, QWidget *parent = 0);
    ~Image();

private:
    Ui::Image *ui;
    QImage *image;
};

#endif // IMAGE_H
