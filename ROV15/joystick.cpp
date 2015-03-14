#include "joystick.h"

Joystick::Joystick(unsigned int joystickNumber, QObject *parent) :
    QObject(parent),
    joystickNumber(joystickNumber),
    isConnected(false)
{
    t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(readJoystickState()));

    if (joystickNumber != Joystick::JOYSTICK2) {
        connect(t, SIGNAL(timeout()), this, SLOT(checkConnectivity()));
    }

    t->start(30);

    for (int i = 0; i < BUTTON_COUNT; i++)
        buttonStates[i] = false;
}

Joystick::~Joystick()
{
}

void Joystick::checkConnectivity() {
    sf::Joystick::update();

    //Emit connectivity signals
    if (sf::Joystick::isConnected(joystickNumber) && !isConnected) {
        isConnected = true;
        emit connected();
    }
    if (!sf::Joystick::isConnected(joystickNumber) && isConnected) {
        isConnected = false;
        emit disconnected();
    }
}

void Joystick::readJoystickState() {
    sf::Joystick::update();

    x = sf::Joystick::getAxisPosition(joystickNumber, sf::Joystick::X);
    y = sf::Joystick::getAxisPosition(joystickNumber, sf::Joystick::Y);
    z = sf::Joystick::getAxisPosition(joystickNumber, sf::Joystick::Z);
    r = sf::Joystick::getAxisPosition(joystickNumber, sf::Joystick::R);

    povX = sf::Joystick::getAxisPosition(joystickNumber, sf::Joystick::PovX);
    povY = sf::Joystick::getAxisPosition(joystickNumber, sf::Joystick::PovY);

    if (sf::Joystick::isConnected(joystickNumber)) {
        for (int i = 0; i < BUTTON_COUNT; i++) {
            if (isButtonPressed(i) && !buttonStates[i]) {
                buttonStates[i] = true;
                emit buttonPressed(i);
            }

            if (!isButtonPressed(i) && buttonStates[i]) {
                buttonStates[i] = false;
                emit buttonReleased(i);
            }
        }
    }
}

bool Joystick::isButtonPressed(int id) {
    switch(id) {
        case BUTTON_LEFT_OFF:     return x == -100;    break;
        case BUTTON_RIGHT_OFF:    return x == 100;     break;
        case BUTTON_UP_OFF:       return y == -100;    break;
        case BUTTON_DOWN_OFF:     return y == 100;     break;
        case BUTTON_LEFT_ON:      return povX == -100; break;
        case BUTTON_RIGHT_ON:     return povX == 100;  break;
        case BUTTON_UP_ON:        return povY == 100; break;
        case BUTTON_DOWN_ON:      return povY == -100;  break;
        case BUTTON_ANALOG_1:     return z == -100;    break;
        case BUTTON_ANALOG_2:     return r == 100;     break;
        case BUTTON_ANALOG_3:     return z == 100;     break;
        case BUTTON_ANALOG_4:     return r == -100;    break;
        default:                  return sf::Joystick::isButtonPressed(joystickNumber, id);
    }
}
