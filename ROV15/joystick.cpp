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

    for (int i = 0; i < 32; i++)
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
    //Emit buttonPressed signal
    for (int i = 0; i < 32; i++) {
        if (sf::Joystick::isButtonPressed(joystickNumber, i)
                && !buttonStates[i]) {
            buttonStates[i] = true;
            emit buttonPressed(i);
        }

        if (!sf::Joystick::isButtonPressed(joystickNumber, i)
                && buttonStates[i]) {
            buttonStates[i] = false;
            emit buttonReleased(i);
        }
    }
}
