#include "image.h"
#include "ui_image.h"

#include <QMouseEvent>
#include <QCameraViewfinder>
#include <QPainter>
#include <QDebug>
#include <QtCore/qmath.h>
#include <QDir>
#include <QFileDialog>
#include <QMimeData>
#include <QDate>
#include <QTime>
#include <QStandardPaths>

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
    setAcceptDrops(true);
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
    float cm = 50.6  + length * 0.2 *0.278;
    ui->valueLength->setText(QString::number(cm));
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

void Image::dragEnterEvent(QDragEnterEvent *e) {
    if (e->mimeData()->hasUrls())
        e->acceptProposedAction();
}

void Image::dropEvent(QDropEvent *e) {
    foreach(const QUrl &url, e->mimeData()->urls()) {
        const QString &fileName = url.toLocalFile();
        QByteArray data = e->mimeData()->data("jpeg");
        ui->label->setPixmap(QPixmap::fromImage(QImage(fileName, data)));
    }
}

void Image::show() {
    QMainWindow::show();

    QByteArray fileformat = "jpeg";

    QString filename = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/ROV_ScreenShots/"
            + QDate::currentDate().toString(Qt::ISODate) + " " +
            QTime::currentTime().toString("hh:mm:ss.zzz").replace(":", "_").replace(".", "_") + ".jpeg";


    qDebug() << filename;

    this->image->save(filename, fileformat.constData());
}
