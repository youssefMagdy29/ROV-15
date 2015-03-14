#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QtSerialPort/QSerialPort>
#include <QMessageBox>
#include <QDate>
#include <QTime>

#include <QtMultimedia/QCamera>
#include <QCameraImageCapture>
#include <QCameraViewfinder>
#include <QCameraInfo>

#include <QTimer>

#include "image.h"
#include "resizable_label.h"

#include <SFML/Window.hpp>

#include <QDesktopServices>

QSerialPort *serial;
QLabel *l;
QCamera *camera;
QVideoWidget *viewfinder;
QCameraImageCapture *imageCapture;
QGraphicsView *graphicsView;
ResizableLabel *lbl;
Image *image;
QImage img;
QCameraInfo camInfo;

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

//Vertical motors speed
QByteArray const VERTICAL_MOTORS_SPEED_UP   = "+";
QByteArray const VERTICAL_MOTORS_SPEED_DOWN = "-";

//Light..
//Commands...
QByteArray const LIGHT_ON  = "n";
QByteArray const LIGHT_OFF = "m";

//Keys...
QChar const KEY_LIGHT_ON  = 'N';
QChar const KEY_LIGHT_OFF = 'M';

//Camera1..
//Commands...
QByteArray const CAM1_RIGHT = "y";
QByteArray const CAM1_LEFT  = "t";
QByteArray const CAM1_STOP  = "o";

//Keys...
QChar const KEY_CAM1_RIGHT = 'Y';
QChar const KEY_CAM1_LEFT  = 'T';

//Camera2..
QByteArray const CAM2_RIGHT = "/";
QByteArray const CAM2_LEFT  = "*";
QByteArray const CAM2_STOP  = ",";

//Arm..
//Commands..
QByteArray const GRIPPER_STOP  = "0";
QByteArray const GRIPPER_RIGHT = "1";
QByteArray const GRIPPER_LEFT  = "2";

//Keys..
QChar const KEY_GRIPPER_STOP  = Qt::Key_0;
QChar const KEY_GRIPPER_RIGHT = Qt::Key_1;
QChar const KEY_GRIPPER_LEFT  = Qt::Key_2;

//Commands..
QByteArray const WRIST_STOP  = "3";
QByteArray const WRIST_RIGHT = "4";
QByteArray const WRIST_LEFT  = "5";

//Keys..
QChar const KEY_WRIST_STOP  = Qt::Key_3;
QChar const KEY_WRIST_RIGHT = Qt::Key_4;
QChar const KEY_WRIST_LEFT  = Qt::Key_5;

//Commands..
QByteArray const ELBOW_STOP  = "6";
QByteArray const ELBOW_RIGHT = "7";
QByteArray const ELBOW_LEFT  = "8";

//Keys..
QChar const KEY_ELBOW_STOP  = Qt::Key_6;
QChar const KEY_ELBOW_RIGHT = Qt::Key_7;
QChar const KEY_ELBOW_LEFT  = Qt::Key_8;

//Commands..
QByteArray const SHOULDER_STOP  = "9";
QByteArray const SHOULDER_RIGHT = "q";
QByteArray const SHOULDER_LEFT  = "e";

//Keys..
QChar const KEY_SHOULDER_STOP  = Qt::Key_9;
QChar const KEY_SHOULDER_RIGHT = 'Q';
QChar const KEY_SHOULDER_LEFT  = 'E';

//Commands..
QByteArray const BASE_STOP  = "j";
QByteArray const BASE_RIGHT = "k";
QByteArray const BASE_LEFT  = "i";

//Keys..
QChar const KEY_BASE_STOP  = 'H';
QChar const KEY_BASE_RIGHT = 'V';
QChar const KEY_BASE_LEFT  = 'U';

//Speed Commands
QByteArray const ARM_SPEED_UP   = "z";
QByteArray const ARM_SPEED_DOWN = "p";

//Release actions
QByteArray const ACTION_RELEASE_JOYSTICK1_UP     = STOP_HORIZONTAL;
QByteArray const ACTION_RELEASE_JOYSTICK1_DOWN   = STOP_HORIZONTAL;
QByteArray const ACTION_RELEASE_JOYSTICK1_RIGHT  = STOP_HORIZONTAL;
QByteArray const ACTION_RELEASE_JOYSTICK1_LEFT   = STOP_HORIZONTAL;
QByteArray const ACTION_RELEASE_JOYSTICK1_1      = STOP_VERTICAL;
QByteArray const ACTION_RELEASE_JOYSTICK1_2      = "";
QByteArray const ACTION_RELEASE_JOYSTICK1_3      = STOP_VERTICAL;
QByteArray const ACTION_RELEASE_JOYSTICK1_4      = "";
QByteArray const ACTION_RELEASE_JOYSTICK1_R1     = STOP_HORIZONTAL;
QByteArray const ACTION_RELEASE_JOYSTICK1_R2     = "";
QByteArray const ACTION_RELEASE_JOYSTICK1_L1     = STOP_HORIZONTAL;
QByteArray const ACTION_RELEASE_JOYSTICK1_L2     = "";
QByteArray const ACTION_RELEASE_JOYSTICK1_START  = "";
QByteArray const ACTION_RELEASE_JOYSTICK1_SELECT = "";

//Co-pilot Control
//Press actions
QByteArray const ACTION_PRESS_JOYSTICK2_UP     = ELBOW_LEFT;
QByteArray const ACTION_PRESS_JOYSTICK2_DOWN   = ELBOW_RIGHT;
QByteArray const ACTION_PRESS_JOYSTICK2_RIGHT  = CAM1_RIGHT;
QByteArray const ACTION_PRESS_JOYSTICK2_LEFT   = CAM1_LEFT;
QByteArray const ACTION_PRESS_JOYSTICK2_1      = SHOULDER_LEFT;
QByteArray const ACTION_PRESS_JOYSTICK2_2      = GRIPPER_LEFT;
QByteArray const ACTION_PRESS_JOYSTICK2_3      = SHOULDER_RIGHT;
QByteArray const ACTION_PRESS_JOYSTICK2_4      = GRIPPER_RIGHT;
QByteArray const ACTION_PRESS_JOYSTICK2_R1     = WRIST_LEFT;
QByteArray const ACTION_PRESS_JOYSTICK2_R2     = WRIST_RIGHT;
QByteArray const ACTION_PRESS_JOYSTICK2_L1     = ARM_SPEED_UP;
QByteArray const ACTION_PRESS_JOYSTICK2_L2     = ARM_SPEED_DOWN;
QByteArray const ACTION_PRESS_JOYSTICK2_START  = CAM2_RIGHT;
QByteArray const ACTION_PRESS_JOYSTICK2_SELECT = CAM2_LEFT;

//Release actions
QByteArray const ACTION_RELEASE_JOYSTICK2_UP     = ELBOW_STOP;
QByteArray const ACTION_RELEASE_JOYSTICK2_DOWN   = ELBOW_STOP;
QByteArray const ACTION_RELEASE_JOYSTICK2_RIGHT  = CAM1_STOP;
QByteArray const ACTION_RELEASE_JOYSTICK2_LEFT   = CAM1_STOP;
QByteArray const ACTION_RELEASE_JOYSTICK2_1      = SHOULDER_STOP;
QByteArray const ACTION_RELEASE_JOYSTICK2_2      = GRIPPER_STOP;
QByteArray const ACTION_RELEASE_JOYSTICK2_3      = SHOULDER_STOP;
QByteArray const ACTION_RELEASE_JOYSTICK2_4      = GRIPPER_STOP;
QByteArray const ACTION_RELEASE_JOYSTICK2_R1     = WRIST_STOP;
QByteArray const ACTION_RELEASE_JOYSTICK2_R2     = WRIST_STOP;
QByteArray const ACTION_RELEASE_JOYSTICK2_L1     = "";
QByteArray const ACTION_RELEASE_JOYSTICK2_L2     = "";
QByteArray const ACTION_RELEASE_JOYSTICK2_START  = CAM2_STOP;
QByteArray const ACTION_RELEASE_JOYSTICK2_SELECT = CAM2_STOP;

bool mode;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    l = ui->labelSentCommand;

    ui->buttonDisconnect->setDisabled(true);

    setupJoystick();

    lbl = new ResizableLabel;
    lbl->setWindowTitle("Screen Shot");

    image = new Image(new QImage());

    //Serial connection setup
    serial = new QSerialPort(this);

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    //Camera setup
     foreach (const QCameraInfo &CameraInfo, QCameraInfo::availableCameras()) {
         if (CameraInfo.description() == "SMI Grabber Device") {
             camInfo = CameraInfo;
         }
         else {
             camInfo = QCameraInfo::defaultCamera();
         }
     }

    camera = new QCamera(camInfo);
    viewfinder = new QVideoWidget;

    graphicsView = ui->graphicsView;

    QGraphicsScene *scene = new QGraphicsScene;

    scene->addWidget(viewfinder);

    graphicsView->scale(-1, 1);
    graphicsView->setScene(scene);

    camera->setViewfinder(viewfinder);

    imageCapture = new QCameraImageCapture(camera);

    connect(imageCapture, SIGNAL(imageSaved(int,QString)),
            this, SLOT(imageSaved(int, QString)));

    camera->setCaptureMode(QCamera::CaptureStillImage);
    if (camInfo.description() == "SMI Grabber Device")
        camera->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    serial->close();
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
        else if (k == KEY_CAM1_LEFT || k == KEY_CAM1_RIGHT) {
            l->setText(CAM1_STOP);
            serial->write(CAM1_STOP);
        }
        else if (k == KEY_GRIPPER_RIGHT || k == KEY_GRIPPER_LEFT) {
            l->setText(GRIPPER_STOP);
            serial->write(GRIPPER_STOP);
        }
        else if (k == KEY_WRIST_RIGHT || k == KEY_WRIST_LEFT) {
            l->setText(WRIST_STOP);
            serial->write(WRIST_STOP);
        }
        else if (k == KEY_ELBOW_RIGHT || k == KEY_ELBOW_LEFT) {
            l->setText(ELBOW_STOP);
            serial->write(ELBOW_STOP);
        }
        else if (k == KEY_SHOULDER_RIGHT || k == KEY_SHOULDER_LEFT) {
            l->setText(SHOULDER_STOP);
            serial->write(SHOULDER_STOP);
        }
        else if (k == KEY_BASE_RIGHT || k == KEY_BASE_LEFT) {
            l->setText(BASE_STOP);
            serial->write(BASE_STOP);
        }
    }
}

void MainWindow::on_buttonConnect_clicked()
{
    serial->setPortName(ui->lineEditCOMNumber->text());
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if(serial->open(QIODevice::ReadWrite)) {
        ui->labelConnectionStatus->setText("Connected");
        ui->labelConnectionStatus->setStyleSheet("color: #00ff00");
        ui->buttonConnect->setDisabled(true);
        ui->buttonDisconnect->setEnabled(true);
    }
    else {
        QMessageBox::critical(this, tr("Error"), "Invalid Serial port");
    }
}

void MainWindow::on_buttonDisconnect_clicked()
{
    if (serial->isOpen()) {
        serial->close();
        ui->labelConnectionStatus->setText("Disconnected");
        ui->labelConnectionStatus->setStyleSheet("color: #ff0000");
        ui->buttonConnect->setEnabled(true);
        ui->buttonDisconnect->setDisabled(true);
    }
    else {
        QMessageBox::critical(this, tr("Error"), "Not Connected!");
    }
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
        else if (k == KEY_CAM1_RIGHT) {
            l->setText(CAM1_RIGHT);
            serial->write(CAM1_RIGHT);
        }
        else if (k == KEY_CAM1_LEFT) {
            l->setText(CAM1_LEFT);
            serial->write(CAM1_LEFT);
        }
        else if (k == KEY_LIGHT_ON) {
            l->setText("$");
            serial->write("$");
        }
        else if (k == KEY_LIGHT_OFF) {
            l->setText("%");
            serial->write("%");
        }
        else if (k == KEY_GRIPPER_RIGHT) {
            l->setText(GRIPPER_RIGHT);
            serial->write(GRIPPER_RIGHT);
        }
        else if (k == KEY_GRIPPER_LEFT) {
            l->setText(GRIPPER_LEFT);
            serial->write(GRIPPER_LEFT);
        }
        else if (k == KEY_WRIST_RIGHT) {
            l->setText(WRIST_RIGHT);
            serial->write(WRIST_RIGHT);
        }
        else if (k == KEY_WRIST_LEFT) {
            l->setText(WRIST_LEFT);
            serial->write(WRIST_LEFT);
        }
        else if (k == KEY_ELBOW_RIGHT) {
            l->setText(ELBOW_RIGHT);
            serial->write(ELBOW_RIGHT);
        }
        else if (k == KEY_ELBOW_LEFT) {
            l->setText(ELBOW_LEFT);
            serial->write(ELBOW_LEFT);
        }
        else if (k == KEY_SHOULDER_RIGHT) {
            l->setText(SHOULDER_RIGHT);
            serial->write(SHOULDER_RIGHT);
        }
        else if (k == KEY_SHOULDER_LEFT) {
            l->setText(SHOULDER_LEFT);
            serial->write(SHOULDER_LEFT);
        }
        else if (k == KEY_BASE_RIGHT) {
            l->setText(BASE_RIGHT);
            serial->write(BASE_RIGHT);
        }
        else if (k == KEY_BASE_LEFT) {
            l->setText(BASE_LEFT);
            serial->write(BASE_LEFT);
        }
    }
}

void MainWindow::readData() {
    QByteArray data;
    while (serial->canReadLine()) {
        data = serial->readAll();
        data = data.trimmed();
        qDebug() << data;
        ui->valueLeakage->setText(data);
    }
}

void MainWindow::on_captureButton_clicked()
{
    imageCapture->capture();
}

void MainWindow::imageSaved(int id, QString str) {
    Q_UNUSED(id);

    img = QImage(str);
    img = img.mirrored(true, false);

    if (mode) {
        image->setImage(img);
        image->show();
    }
    else {
        QByteArray fileformat = "jpeg";
        lbl->setPixmap(QPixmap::fromImage(img.scaled(lbl->width(), lbl->height())));
        lbl->setImage(&img);
        lbl->show();
        QString filename = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/ROV_ScreenShots/"
                + QDate::currentDate().toString(Qt::ISODate) + " " +
                QTime::currentTime().toString("hh:mm:ss.zzz").replace(":", "_").replace(".", "_") + ".jpeg";
        img.save(filename, fileformat.constData());
    }
}

void MainWindow::setupJoystick() {
    j1 = new Joystick(Joystick::JOYSTICK1);
    j2 = new Joystick(Joystick::JOYSTICK2);

    initializeJ1ActionPress();

    //Connectivity signals
    connect(j1, SIGNAL(connected()), this, SLOT(joystickConnected()));
    connect(j1, SIGNAL(disconnected()), this, SLOT(joystickDisconnected()));

    //Button pressing signals
    connect(j1, SIGNAL(buttonPressed(int)), this, SLOT(joystick1ButtonPressed(int)));
    connect(j2, SIGNAL(buttonPressed(int)), this, SLOT(joystick2ButtonPressed(int)));

    //Button releasing signals
    connect(j1, SIGNAL(buttonReleased(int)), this, SLOT(joystick1ButtonReleased(int)));
    connect(j2, SIGNAL(buttonReleased(int)), this, SLOT(joystick2ButtonReleased(int)));
}

void MainWindow::joystickConnected() {
    ui->joystickStatus->setText("Connected");
    ui->joystickStatus->setStyleSheet("color: #00ff00");
}

void MainWindow::joystickDisconnected() {
    ui->joystickStatus->setText("Not Connected");
    ui->joystickStatus->setStyleSheet("color: #ff0000");
}

void MainWindow::joystick1ButtonPressed(int id) {
    l->setText(j1ActionPress[id]);
    serial->write(j1ActionPress[id]);
}

void MainWindow::joystick1ButtonReleased(int id) {

}

void MainWindow::joystick2ButtonPressed(int id) {

}

void MainWindow::joystick2ButtonReleased(int id) {

}

void MainWindow::initializeJ1ActionPress() {
    j1ActionPress[Joystick::BUTTON_UP]     = FORWARD;
    j1ActionPress[Joystick::BUTTON_DOWN]   = BACKWARD;
    j1ActionPress[Joystick::BUTTON_RIGHT]  = MOVE_RIGHT;
    j1ActionPress[Joystick::BUTTON_LEFT]   = MOVE_LEFT;
    j1ActionPress[Joystick::BUTTON_1]      = UP;
    j1ActionPress[Joystick::BUTTON_2]      = "";
    j1ActionPress[Joystick::BUTTON_3]      = DOWN;
    j1ActionPress[Joystick::BUTTON_4]      = LIGHT_ON;
    j1ActionPress[Joystick::BUTTON_R1]     = TURN_RIGHT;
    j1ActionPress[Joystick::BUTTON_R2]     = VERTICAL_MOTORS_SPEED_UP;
    j1ActionPress[Joystick::BUTTON_L1]     = TURN_LEFT;
    j1ActionPress[Joystick::BUTTON_L2]     = VERTICAL_MOTORS_SPEED_DOWN;
    j1ActionPress[Joystick::BUTTON_START]  = "";
    j1ActionPress[Joystick::BUTTON_SELECT] = "";
}
