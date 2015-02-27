#ifndef IMAGE_H
#define IMAGE_H

#include <QMainWindow>

namespace Ui {
class Image;
}

class Image : public QMainWindow
{
    Q_OBJECT

public:
    explicit Image(QImage *image, QWidget *parent = 0);
    ~Image();

private:
    Ui::Image *ui;
    QImage *image;

private slots:
    void showResult(int startX, int startY, int endX, int endY);
    void saveAs();
};

#endif // IMAGE_H
