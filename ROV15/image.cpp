#include "image.h"
#include "ui_image.h"

#include <QMouseEvent>
#include <QCameraViewfinder>
#include <QPainter>
#include <QDebug>
#include <QtCore/qmath.h>
#include <QDir>
#include <QFileDialog>

Image::Image(QImage *image, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Image)
{
    ui->setupUi(this);
    *image = image->mirrored(true, false);
    ui->label->setPixmap(QPixmap::fromImage(*image));
    this->image = image;

    connect(ui->label, SIGNAL(lineFinished(int,int,int,int)),
            this, SLOT(showResult(int, int, int, int)));
    connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exit()));
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

void Image::saveAs() {
    const QByteArray fileFormat = "jpeg";

    const QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    const QString fileName =
            QFileDialog::getSaveFileName(this, tr("Save As"),
                                         initialPath,
                                         tr("%1 Files (*.%2);;All Files (*)")
                                         .arg(QString::fromLatin1(
                                                  fileFormat.toUpper()))
                                         .arg(QString::fromLatin1(fileFormat)));
    if (image->save(fileName, fileFormat.constData())) {
        qDebug() << "Success";
    }
    else {
        qDebug() << "Fail";
    }
}

void Image::setImage(QImage &image) {
    this->image = &image;
    ui->label->setPixmap(QPixmap::fromImage(image));
}

void Image::exit() {
    this->close();
}
