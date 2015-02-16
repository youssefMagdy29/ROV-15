#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>

QLabel *l;
//Directions...
    //Commands...
    QString const FORWARD         = "f";
    QString const BACKWARD        = "b";
    QString const MOVE_RIGHT      = "x";
    QString const MOVE_LEFT       = "c";
    QString const TURN_RIGHT      = "r";
    QString const TURN_LEFT       = "l";
    QString const UP              = "u";
    QString const DOWN            = "d";
    QString const STOP_HORIZONTAL = "h";
    QString const STOP_VERTICAL   = "v";
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
    QString const LIGHT_ON  = "n";
    QString const LIGHT_OFF = "m";

    //Keys...
    QChar const KEY_LIGHT_ON  = 'N';
    QChar const KEY_LIGHT_OFF = 'M';

//Camera..
    //Commands...
    QString const CAM_RIGHT = "y";
    QString const CAM_LEFT  = "t";
    QString const CAM_STOP  = "o";

    //Keys...
    QChar const KEY_CAM_RIGHT = 'Y';
    QChar const KEY_CAM_LEFT  = 'T';

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    l = ui->labelSentCommand;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    QChar k = e->key();
    if (k == KEY_FORWARD) {
        l->setText(FORWARD);
    }
    else if (k == KEY_BACKWARD) {
        l->setText(BACKWARD);
    }
    else if (k == KEY_MOVE_RIGHT) {
        l->setText(MOVE_RIGHT);
    }
    else if (k == KEY_MOVE_LEFT) {
        l->setText(MOVE_LEFT);
    }
    else if (k == KEY_TURN_RIGHT) {
        l->setText(TURN_RIGHT);
    }
    else if (k == KEY_TURN_LEFT) {
        l->setText(TURN_LEFT);
    }
    else if (k == KEY_UP) {
        l->setText(UP);
    }
    else if (k == KEY_DOWN) {
        l->setText(DOWN);
    }
    else if (k == KEY_CAM_RIGHT) {
        l->setText(CAM_RIGHT);
    }
    else if (k == KEY_CAM_LEFT) {
        l->setText(CAM_LEFT);
    }
    else if (k == KEY_LIGHT_ON) {
        l->setText(LIGHT_ON);
    }
    else if (k == KEY_LIGHT_OFF) {
        l->setText(LIGHT_OFF);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e) {
    QChar k = e->key();
    if (k == KEY_FORWARD    || k == KEY_BACKWARD  || k == KEY_MOVE_LEFT ||
        k == KEY_MOVE_RIGHT || k == KEY_TURN_LEFT || k == KEY_TURN_RIGHT) {
        l->setText(STOP_HORIZONTAL);
    }
    else if (k == KEY_UP || k == KEY_DOWN) {
        l->setText(STOP_VERTICAL);
    }
    else if (k == KEY_CAM_LEFT || k == KEY_CAM_RIGHT) {
        l->setText(CAM_STOP);
    }
}

void MainWindow::on_buttonConnect_clicked()
{
    ui->labelConnectionStatus->setText("Connected");
    ui->labelConnectionStatus->setStyleSheet("color: #00ff00");
}

void MainWindow::on_buttonDisconnect_clicked()
{
    ui->labelConnectionStatus->setText("Disconnected");
    ui->labelConnectionStatus->setStyleSheet("color: #ff0000");
}
