#include "image.h"
#include "ui_image.h"

#include <QMouseEvent>
#include <QCameraViewfinder>
#include <QPainter>
#include <QDebug>

Image::Image(QImage *image, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Image)
{
    ui->setupUi(this);
    *image = image->mirrored(true, false);
    ui->label->setPixmap(QPixmap::fromImage(*image));
    this->image = image;
}

Image::~Image()
{
    delete ui;
}
