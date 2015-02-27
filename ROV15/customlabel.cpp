#include "customlabel.h"

#include <QPainter>
#include <QMouseEvent>

customLabel::customLabel(QWidget *parent)
    :QLabel(parent) {
    startX = 0;
    startY = 0;
    endX   = 0;
    endY   = 0;
    clicked = false;
}

void customLabel::paintEvent(QPaintEvent *e) {
    QLabel::paintEvent(e);

    QPainter painter(this);

    QPen pen = QPen();
    pen.setColor(QColor(255, 0, 0));
    pen.setWidth(5);

    painter.setPen(pen);

    if (clicked) {
        painter.drawLine(startX, startY, endX, endY);
    }
}

void customLabel::mouseMoveEvent(QMouseEvent *e) {
    if (clicked == false) {
        startX = e->x();
        startY = e->y();
        clicked = true;
    }

    endX = e->x();
    endY = e->y();

    if (endX < 0)
        endX = 0;
    else if (endX > 640)
        endX = 640;
    if (endY < 0)
        endY = 0;
    else if (endY > 480)
        endY = 480;

    this->repaint();
}

void customLabel::mouseReleaseEvent(QMouseEvent *e) {
    Q_UNUSED(e);

    clicked = false;
    emit lineFinished(startX, startY, endX, endY);
}
