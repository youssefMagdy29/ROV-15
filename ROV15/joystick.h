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
        BUTTON_UP_OFF       = 32,
        BUTTON_DOWN_OFF     = 33,
        BUTTON_RIGHT_OFF    = 34,
        BUTTON_LEFT_OFF     = 35,
        BUTTON_UP_ON        = 36,
        BUTTON_DOWN_ON      = 37,
        BUTTON_RIGHT_ON     = 38,
        BUTTON_LEFT_ON      = 39,
        BUTTON_ANALOG_1     = 40,
        BUTTON_ANALOG_2     = 41,
        BUTTON_ANALOG_3     = 42,
        BUTTON_ANALOG_4     = 43,
        BUTTON_1            = 0,
        BUTTON_2            = 1,
        BUTTON_3            = 2,
        BUTTON_4            = 3,
        BUTTON_R1           = 5,
        BUTTON_R2           = 7,
        BUTTON_L1           = 4,
        BUTTON_L2           = 6,
        BUTTON_START        = 9,
        BUTTON_SELECT       = 8,
        BUTTON_COUNT        = 44
    } BUTTON;

private:
    int joystickNumber;
    bool isConnected;
    bool buttonStates[BUTTON_COUNT];
    QTimer *t;
    float x, y, z, r;
    float povX, povY;

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
