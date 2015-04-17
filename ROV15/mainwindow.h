#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "image.h"
#include "resizable_label.h"

#include <QLabel>
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QCamera>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QVideoWidget>
#include <QGraphicsView>
#include <QMap>

#include "joystick.h"

#include "mission.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int KEY_FORWARD;
    int KEY_BACKWARD;
    int KEY_MOVE_RIGHT;
    int KEY_MOVE_LEFT;
    int KEY_TURN_RIGHT;
    int KEY_TURN_LEFT;
    int KEY_UP;
    int KEY_DOWN;
    int KEY_VMOTORS_SPEED_UP;
    int KEY_VMOTORS_SPEED_DOWN;
    int KEY_LIGHT_ON;
    int KEY_LIGHT_OFF;
    int KEY_CAM1_RIGHT;
    int KEY_CAM1_LEFT;
    int KEY_CAM2_RIGHT;
    int KEY_CAM2_LEFT;
    int KEY_GRIPPER_RIGHT;
    int KEY_GRIPPER_LEFT;
    int KEY_WRIST_RIGHT;
    int KEY_WRIST_LEFT;
    int KEY_ELBOW_RIGHT;
    int KEY_ELBOW_LEFT;
    int KEY_SHOULDER_RIGHT;
    int KEY_SHOULDER_LEFT;
    int KEY_BASE_RIGHT;
    int KEY_BASE_LEFT;
    int KEY_ARM_SPEED_UP;
    int KEY_ARM_SPEED_DOWN;

    Mission *currentMission;

private slots:
    //void keyReleaseEvent(QKeyEvent *e);

    void on_buttonConnect_clicked();

    void on_buttonDisconnect_clicked();

    void readData();

    void on_captureButton_clicked();

    void imageSaved(int id, QString str);

    //void keyPressEvent(QKeyEvent *e);

    void joystickConnected();

    void joystickDisconnected();

    void joystick1ButtonPressed(int id);

    void joystick2ButtonPressed(int id);

    void joystick1ButtonReleased(int id);

    void joystick2ButtonReleased(int id);

    void on_sendButton_clicked();

private:
    Ui::MainWindow *ui;
    QLabel *l;
    QSerialPort *serial;
    QCamera *camera;
    QVideoWidget *viewfinder;
    QCameraImageCapture *imageCapture;
    QGraphicsView *graphicsView;
    Image *image;
    QImage img;
    QCameraInfo camInfo;
    QByteArray keyboardActionPress[255];
    QByteArray keyboardActionRelease[255];
    Joystick *j1, *j2;
    QByteArray j1ActionPress[Joystick::BUTTON_COUNT], j1ActionRelease[Joystick::BUTTON_COUNT];
    QByteArray j2ActionPress[Joystick::BUTTON_COUNT], j2ActionRelease[Joystick::BUTTON_COUNT];
    bool mode;
    bool toggleJ1[Joystick::BUTTON_COUNT];
    bool toggleJ2[Joystick::BUTTON_COUNT];
    double xAcc, yAcc, zAcc;
    double xVel, yVel, zVel;
    double xDst, yDst, zDst;
    double SAMPLE_TIME;
    Mission *missionsList[30];

    void setupSerialConnection();

    void setupCamera();

    void initializeKActionPress();
    void initializeKActionRelease();

    void setupJoystick();
    void initializeJ1ButtonKinds();
    void initializeJ1ActionPress();
    void initializeJ2ActionPress();
    void initializeJ1ActionRelease();
    void initializeJ2ActionRelease();

    void initMissionsList();

    void updateMission();

    void initKeys() {
        KEY_FORWARD            = 'W';
        KEY_BACKWARD           = 'S';
        KEY_MOVE_RIGHT         = 'D';
        KEY_MOVE_LEFT          = 'A';
        KEY_TURN_RIGHT         = 'L';
        KEY_TURN_LEFT          = 'K';
        KEY_UP                 = 'I';
        KEY_DOWN               = 'J';
        KEY_VMOTORS_SPEED_UP   = 'G';
        KEY_VMOTORS_SPEED_DOWN = 'H';
        KEY_LIGHT_ON           = 'N';
        KEY_LIGHT_OFF          = 'M';
        KEY_CAM1_RIGHT         = 'Y';
        KEY_CAM1_LEFT          = 'T';
        KEY_CAM2_RIGHT         = '/';
        KEY_CAM2_LEFT          = '*';
        KEY_GRIPPER_RIGHT      = '.';
        KEY_GRIPPER_LEFT       = '0';
        KEY_WRIST_RIGHT        = '9';
        KEY_WRIST_LEFT         = '3';
        KEY_ELBOW_RIGHT        = '7';
        KEY_ELBOW_LEFT         = '1';
        KEY_SHOULDER_RIGHT     = '8';
        KEY_SHOULDER_LEFT      = '2';
        KEY_BASE_RIGHT         = '6';
        KEY_BASE_LEFT          = '4';
        KEY_ARM_SPEED_UP       = '+';
        KEY_ARM_SPEED_DOWN     = '-';
    }
};

#endif // MAINWINDOW_H
