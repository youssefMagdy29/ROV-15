#include "resizable_label.h"
#include <QResizeEvent>

ResizableLabel::ResizableLabel(QWidget *parent) :
    QLabel(parent) {
    image = new QImage();
}

void ResizableLabel::resizeEvent(QResizeEvent *e) {
    QPixmap p = QPixmap::fromImage(image->scaled(e->size().width(), e->size().height()));
    this->setPixmap(p);
}

void ResizableLabel::setImage(QImage *image) {
    this->image = image;
    this->setPixmap(QPixmap::fromImage(*image));
}
