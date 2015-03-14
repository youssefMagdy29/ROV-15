#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QObject>
#include <QTimer>

#include <SFML/Window.hpp>

class Joystick : public QObject
{
    Q_OBJECT

public:
    explicit Joystick(unsigned int joystickNumber, QObject *parent = 0);
    ~Joystick();

    static enum JOYSTICK {
        JOYSTICK1 = 0,
        JOYSTICK2 = 1
    } JOYSTICK;

    static enum BUTTON {
        BUTTON_UP     = 32,
        BUTTON_DOWN   = 33,
        BUTTON_RIGHT  = 34,
        BUTTON_LEFT   = 35,
        BUTTON_1      = 0,
        BUTTON_2      = 1,
        BUTTON_3      = 2,
        BUTTON_4      = 3,
        BUTTON_R1     = 5,
        BUTTON_R2     = 7,
        BUTTON_L1     = 4,
        BUTTON_L2     = 6,
        BUTTON_START  = 9,
        BUTTON_SELECT = 8
    } BUTTON;

private:
    int joystickNumber;
    bool isConnected;
    bool buttonStates[36];
    QTimer *t;
    float x, y;

    bool isButtonPressed(int id);

signals:
    void connected();

    void disconnected();

    void buttonPressed(int id);

    void buttonReleased(int id);

private slots:
    void checkConnectivity();

    void readJoystickState();
};

#endif // JOYSTICK_H
