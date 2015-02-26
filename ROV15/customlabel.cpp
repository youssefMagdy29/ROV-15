#include "customlabel.h"

#include <QPainter>
#include <QMouseEvent>

customLabel::customLabel(QWidget *parent)
    :QLabel(parent) {
    startX = 0;
    startY = 0;
    clicked = false;
}

void customLabel::paintEvent(QPaintEvent *e) {
    QLabel::paintEvent(e);

    QPainter painter(this);

    painter.drawLine(startX, startY, endX, endY);
}

void customLabel::mouseMoveEvent(QMouseEvent *e) {
    if (clicked == false) {
        startX = e->x();
        startY = e->y();
        clicked = true;
    }

    endX = e->x();
    endY = e->y();

    this->repaint();
}

void customLabel::mouseReleaseEvent(QMouseEvent *e) {
    clicked = false;
}
