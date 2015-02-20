#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QtSerialPort/QSerialPort>
#include <QMessageBox>

#include <QtMultimedia/QCamera>
#include <QCameraImageCapture>
#include <QCameraViewfinder>

QSerialPort *serial;
QLabel *l;
QCamera *camera;
QCameraViewfinder *viewfinder;
QCameraImageCapture *imageCapture;

//Directions...
//Commands...
QByteArray const FORWARD         = "f";
QByteArray const BACKWARD        = "b";
QByteArray const MOVE_RIGHT      = "x";
QByteArray const MOVE_LEFT       = "c";
QByteArray const TURN_RIGHT      = "r";
QByteArray const TURN_LEFT       = "l";
QByteArray const UP              = "u";
QByteArray const DOWN            = "d";
QByteArray const STOP_HORIZONTAL = "h";
QByteArray const STOP_VERTICAL   = "v";
//Keys...
QChar const KEY_FORWARD    = 'W';
QChar const KEY_BACKWARD   = 'S';
QChar const KEY_MOVE_RIGHT = 'D';
QChar const KEY_MOVE_LEFT  = 'A';
QChar const KEY_TURN_RIGHT = 'K';
QChar const KEY_TURN_LEFT  = 'L';
QChar const KEY_UP         = 'I';
QChar const KEY_DOWN       = 'J';

//Light..
//Commands...
QByteArray const LIGHT_ON  = "n";
QByteArray const LIGHT_OFF = "m";

//Keys...
QChar const KEY_LIGHT_ON  = 'N';
QChar const KEY_LIGHT_OFF = 'M';

//Camera..
//Commands...
QByteArray const CAM_RIGHT = "y";
QByteArray const CAM_LEFT  = "t";
QByteArray const CAM_STOP  = "o";

//Keys...
QChar const KEY_CAM_RIGHT = 'Y';
QChar const KEY_CAM_LEFT  = 'T';

//Arm..
QByteArray const GRIPPER_STOP  = '0';
QByteArray const GRIPPER_RIGHT = '1';
QByteArray const GRIPPER_LEFT  = '2';

QByteArray const WRIST_STOP  = '3';
QByteArray const WRIST_RIGHT = '4';
QByteArray const WRIST_LEFT  = '5';

QByteArray const ELBOW_STOP  = '6';
QByteArray const ELBOW_RIGHT = '7';
QByteArray const ELBOW_LEFT  = '8';

QByteArray const SHOULDER_STOP  = '9';
QByteArray const SHOULDER_RIGHT = 'q';
QByteArray const SHOULDER_LEFT  = 'e';

QByteArray const BASE_STOP  = 'j';
QByteArray const BASE_RIGHT = 'k';
QByteArray const BASE_LEFT  = 'i';

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    l = ui->labelSentCommand;

    //Serial connection setup
    serial = new QSerialPort(this);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    //Camera setup
    camera = new QCamera;

    viewfinder = ui->viewfinder;
    viewfinder->show();

    camera->setViewfinder(viewfinder);

    imageCapture = new QCameraImageCapture(camera);

    camera->setCaptureMode(QCamera::CaptureStillImage);
    //camera->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    serial->close();
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    QChar k = e->key();
    if (!e->isAutoRepeat()) {
        if (k == KEY_FORWARD) {
            l->setText(FORWARD);
            serial->write(FORWARD);
        }
        else if (k == KEY_BACKWARD) {
            l->setText(BACKWARD);
            serial->write(BACKWARD);
        }
        else if (k == KEY_MOVE_RIGHT) {
            l->setText(MOVE_RIGHT);
            serial->write(MOVE_RIGHT);
        }
        else if (k == KEY_MOVE_LEFT) {
            l->setText(MOVE_LEFT);
            serial->write(MOVE_LEFT);
        }
        else if (k == KEY_TURN_RIGHT) {
            l->setText(TURN_RIGHT);
            serial->write(TURN_RIGHT);
        }
        else if (k == KEY_TURN_LEFT) {
            l->setText(TURN_LEFT);
            serial->write(TURN_LEFT);
        }
        else if (k == KEY_UP) {
            l->setText(UP);
            serial->write(UP);
        }
        else if (k == KEY_DOWN) {
            l->setText(DOWN);
            serial->write(DOWN);
        }
        else if (k == KEY_CAM_RIGHT) {
            l->setText(CAM_RIGHT);
            serial->write(CAM_RIGHT);
        }
        else if (k == KEY_CAM_LEFT) {
            l->setText(CAM_LEFT);
            serial->write(CAM_LEFT);
        }
        else if (k == KEY_LIGHT_ON) {
            l->setText(LIGHT_ON);
            serial->write(LIGHT_ON);
        }
        else if (k == KEY_LIGHT_OFF) {
            l->setText(LIGHT_OFF);
            serial->write(LIGHT_OFF);
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e) {
    QChar k = e->key();
    if (!e->isAutoRepeat()) {
        if (k == KEY_FORWARD    || k == KEY_BACKWARD  || k == KEY_MOVE_LEFT ||
                k == KEY_MOVE_RIGHT || k == KEY_TURN_LEFT || k == KEY_TURN_RIGHT) {
            l->setText(STOP_HORIZONTAL);
            serial->write(STOP_HORIZONTAL);
        }
        else if (k == KEY_UP || k == KEY_DOWN) {
            l->setText(STOP_VERTICAL);
            serial->write(STOP_VERTICAL);
        }
        else if (k == KEY_CAM_LEFT || k == KEY_CAM_RIGHT) {
            l->setText(CAM_STOP);
            serial->write(CAM_STOP);
        }
    }
}

void MainWindow::on_buttonConnect_clicked()
{
    serial->setPortName(ui->lineEditCOMNumber->text());
    if(serial->open(QIODevice::ReadWrite)) {
        ui->labelConnectionStatus->setText("Connected");
        ui->labelConnectionStatus->setStyleSheet("color: #00ff00");
    }
    else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());
    }
}

void MainWindow::on_buttonDisconnect_clicked()
{
    if (serial->isOpen()) {
        serial->close();
        ui->labelConnectionStatus->setText("Disconnected");
        ui->labelConnectionStatus->setStyleSheet("color: #ff0000");
    }
    else {
        QMessageBox::critical(this, tr("Error"), "There is no serial connection");
    }
}

void MainWindow::readData() {
    QByteArray data = serial->readAll();
    ui->label_2->setText(data);
}
