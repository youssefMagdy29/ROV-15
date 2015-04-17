#include "mission.h"

Mission::Mission(QString demo, QString name, QString description, QImage *image, int time) :
    demo(demo),
    name(name),
    description(description),
    image(image),
    time(time)
{
}

QString Mission::getDemo() {
    return demo;
}

QString Mission::getName() {
    return name;
}

QString Mission::getDescription() {
    return description;
}

QImage *Mission::getImage() {
    return image;
}

int Mission::getTime() {
    return time;
}


