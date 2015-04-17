#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QMessageBox>
#include <QDate>
#include <QTime>
#include <QTimer>
#include <QDesktopServices>
#include <QtMath>
#include <QDir>

static const QByteArray FORWARD                    = "b";
static const QByteArray BACKWARD                   = "f";
static const QByteArray MOVE_RIGHT                 = "x";
static const QByteArray MOVE_LEFT                  = "c";
static const QByteArray TURN_RIGHT                 = "l";
static const QByteArray TURN_LEFT                  = "r";
static const QByteArray UP                         = "u";
static const QByteArray DOWN                       = "d";
static const QByteArray STOP_HORIZONTAL            = "h";
static const QByteArray STOP_VERTICAL              = "v";
static const QByteArray VERTICAL_MOTORS_SPEED_UP   = "+";
static const QByteArray VERTICAL_MOTORS_SPEED_DOWN = "-";
static const QByteArray LIGHT_ON                   = "n";
static const QByteArray LIGHT_OFF                  = "m";
static const QByteArray CAM1_RIGHT                 = "y";
static const QByteArray CAM1_LEFT                  = "t";
static const QByteArray CAM1_STOP                  = "o";
static const QByteArray CAM2_RIGHT                 = "/";
static const QByteArray CAM2_LEFT                  = "*";
static const QByteArray CAM2_STOP                  = ",";
static const QByteArray GRIPPER_STOP               = "0";
static const QByteArray GRIPPER_RIGHT              = "1";
static const QByteArray GRIPPER_LEFT               = "2";
static const QByteArray WRIST_STOP                 = "3";
static const QByteArray WRIST_RIGHT                = "4";
static const QByteArray WRIST_LEFT                 = "5";
static const QByteArray ELBOW_STOP                 = "6";
static const QByteArray ELBOW_RIGHT                = "7";
static const QByteArray ELBOW_LEFT                 = "8";
static const QByteArray SHOULDER_STOP              = "9";
static const QByteArray SHOULDER_RIGHT             = "q";
static const QByteArray SHOULDER_LEFT              = "e";
static const QByteArray BASE_STOP                  = "j";
static const QByteArray BASE_RIGHT                 = "k";
static const QByteArray BASE_LEFT                  = "i";
static const QByteArray ARM_SPEED_UP               = "z";
static const QByteArray ARM_SPEED_DOWN             = "p";

QTimer *tt;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    image(new Image(new QImage())),
    mode(false),
    xVel(0), yVel(0), zVel(0),
    xDst(0), yDst(0), zDst(0),
    SAMPLE_TIME(4.0 / 100)
{
    ui->setupUi(this);

    l = ui->labelSentCommand;

    ui->buttonDisconnect->setDisabled(true);

    initKeys();
    initializeKActionPress();
    initializeKActionRelease();

    setupSerialConnection();
    setupCamera();
    setupJoystick();

    initMissionsList();

    currentMission = missionsList[0];
    updateMission();

    for (int i = 0; i < Joystick::BUTTON_COUNT; i++) {
        toggleJ1[i] = false;
        toggleJ2[i] = false;
    }

    tt = new QTimer(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    serial->close();
}

void MainWindow::setupSerialConnection() {
    serial = new QSerialPort(this);
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
}

void MainWindow::setupCamera() {
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
   //if (camInfo.description() == "SMI Grabber Device")
       camera->start();
}

void MainWindow::initializeKActionPress() {
    keyboardActionPress[KEY_FORWARD]            = FORWARD;
    keyboardActionPress[KEY_BACKWARD]           = BACKWARD;
    keyboardActionPress[KEY_MOVE_RIGHT]         = MOVE_RIGHT;
    keyboardActionPress[KEY_MOVE_LEFT]          = MOVE_LEFT;
    keyboardActionPress[KEY_TURN_RIGHT]         = TURN_RIGHT;
    keyboardActionPress[KEY_TURN_LEFT]          = TURN_LEFT;
    keyboardActionPress[KEY_UP]                 = UP;
    keyboardActionPress[KEY_DOWN]               = DOWN;
    keyboardActionPress[KEY_VMOTORS_SPEED_UP]   = VERTICAL_MOTORS_SPEED_UP;
    keyboardActionPress[KEY_VMOTORS_SPEED_DOWN] = VERTICAL_MOTORS_SPEED_DOWN;
    keyboardActionPress[KEY_LIGHT_ON]           = LIGHT_ON;
    keyboardActionPress[KEY_LIGHT_OFF]          = LIGHT_OFF;
    keyboardActionPress[KEY_CAM1_RIGHT]         = CAM1_RIGHT;
    keyboardActionPress[KEY_CAM1_LEFT]          = CAM1_LEFT;
    keyboardActionPress[KEY_CAM2_RIGHT]         = CAM2_RIGHT;
    keyboardActionPress[KEY_CAM2_LEFT]          = CAM2_LEFT;
    keyboardActionPress[KEY_GRIPPER_RIGHT]      = GRIPPER_RIGHT;
    keyboardActionPress[KEY_GRIPPER_LEFT]       = GRIPPER_LEFT;
    keyboardActionPress[KEY_WRIST_RIGHT]        = WRIST_RIGHT;
    keyboardActionPress[KEY_WRIST_LEFT]         = WRIST_LEFT;
    keyboardActionPress[KEY_ELBOW_RIGHT]        = ELBOW_RIGHT;
    keyboardActionPress[KEY_ELBOW_LEFT]         = ELBOW_LEFT;
    keyboardActionPress[KEY_SHOULDER_RIGHT]     = SHOULDER_RIGHT;
    keyboardActionPress[KEY_SHOULDER_LEFT]      = SHOULDER_LEFT;
    keyboardActionPress[KEY_BASE_RIGHT]         = BASE_RIGHT;
    keyboardActionPress[KEY_BASE_LEFT]          = BASE_LEFT;
    keyboardActionPress[KEY_ARM_SPEED_UP]       = ARM_SPEED_UP;
    keyboardActionPress[KEY_ARM_SPEED_DOWN]     = ARM_SPEED_DOWN;
}

void MainWindow::initializeKActionRelease() {
    keyboardActionRelease[KEY_FORWARD]            = STOP_HORIZONTAL;
    keyboardActionRelease[KEY_BACKWARD]           = STOP_HORIZONTAL;
    keyboardActionRelease[KEY_MOVE_RIGHT]         = STOP_HORIZONTAL;
    keyboardActionRelease[KEY_MOVE_LEFT]          = STOP_HORIZONTAL;
    keyboardActionRelease[KEY_TURN_RIGHT]         = STOP_HORIZONTAL;
    keyboardActionRelease[KEY_TURN_LEFT]          = STOP_HORIZONTAL;
    keyboardActionRelease[KEY_UP]                 = STOP_VERTICAL;
    keyboardActionRelease[KEY_DOWN]               = STOP_VERTICAL;
    keyboardActionRelease[KEY_VMOTORS_SPEED_UP]   = "";
    keyboardActionRelease[KEY_VMOTORS_SPEED_DOWN] = "";
    keyboardActionRelease[KEY_LIGHT_ON]           = "";
    keyboardActionRelease[KEY_LIGHT_OFF]          = "";
    keyboardActionRelease[KEY_CAM1_RIGHT]         = CAM1_STOP;
    keyboardActionRelease[KEY_CAM1_LEFT]          = CAM1_STOP;
    keyboardActionRelease[KEY_CAM2_RIGHT]         = CAM2_STOP;
    keyboardActionRelease[KEY_CAM2_LEFT]          = CAM2_STOP;
    keyboardActionRelease[KEY_GRIPPER_RIGHT]      = GRIPPER_STOP;
    keyboardActionRelease[KEY_GRIPPER_LEFT]       = GRIPPER_STOP;
    keyboardActionRelease[KEY_WRIST_RIGHT]        = WRIST_STOP;
    keyboardActionRelease[KEY_WRIST_LEFT]         = WRIST_STOP;
    keyboardActionRelease[KEY_ELBOW_RIGHT]        = ELBOW_STOP;
    keyboardActionRelease[KEY_ELBOW_LEFT]         = ELBOW_STOP;
    keyboardActionRelease[KEY_SHOULDER_RIGHT]     = SHOULDER_STOP;
    keyboardActionRelease[KEY_SHOULDER_LEFT]      = SHOULDER_STOP;
    keyboardActionRelease[KEY_BASE_RIGHT]         = BASE_STOP;
    keyboardActionRelease[KEY_BASE_LEFT]          = BASE_STOP;
    keyboardActionRelease[KEY_ARM_SPEED_UP]       = "";
    keyboardActionRelease[KEY_ARM_SPEED_DOWN]     = "";
}

/*void MainWindow::keyPressEvent(QKeyEvent *e) {
    QByteArray cmd;

    if (e->key() < 255)
        cmd = keyboardActionPress[e->key()];

    if (!e->isAutoRepeat() && cmd != "") {
        l->setText(cmd);
        serial->write(cmd);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e) {
    QByteArray cmd;

    if (e->key() < 255)
        cmd = keyboardActionRelease[e->key()];

    if (!e->isAutoRepeat() && cmd != "") {
        l->setText(cmd);
        serial->write(cmd);
    }
}*/

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

void MainWindow::readData() {
    QByteArray data;
    while (serial->canReadLine()) {
        data = serial->readAll();
        data = data.trimmed();

        int b = data.indexOf('b');
        int c = data.indexOf('c');
        int p = data.indexOf('p');
        int t = data.indexOf('t');
        int s = data.indexOf('s');
        int l = data.indexOf('l');
        int x = data.indexOf('x');
        int y = data.indexOf('y');

        QByteArray QxAcc = data.mid(1, b - 1);
        QByteArray QyAcc = data.mid(b + 1, c - b - 1);
        QByteArray QzAcc = data.mid(c + 1, p - c - 1);

        QByteArray Qpres = data.mid(p + 1, t - p - 1);
        QByteArray Qtemp = data.mid(t + 1, s - t - 1);
        QByteArray Qserv = data.mid(s + 1, l - s - 1);

        QByteArray Qleak = data.mid(l + 1, x - l - 1);

        QByteArray Qmx   = data.mid(x + 1, y - x - 1);
        QByteArray Qmy   = data.mid(y + 1);

        double mx = Qmx.toDouble();
        double my = Qmy.toDouble();

        double heading = qAtan2(mx, my);

        if (heading < 0)
            heading += 2 * M_PI;

        double Qcomp = heading * 180 / M_PI;

        qDebug() << tt->remainingTime();

        if (tt->isActive())
            tt->stop();

        tt->start(1000);

        xAcc = QxAcc.toDouble() / 1712.0 - 0.3;
        yAcc = QyAcc.toDouble() / 1606.0 + 0.4;
        zAcc = QzAcc.toDouble() / 1548.0;

        xVel += xAcc * SAMPLE_TIME;
        yVel += yAcc * SAMPLE_TIME;
        zVel += zAcc * SAMPLE_TIME;

        xDst += xVel * SAMPLE_TIME;
        yDst += yVel * SAMPLE_TIME;
        zDst += zVel * SAMPLE_TIME;

        ui->valueXAcceleration->setText(QString::number(xAcc));
        ui->valueYAcceleration->setText(QString::number(yAcc));
        ui->valueZAcceleration->setText(QString::number(zAcc));

        ui->valueXVelocity->setText(QString::number(xVel));
        ui->valueYVelocity->setText(QString::number(yVel));
        ui->valueZVelocity->setText(QString::number(zVel));

        ui->valueXDistance->setText(QString::number(xDst));
        ui->valueYDistance->setText(QString::number(yDst));
        ui->valueZDistance->setText(QString::number(zDst));

        ui->valuePressure->setText(Qpres);
        ui->valueTemperature->setText(Qtemp);
        ui->valueCamera->setText(Qserv);

        ui->valueLeakage->setText(Qleak);

        ui->valueCompass->setText(QString::number(Qcomp));
    }
}

void MainWindow::on_sendButton_clicked() {
    QString cmd = ui->lineEditTerminalCmd->text();
    ui->lineEditTerminalCmd->clear();
    l->setText(cmd);
    serial->write(cmd.toUtf8());
}

void MainWindow::on_captureButton_clicked()
{
    imageCapture->capture();
}

void MainWindow::imageSaved(int id, QString str) {
    Q_UNUSED(id);

    img = QImage(str);
    img = img.mirrored(true, false);

    if (!mode) {
        image->setImage(img);
        image->setMission(currentMission);
        image->show();
    }
    else {
        QByteArray fileformat = "jpeg";
        ui->lastScreenShot->setPixmap(QPixmap::fromImage(
                                          img.scaled(ui->lastScreenShot->width(), ui->lastScreenShot->height())));
        QString basePath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/ROV_ScreenShots/"
                + currentMission->getDemo() + "/" + currentMission->getName() + "/";
        QDir dir(basePath);
        if (!dir.exists())
            dir.mkpath(".");
        QString filename = basePath  + QDate::currentDate().toString(Qt::ISODate) + " " +
                QTime::currentTime().toString("hh:mm:ss.zzz").replace(":", "_").replace(".", "_") + ".jpeg";
        img.save(filename, fileformat.constData());
    }
}

void MainWindow::setupJoystick() {
    j1 = new Joystick(Joystick::JOYSTICK1);
    j2 = new Joystick(Joystick::JOYSTICK2);

    initializeJ1ButtonKinds();
    initializeJ1ActionPress();
    initializeJ1ActionRelease();
    initializeJ2ActionPress();
    initializeJ2ActionRelease();

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
    int kind = j1->getKind(id);

    if (kind == Joystick::CAPTURE)
        imageCapture->capture();

    else if (kind == Joystick::TOGGLE) {
        toggleJ1[id] = !toggleJ1[id];
        if (toggleJ1[id]) {
            l->setText(j1ActionPress[id]);
            serial->write(j1ActionPress[id]);
        }
        else {
            l->setText(j1ActionRelease[id]);
            serial->write(j1ActionRelease[id]);
        }
    }

    else if (kind == Joystick::MODE) {
        mode = !mode;
        if (mode)
            ui->valueMode->setText("Measuring");
        else
            ui->valueMode->setText("Capturing");
    }

    else if (j1ActionPress[id] != "") {
        l->setText(j1ActionPress[id]);
        serial->write(j1ActionPress[id]);
    }
}

void MainWindow::joystick1ButtonReleased(int id) {
    if (j1ActionRelease[id] != ""
            && j1->getKind(id) != Joystick::TOGGLE) {
        l->setText(j1ActionRelease[id]);
        serial->write(j1ActionRelease[id]);
    }
}

void MainWindow::joystick2ButtonPressed(int id) {
    int kind = j2->getKind(id);

    if (kind == Joystick::TOGGLE) {
        toggleJ2[id] = !toggleJ2[id];
        if (toggleJ2[id]) {
            l->setText(j2ActionPress[id]);
            serial->write(j2ActionPress[id]);
        }
        else {
            l->setText(j2ActionRelease[id]);
            serial->write(j2ActionRelease[id]);
        }
    }
    else if (j2ActionPress[id] != "") {
        l->setText(j2ActionPress[id]);
        serial->write(j2ActionPress[id]);
    }
}

void MainWindow::joystick2ButtonReleased(int id) {
    if (j2ActionRelease[id] != ""
            && j2->getKind(id) != Joystick::TOGGLE) {
        l->setText(j2ActionRelease[id]);
        serial->write(j2ActionRelease[id]);
    }
}

void MainWindow::initializeJ1ButtonKinds() {
    j1->setKind(Joystick::BUTTON_2, Joystick::CAPTURE);
    j1->setKind(Joystick::BUTTON_ANALOG_2, Joystick::CAPTURE);
    j1->setKind(Joystick::BUTTON_4, Joystick::TOGGLE);
    j1->setKind(Joystick::BUTTON_ANALOG_4, Joystick::TOGGLE);
    j1->setKind(Joystick::BUTTON_START, Joystick::MODE);
    j1->setKind(Joystick::BUTTON_R2, Joystick::AUTO_REPEAT);
    j1->setKind(Joystick::BUTTON_L2, Joystick::AUTO_REPEAT);
}

void MainWindow::initializeJ1ActionPress() {
    j1ActionPress[Joystick::BUTTON_UP_OFF]     = FORWARD;
    j1ActionPress[Joystick::BUTTON_DOWN_OFF]   = BACKWARD;
    j1ActionPress[Joystick::BUTTON_RIGHT_OFF]  = MOVE_RIGHT;
    j1ActionPress[Joystick::BUTTON_LEFT_OFF]   = MOVE_LEFT;
    j1ActionPress[Joystick::BUTTON_UP_ON]      = FORWARD;
    j1ActionPress[Joystick::BUTTON_DOWN_ON]    = BACKWARD;
    j1ActionPress[Joystick::BUTTON_RIGHT_ON]   = MOVE_RIGHT;
    j1ActionPress[Joystick::BUTTON_LEFT_ON]    = MOVE_LEFT;
    j1ActionPress[Joystick::BUTTON_1]          = UP;
    j1ActionPress[Joystick::BUTTON_2]          = "";
    j1ActionPress[Joystick::BUTTON_3]          = DOWN;
    j1ActionPress[Joystick::BUTTON_4]          = LIGHT_ON;
    j1ActionPress[Joystick::BUTTON_ANALOG_1]   = UP;
    j1ActionPress[Joystick::BUTTON_ANALOG_2]   = "";
    j1ActionPress[Joystick::BUTTON_ANALOG_3]   = DOWN;
    j1ActionPress[Joystick::BUTTON_ANALOG_4]   = LIGHT_ON;
    j1ActionPress[Joystick::BUTTON_R1]         = TURN_RIGHT;
    j1ActionPress[Joystick::BUTTON_R2]         = VERTICAL_MOTORS_SPEED_UP;
    j1ActionPress[Joystick::BUTTON_L1]         = TURN_LEFT;
    j1ActionPress[Joystick::BUTTON_L2]         = VERTICAL_MOTORS_SPEED_DOWN;
    j1ActionPress[Joystick::BUTTON_START]      = "";
    j1ActionPress[Joystick::BUTTON_SELECT]     = "";
}

void MainWindow::initializeJ1ActionRelease() {
    j1ActionRelease[Joystick::BUTTON_UP_OFF]     = STOP_HORIZONTAL;
    j1ActionRelease[Joystick::BUTTON_DOWN_OFF]   = STOP_HORIZONTAL;
    j1ActionRelease[Joystick::BUTTON_RIGHT_OFF]  = STOP_HORIZONTAL;
    j1ActionRelease[Joystick::BUTTON_LEFT_OFF]   = STOP_HORIZONTAL;
    j1ActionRelease[Joystick::BUTTON_UP_ON]      = STOP_HORIZONTAL;
    j1ActionRelease[Joystick::BUTTON_DOWN_ON]    = STOP_HORIZONTAL;
    j1ActionRelease[Joystick::BUTTON_RIGHT_ON]   = STOP_HORIZONTAL;
    j1ActionRelease[Joystick::BUTTON_LEFT_ON]    = STOP_HORIZONTAL;
    j1ActionRelease[Joystick::BUTTON_1]          = STOP_VERTICAL;
    j1ActionRelease[Joystick::BUTTON_2]          = "";
    j1ActionRelease[Joystick::BUTTON_3]          = STOP_VERTICAL;
    j1ActionRelease[Joystick::BUTTON_4]          = LIGHT_OFF;
    j1ActionRelease[Joystick::BUTTON_ANALOG_1]   = STOP_VERTICAL;
    j1ActionRelease[Joystick::BUTTON_ANALOG_2]   = "";
    j1ActionRelease[Joystick::BUTTON_ANALOG_3]   = STOP_VERTICAL;
    j1ActionRelease[Joystick::BUTTON_ANALOG_4]   = LIGHT_OFF;
    j1ActionRelease[Joystick::BUTTON_R1]         = STOP_HORIZONTAL;
    j1ActionRelease[Joystick::BUTTON_R2]         = "";
    j1ActionRelease[Joystick::BUTTON_L1]         = STOP_HORIZONTAL;
    j1ActionRelease[Joystick::BUTTON_L2]         = "";
    j1ActionRelease[Joystick::BUTTON_START]      = "";
    j1ActionRelease[Joystick::BUTTON_SELECT]     = "";
}

void MainWindow::initializeJ2ActionPress() {
    j2ActionPress[Joystick::BUTTON_UP_OFF]     = ELBOW_LEFT;
    j2ActionPress[Joystick::BUTTON_DOWN_OFF]   = ELBOW_RIGHT;
    j2ActionPress[Joystick::BUTTON_RIGHT_OFF]  = CAM1_RIGHT;
    j2ActionPress[Joystick::BUTTON_LEFT_OFF]   = CAM1_LEFT;
    j2ActionPress[Joystick::BUTTON_UP_ON]      = ELBOW_LEFT;
    j2ActionPress[Joystick::BUTTON_DOWN_ON]    = ELBOW_RIGHT;
    j2ActionPress[Joystick::BUTTON_RIGHT_ON]   = CAM1_RIGHT;
    j2ActionPress[Joystick::BUTTON_LEFT_ON]    = CAM1_LEFT;
    j2ActionPress[Joystick::BUTTON_1]          = SHOULDER_LEFT;
    j2ActionPress[Joystick::BUTTON_2]          = GRIPPER_LEFT;
    j2ActionPress[Joystick::BUTTON_3]          = SHOULDER_RIGHT;
    j2ActionPress[Joystick::BUTTON_4]          = GRIPPER_RIGHT;
    j2ActionPress[Joystick::BUTTON_ANALOG_1]   = SHOULDER_LEFT;
    j2ActionPress[Joystick::BUTTON_ANALOG_2]   = GRIPPER_LEFT;
    j2ActionPress[Joystick::BUTTON_ANALOG_3]   = SHOULDER_RIGHT;
    j2ActionPress[Joystick::BUTTON_ANALOG_4]   = GRIPPER_RIGHT;
    j2ActionPress[Joystick::BUTTON_R1]         = WRIST_LEFT;
    j2ActionPress[Joystick::BUTTON_R2]         = WRIST_RIGHT;
    j2ActionPress[Joystick::BUTTON_L1]         = ARM_SPEED_UP;
    j2ActionPress[Joystick::BUTTON_L2]         = ARM_SPEED_DOWN;
    j2ActionPress[Joystick::BUTTON_START]      = CAM2_RIGHT;
    j2ActionPress[Joystick::BUTTON_SELECT]     = CAM2_LEFT;
}

void MainWindow::initializeJ2ActionRelease() {
    j2ActionRelease[Joystick::BUTTON_UP_OFF]     = ELBOW_STOP;
    j2ActionRelease[Joystick::BUTTON_DOWN_OFF]   = ELBOW_STOP;
    j2ActionRelease[Joystick::BUTTON_RIGHT_OFF]  = CAM1_STOP;
    j2ActionRelease[Joystick::BUTTON_LEFT_OFF]   = CAM1_STOP;
    j2ActionRelease[Joystick::BUTTON_UP_ON]      = ELBOW_STOP;
    j2ActionRelease[Joystick::BUTTON_DOWN_ON]    = ELBOW_STOP;
    j2ActionRelease[Joystick::BUTTON_RIGHT_ON]   = CAM1_STOP;
    j2ActionRelease[Joystick::BUTTON_LEFT_ON]    = CAM1_STOP;
    j2ActionRelease[Joystick::BUTTON_1]          = SHOULDER_STOP;
    j2ActionRelease[Joystick::BUTTON_2]          = GRIPPER_STOP;
    j2ActionRelease[Joystick::BUTTON_3]          = SHOULDER_STOP;
    j2ActionRelease[Joystick::BUTTON_4]          = GRIPPER_STOP;
    j2ActionRelease[Joystick::BUTTON_ANALOG_1]   = SHOULDER_STOP;
    j2ActionRelease[Joystick::BUTTON_ANALOG_2]   = GRIPPER_STOP;
    j2ActionRelease[Joystick::BUTTON_ANALOG_3]   = SHOULDER_STOP;
    j2ActionRelease[Joystick::BUTTON_ANALOG_4]   = GRIPPER_STOP;
    j2ActionRelease[Joystick::BUTTON_R1]         = WRIST_STOP;
    j2ActionRelease[Joystick::BUTTON_R2]         = WRIST_STOP;
    j2ActionRelease[Joystick::BUTTON_L1]         = "";
    j2ActionRelease[Joystick::BUTTON_L2]         = "";
    j2ActionRelease[Joystick::BUTTON_START]      = CAM2_STOP;
    j2ActionRelease[Joystick::BUTTON_SELECT]     = CAM2_STOP;
}

void MainWindow::initMissionsList() {
    QString fileName = QDir::currentPath() + "/Missions/";
    missionsList[0] = new Mission("Demo #1", "Maneuvering through the hole",
                                  "Maneuvering through a 75cm x 75cm hole in the ice.",
                                  new QImage(fileName + "1.jpeg"), 50);
    qDebug() << fileName;
}

void MainWindow::updateMission() {
    ui->valueMissionName->setText(currentMission->getName());
    ui->valueMissionDescription->setText(currentMission->getDescription());
    ui->imageMission->setImage(currentMission->getImage());

    int t = currentMission->getTime();
    QString time = "";

    if (t / 60 == 0)
        time += "00";
    else
        time += QString::number(t / 60);
    time += ":";
    if (t % 60 == 0)
        time += "00";
    else
        time += QString::number(t % 60);
    ui->valueMissionTime->setText(time);
}
