#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QObject>
#include <QTimer>

#include <SFML/Window.hpp>

class Joystick : public QObject
{
    Q_OBJECT

public:
    explicit Joystick(int joystickNumber, QObject *parent = 0);
    ~Joystick();

    static enum JOYSTICK {
        JOYSTICK1 = 0,
        JOYSTICK2 = 1
    } JOYSTICK;

    static enum BUTTON {
        JOYSTICK_1      = 0,
        JOYSTICK_2      = 1,
        JOYSTICK_3      = 2,
        JOYSTICK_4      = 3,
        JOYSTICK_R1     = 5,
        JOYSTICK_R2     = 7,
        JOYSTICK_L1     = 4,
        JOYSTICK_L2     = 6,
        JOYSTICK_START  = 9,
        JOYSTICK_SELECT = 8
    } BUTTON;

private:
    int joystickNumber;
    bool isConnected;
    bool buttonState[32];
    QTimer *t;

signals:
    void connected();

    void disconnected();

    void buttonPressed();

    void buttonReleased();

private slots:
    void checkConnectivity();

    void readJoystickState();
};

#endif // JOYSTICK_H
