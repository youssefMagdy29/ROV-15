#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QtSerialPort/QSerialPort>
#include <QMessageBox>

QSerialPort *serial;
QLabel *l;
QByteArray prevCmd;
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    l = ui->labelSentCommand;
    serial = new QSerialPort(this);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
}

MainWindow::~MainWindow()
{
    delete ui;
    serial->close();
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    QChar k = e->key();
    if (k == KEY_FORWARD) {
        prevCmd = FORWARD;
        l->setText(FORWARD);
        serial->write(FORWARD);
    }
    else if (k == KEY_BACKWARD) {
        prevCmd = BACKWARD;
        l->setText(BACKWARD);
        serial->write(BACKWARD);
    }
    else if (k == KEY_MOVE_RIGHT) {
        prevCmd = MOVE_RIGHT;
        l->setText(MOVE_RIGHT);
        serial->write(MOVE_RIGHT);
    }
    else if (k == KEY_MOVE_LEFT) {
        prevCmd = MOVE_LEFT;
        l->setText(MOVE_LEFT);
        serial->write(MOVE_LEFT);
    }
    else if (k == KEY_TURN_RIGHT) {
        prevCmd = TURN_RIGHT;
        l->setText(TURN_RIGHT);
        serial->write(TURN_RIGHT);
    }
    else if (k == KEY_TURN_LEFT) {
        prevCmd = TURN_LEFT;
        l->setText(TURN_LEFT);
        serial->write(TURN_LEFT);
    }
    else if (k == KEY_UP) {
        prevCmd = UP;
        l->setText(UP);
        serial->write(UP);
    }
    else if (k == KEY_DOWN) {
        prevCmd = DOWN;
        l->setText(DOWN);
        serial->write(DOWN);
    }
    else if (k == KEY_CAM_RIGHT) {
        prevCmd = CAM_RIGHT;
        l->setText(CAM_RIGHT);
        serial->write(CAM_RIGHT);
    }
    else if (k == KEY_CAM_LEFT) {
        prevCmd = CAM_LEFT;
        l->setText(CAM_LEFT);
        serial->write(CAM_LEFT);
    }
    else if (k == KEY_LIGHT_ON) {
        prevCmd = LIGHT_ON;
        l->setText(LIGHT_ON);
        serial->write(LIGHT_ON);
    }
    else if (k == KEY_LIGHT_OFF) {
        prevCmd = LIGHT_OFF;
        l->setText(LIGHT_OFF);
        serial->write(LIGHT_OFF);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e) {
    QChar k = e->key();
    if (k == KEY_FORWARD    || k == KEY_BACKWARD  || k == KEY_MOVE_LEFT ||
        k == KEY_MOVE_RIGHT || k == KEY_TURN_LEFT || k == KEY_TURN_RIGHT) {
        prevCmd = "";
        l->setText(STOP_HORIZONTAL);
        serial->write(STOP_HORIZONTAL);
    }
    else if (k == KEY_UP || k == KEY_DOWN) {
        prevCmd = "";
        l->setText(STOP_VERTICAL);
        serial->write(STOP_VERTICAL);
    }
    else if (k == KEY_CAM_LEFT || k == KEY_CAM_RIGHT) {
        prevCmd = "";
        l->setText(CAM_STOP);
        serial->write(CAM_STOP);
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
}
