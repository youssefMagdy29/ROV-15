#ifndef IMAGE_H
#define IMAGE_H

#include <QMainWindow>
#include "mission.h"

namespace Ui {
class Image;
}

class Image : public QMainWindow
{
    Q_OBJECT

public:
    explicit Image(QImage *image, QWidget *parent = 0);
    void setImage(QImage &image);
    ~Image();
    void show();
    void setMission(Mission *cm);

private:
    Ui::Image *ui;
    QImage *image;
    Mission *currentMission;

private slots:
    void showResult(int startX, int startY, int endX, int endY);
    void saveAs();
    void exit();
protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
};

#endif // IMAGE_H
