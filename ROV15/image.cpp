#include "image.h"
#include "ui_image.h"

#include <QMouseEvent>
#include <QCameraViewfinder>
#include <QPainter>
#include <QDebug>
#include <QtCore/qmath.h>

Image::Image(QImage *image, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Image)
{
    ui->setupUi(this);
    *image = image->mirrored(true, false);
    ui->label->setPixmap(QPixmap::fromImage(*image));
    this->image = image;

    connect(ui->label, SIGNAL(lineFinished(int,int,int,int)),
            this, SLOT(showResult(int, int, int, int)));
}

Image::~Image()
{
    delete ui;
}

void Image::showResult(int startX, int startY, int endX, int endY) {
    ui->valueStartX->setText(QString::number(startX));
    ui->valueStartY->setText(QString::number(startY));
    ui->valueEndX->setText(QString::number(endX));
    ui->valueEndY->setText(QString::number(endY));
    int width  = endX - startX;
    int height = endY - startY;
    float length = qSqrt(height * height + width * width);
    ui->valueLength->setText(QString::number(length));
}
