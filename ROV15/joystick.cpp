#include "joystick.h"

#include <QDebug>

Joystick::Joystick(int joystickNumber, QObject *parent) :
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
        qDebug() << "Connected";
    }
    if (!sf::Joystick::isConnected(joystickNumber) && isConnected) {
        isConnected = false;
        emit disconnected();
        qDebug() << "Disconnected";
    }
}

void Joystick::readJoystickState() {

}
