#include "joystick.h"

joystick::joystick(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::joystick)
{
    ui->setupUi(this);
}

joystick::~joystick()
{
}
