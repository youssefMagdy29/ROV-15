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

    QPen pen = QPen();
    pen.setColor(QColor(255, 0, 0));
    pen.setWidth(5);

    painter.setPen(pen);

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
    Q_UNUSED(e);

    clicked = false;
}
