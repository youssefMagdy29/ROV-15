#include "joystick.h"

Joystick::Joystick(unsigned int joystickNumber, QObject *parent) :
    QObject(parent),
    joystickNumber(joystickNumber),
    isConnected(false),
    t(new QTimer(this)),
    checkAutoRepeat(new QTimer(this)),
    autoRepeat(new QTimer(this)),
    currentButton(0)
{
    connect(t, SIGNAL(timeout()), this, SLOT(readJoystickState()));
    connect(checkAutoRepeat, SIGNAL(timeout()), this, SLOT(sCheckAutoRepeat()));
    connect(autoRepeat, SIGNAL(timeout()), this, SLOT(sAutoRepeat()));

    if (joystickNumber != Joystick::JOYSTICK2) {
        connect(t, SIGNAL(timeout()), this, SLOT(checkConnectivity()));
    }

    t->start(30);

    for (int i = 0; i < BUTTON_COUNT; i++)
        buttonStates[i] = false;

    for (int i = 0; i < BUTTON_COUNT; i++)
        buttonKinds[i] = 0;
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
                if (checkAutoRepeat->isActive())
                    checkAutoRepeat->stop();

                if (autoRepeat->isActive())
                    autoRepeat->stop();

                if (getKind(i) == AUTO_REPEAT) {
                    checkAutoRepeat->start(500);

                    currentButton = i;
                }
                buttonStates[i] = true;
                emit buttonPressed(i);
            }

            if (!isButtonPressed(i) && buttonStates[i]) {
                if (checkAutoRepeat->isActive())
                    checkAutoRepeat->stop();

                if (autoRepeat->isActive())
                    autoRepeat->stop();

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

void Joystick::setKind(int id, int kind) {
    buttonKinds[id] = kind;
}

int Joystick::getKind(int id) {
    return buttonKinds[id];
}

void Joystick::sCheckAutoRepeat() {
    checkAutoRepeat->stop();
    autoRepeat->start(100);
}

void Joystick::sAutoRepeat() {
    emit buttonPressed(currentButton);
}
