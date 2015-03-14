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

    for (int i = 0; i < 36; i++)
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

    if (sf::Joystick::isConnected(joystickNumber)) {
        for (int i = 0; i < 36; i++) {
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
    if (id == Joystick::BUTTON_LEFT)
        return x == -100;

    else if (id == Joystick::BUTTON_RIGHT)
        return x == 100;

    else if (id == Joystick::BUTTON_UP)
        return y == -100;

    else if (id == Joystick::BUTTON_DOWN)
        return y == 100;

    else
        return sf::Joystick::isButtonPressed(joystickNumber, id);
}
