#ifndef JOYSTICK_H
#define JOYSTICK_H

class Joystick
{
    Q_OBJECT

public:
    explicit Joystick(int joystickNumber);
    ~Joystick();

    static enum JOYSTICK {
        JOYSTICK_1 = 0,
        JOYSTICK_2 = 1
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
    bool buttonState[32];

signals:
    void connected();
    void disconnected();
    void buttonPressed();
    void buttonReleased();
};

#endif // JOYSTICK_H
