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

#include <QTimer>

#include "image.h"

#include <SFML/Window.hpp>

QSerialPort *serial;
QLabel *l;
QCamera *camera;
QVideoWidget *viewfinder;
QCameraImageCapture *imageCapture;
QGraphicsView *graphicsView;
QLabel *lbl;
Image *image;
QImage img;

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

//Joystick buttons
unsigned int const JOYSTICK_1      = 0;
unsigned int const JOYSTICK_2      = 1;
unsigned int const JOYSTICK_3      = 2;
unsigned int const JOYSTICK_4      = 3;
unsigned int const JOYSTICK_R1     = 5;
unsigned int const JOYSTICK_R2     = 7;
unsigned int const JOYSTICK_L1     = 4;
unsigned int const JOYSTICK_L2     = 6;
unsigned int const JOYSTICK_START  = 9;
unsigned int const JOYSTICK_SELECT = 8;

//Pilot Control
//Press actions
QByteArray const ACTION_PRESS_JOYSTICK1_UP     = FORWARD;
QByteArray const ACTION_PRESS_JOYSTICK1_DOWN   = BACKWARD;
QByteArray const ACTION_PRESS_JOYSTICK1_RIGHT  = MOVE_RIGHT;
QByteArray const ACTION_PRESS_JOYSTICK1_LEFT   = MOVE_LEFT;
QByteArray const ACTION_PRESS_JOYSTICK1_1      = UP;
QByteArray const ACTION_PRESS_JOYSTICK1_2      = "";
QByteArray const ACTION_PRESS_JOYSTICK1_3      = DOWN;
QByteArray const ACTION_PRESS_JOYSTICK1_4      = LIGHT_ON;
QByteArray const ACTION_PRESS_JOYSTICK1_R1     = TURN_RIGHT;
QByteArray const ACTION_PRESS_JOYSTICK1_R2     = "";
QByteArray const ACTION_PRESS_JOYSTICK1_L1     = TURN_LEFT;
QByteArray const ACTION_PRESS_JOYSTICK1_L2     = "";
QByteArray const ACTION_PRESS_JOYSTICK1_START  = "";
QByteArray const ACTION_PRESS_JOYSTICK1_SELECT = "";

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

bool x_1_pressed = false;
bool y_1_pressed = false;
bool y_2_pressed = false;
bool z_2_pressed = false;
bool r_2_pressed = false;

bool _1_pressed[32];
bool _2_pressed[32];

bool mode;
bool light_state;
int img_counter;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    l = ui->labelSentCommand;

    ui->buttonDisconnect->setDisabled(true);

    lbl = new QLabel;
    lbl->setWindowTitle("Screen Shot");

    image = new Image(new QImage());

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
    camera->start();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(readJoystickState()));
    timer->start(15);
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
    QByteArray data = serial->readAll();
    ui->label_2->setText(data);
}

void MainWindow::readJoystickState() {

    sf::Joystick::update();

    if (sf::Joystick::isConnected(0)) {
        ui->joystickStatus->setText("Connected");
        ui->joystickStatus->setStyleSheet("color: #00ff00");

        //ROV Control
        float x1 = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
        float y1 = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

        if (!_1_pressed[JOYSTICK_1]) {
            if (sf::Joystick::isButtonPressed(0, JOYSTICK_1)) {
                l->setText(ACTION_PRESS_JOYSTICK1_1);
                serial->write(ACTION_PRESS_JOYSTICK1_1);
                _1_pressed[JOYSTICK_1] = true;
            }
        }
        else if (!sf::Joystick::isButtonPressed(0, JOYSTICK_1)) {
            l->setText(ACTION_RELEASE_JOYSTICK1_1);
            serial->write(ACTION_RELEASE_JOYSTICK1_1);
            _1_pressed[JOYSTICK_1] = false;
        }

        if (!_1_pressed[JOYSTICK_3]) {
            if (sf::Joystick::isButtonPressed(0, JOYSTICK_3)) {
                l->setText(ACTION_PRESS_JOYSTICK1_3);
                serial->write(ACTION_PRESS_JOYSTICK1_3);
                _1_pressed[JOYSTICK_3] = true;
            }
        }
        else if (!sf::Joystick::isButtonPressed(0, JOYSTICK_3)) {
            l->setText(ACTION_RELEASE_JOYSTICK1_3);
            serial->write(ACTION_RELEASE_JOYSTICK1_3);
            _1_pressed[JOYSTICK_3] = false;
        }

        if (!_1_pressed[JOYSTICK_2]) {
            if (sf::Joystick::isButtonPressed(0, JOYSTICK_2)) {
                /*l->setText(ACTION_PRESS_JOYSTICK1_2);
                serial->write(ACTION_PRESS_JOYSTICK1_2);*/
                imageCapture->capture();
                _1_pressed[JOYSTICK_2] = true;
            }
        }
        else if (!sf::Joystick::isButtonPressed(0, JOYSTICK_2)) {
            /*l->setText(ACTION_RELEASE_JOYSTICK1_2);
            serial->write(ACTION_RELEASE_JOYSTICK1_2);*/
            _1_pressed[JOYSTICK_2] = false;
        }

        if (!_1_pressed[JOYSTICK_4]) {
            if (sf::Joystick::isButtonPressed(0, JOYSTICK_4)) {
                light_state = !light_state;
                if (light_state) {
                    l->setText(LIGHT_ON);
                    serial->write(LIGHT_ON);
                }
                else {
                    l->setText(LIGHT_OFF);
                    serial->write(LIGHT_OFF);
                }
                _1_pressed[JOYSTICK_4] = true;
            }
        }
        else if (!sf::Joystick::isButtonPressed(0, JOYSTICK_4)) {
            /*l->setText(ACTION_RELEASE_JOYSTICK1_4);
            serial->write(ACTION_RELEASE_JOYSTICK1_4);*/
            _1_pressed[JOYSTICK_4] = false;
        }

        if (!_1_pressed[JOYSTICK_R1]) {
            if (sf::Joystick::isButtonPressed(0, JOYSTICK_R1)) {
                l->setText(ACTION_PRESS_JOYSTICK1_R1);
                serial->write(ACTION_PRESS_JOYSTICK1_R1);
                _1_pressed[JOYSTICK_R1] = true;
            }
        }
        else if (!sf::Joystick::isButtonPressed(0, JOYSTICK_R1)) {
            l->setText(ACTION_RELEASE_JOYSTICK1_R1);
            serial->write(ACTION_RELEASE_JOYSTICK1_R1);
            _1_pressed[JOYSTICK_R1] = false;
        }

        /*if (!_1_pressed[JOYSTICK_R2]) {
            if (sf::Joystick::isButtonPressed(0, JOYSTICK_R2)) {
                l->setText(ACTION_PRESS_JOYSTICK1_R2);
                serial->write(ACTION_PRESS_JOYSTICK1_R2);
                _1_pressed[JOYSTICK_R2] = true;
            }
        }
        else if (!sf::Joystick::isButtonPressed(0, JOYSTICK_R2)) {
            l->setText(ACTION_RELEASE_JOYSTICK1_R2);
            serial->write(ACTION_RELEASE_JOYSTICK1_R2);
            _1_pressed[JOYSTICK_R2] = false;
        }*/

        if (!_1_pressed[JOYSTICK_L1]) {
            if (sf::Joystick::isButtonPressed(0, JOYSTICK_L1)) {
                l->setText(ACTION_PRESS_JOYSTICK1_L1);
                serial->write(ACTION_PRESS_JOYSTICK1_L1);
                _1_pressed[JOYSTICK_L1] = true;
            }
        }
        else if (!sf::Joystick::isButtonPressed(0, JOYSTICK_L1)) {
            l->setText(ACTION_RELEASE_JOYSTICK1_L1);
            serial->write(ACTION_RELEASE_JOYSTICK1_L1);
            _1_pressed[JOYSTICK_L1] = false;
        }

        /*if (!_1_pressed[JOYSTICK_L2]) {
            if (sf::Joystick::isButtonPressed(0, JOYSTICK_L2)) {
                l->setText(ACTION_PRESS_JOYSTICK1_L2);
                serial->write(ACTION_PRESS_JOYSTICK1_L2);
                _1_pressed[JOYSTICK_L2] = true;
            }
        }
        else if (!sf::Joystick::isButtonPressed(0, JOYSTICK_L2)) {
            l->setText(ACTION_RELEASE_JOYSTICK1_L2);
            serial->write(ACTION_RELEASE_JOYSTICK1_L2);
            _1_pressed[JOYSTICK_L2] = false;
        }*/

        if (!_1_pressed[JOYSTICK_START]) {
            if (sf::Joystick::isButtonPressed(0, JOYSTICK_START)) {
                /*l->setText(ACTION_PRESS_JOYSTICK1_START);
                serial->write(ACTION_PRESS_JOYSTICK1_START);*/
                mode = !mode;
                if (mode)
                    ui->valueMode->setText("Measuring");
                else
                    ui->valueMode->setText("Capturing");
                _1_pressed[JOYSTICK_START] = true;
            }
        }
        else if (!sf::Joystick::isButtonPressed(0, JOYSTICK_START)) {
            /*l->setText(ACTION_RELEASE_JOYSTICK1_START);
            serial->write(ACTION_RELEASE_JOYSTICK1_START);*/
            _1_pressed[JOYSTICK_START] = false;
        }


        if (!x_1_pressed) {
            if (x1 == -100) {
                l->setText(ACTION_PRESS_JOYSTICK1_LEFT);
                serial->write(ACTION_PRESS_JOYSTICK1_LEFT);
                x_1_pressed = true;
            }
            else if (x1 == 100) {
                l->setText(ACTION_PRESS_JOYSTICK1_RIGHT);
                serial->write(ACTION_PRESS_JOYSTICK1_RIGHT);
                x_1_pressed = true;
            }
        }
        else if ((int) x1 == 0) {
            l->setText(ACTION_RELEASE_JOYSTICK1_LEFT);
            serial->write(ACTION_RELEASE_JOYSTICK1_LEFT);
            x_1_pressed = false;
        }

        if (!y_1_pressed) {
            if (y1 == -100) {
                l->setText(ACTION_PRESS_JOYSTICK1_UP);
                serial->write(ACTION_PRESS_JOYSTICK1_UP);
                y_1_pressed = true;
            }
            else if (y1 == 100) {
                l->setText(ACTION_PRESS_JOYSTICK1_DOWN);
                serial->write(ACTION_PRESS_JOYSTICK1_DOWN);
                y_1_pressed = true;
            }
        }
        else if ((int) y1 == 0) {
            l->setText(ACTION_RELEASE_JOYSTICK1_DOWN);
            serial->write(ACTION_RELEASE_JOYSTICK1_DOWN);
            y_1_pressed = false;
        }

        //Arm Control
        float y2 = sf::Joystick::getAxisPosition(1, sf::Joystick::Y);
        float z2 = sf::Joystick::getAxisPosition(1, sf::Joystick::Z);
        float r2 = sf::Joystick::getAxisPosition(1, sf::Joystick::R);

        //Gripper
        if (!_2_pressed[JOYSTICK_2]) {
            if (sf::Joystick::isButtonPressed(1, JOYSTICK_2)) {
                l->setText(GRIPPER_LEFT);
                serial->write(GRIPPER_LEFT);
                _2_pressed[JOYSTICK_2] = true;
            }
        }
        else if (!sf::Joystick::isButtonPressed(1, JOYSTICK_2)) {
            l->setText(GRIPPER_STOP);
            serial->write(GRIPPER_STOP);
            _2_pressed[JOYSTICK_2] = false;
        }
        if (!_2_pressed[JOYSTICK_4]) {
            if (sf::Joystick::isButtonPressed(1, JOYSTICK_4)) {
                l->setText(GRIPPER_RIGHT);
                serial->write(GRIPPER_RIGHT);
                _2_pressed[JOYSTICK_4] = true;
            }
        }
        else if (!sf::Joystick::isButtonPressed(1, JOYSTICK_4)) {
            l->setText(GRIPPER_STOP);
            serial->write(GRIPPER_STOP);
            _2_pressed[JOYSTICK_4] = false;
        }
        //Wrist
        if (!_2_pressed[JOYSTICK_1]) {
            if (sf::Joystick::isButtonPressed(1, JOYSTICK_1)) {
                l->setText(WRIST_LEFT);
                serial->write(WRIST_LEFT);
                _2_pressed[JOYSTICK_1] = true;
            }
        }
        else if (!sf::Joystick::isButtonPressed(1, JOYSTICK_1)) {
            l->setText(WRIST_STOP);
            serial->write(WRIST_STOP);
            _2_pressed[JOYSTICK_1] = false;
        }
        if (!_2_pressed[JOYSTICK_3]) {
            if (sf::Joystick::isButtonPressed(1, JOYSTICK_3)) {
                l->setText(WRIST_RIGHT);
                serial->write(WRIST_RIGHT);
                _2_pressed[JOYSTICK_3] = true;
            }
        }
        else if (!sf::Joystick::isButtonPressed(1, JOYSTICK_3)) {
            l->setText(WRIST_STOP);
            serial->write(WRIST_STOP);
            _2_pressed[JOYSTICK_3] = false;
        }
        //Elbow
        if (!y_2_pressed) {
            if (y2 == -100) {
                l->setText(ELBOW_LEFT);
                serial->write(ELBOW_LEFT);
                y_2_pressed = true;
            }
            else if (y2 == 100) {
                l->setText(ELBOW_RIGHT);
                serial->write(ELBOW_RIGHT);
                y_2_pressed = true;
            }
        }
        else if (abs(y2) < 5) {
            l->setText(ELBOW_STOP);
            serial->write(ELBOW_STOP);
            y_2_pressed = false;
        }
        //Shoulder
        if (!z_2_pressed) {
            if (z2 == -100) {
                l->setText(SHOULDER_LEFT);
                serial->write(SHOULDER_LEFT);
                z_2_pressed = true;
            }
            else if (z2 == 100) {
                l->setText(SHOULDER_RIGHT);
                serial->write(SHOULDER_RIGHT);
                z_2_pressed = true;
            }
        }
        else if (abs(z2) < 5) {
            l->setText(SHOULDER_STOP);
            serial->write(SHOULDER_STOP);
            z_2_pressed = false;
        }
        //Base
        if (!r_2_pressed) {
            if (r2 == -100) {
                l->setText(BASE_LEFT);
                serial->write(BASE_LEFT);
                r_2_pressed = true;
            }
            else if (r2 == 100) {
                l->setText(BASE_RIGHT);
                serial->write(BASE_RIGHT);
                r_2_pressed = true;
            }
        }
        else if (abs(r2) < 5) {
            l->setText(BASE_STOP);
            serial->write(BASE_STOP);
            r_2_pressed = false;
        }
    }
    else {
        ui->joystickStatus->setText("Not Connected");
        ui->joystickStatus->setStyleSheet("color: #ff0000");
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
        lbl->setPixmap(QPixmap::fromImage(img));
        lbl->show();
        QString filename = "C:/Users/Youssef/Desktop/ROV_ScreenShots/"
                + QDate::currentDate().toString(Qt::ISODate) + " " +
                QTime::currentTime().toString("hh:mm:ss.zzz").replace(":", "_").replace(".", "_") + ".jpeg";
        qDebug() << filename;
        img.save(filename, fileformat.constData());
    }
}
