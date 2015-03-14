#ifndef JOYSTICK_H
#define JOYSTICK_H

class Joystick
{
    Q_OBJECT

public:
    explicit Joystick(int joystick_number);
    ~Joystick();

    static enum JOYSTICK{
        JOYSTICK_1 = 0,
        JOYSTICK_2 = 1
    } JOYSTICK;

private:

};

#endif // JOYSTICK_H
