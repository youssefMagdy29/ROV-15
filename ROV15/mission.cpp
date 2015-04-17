#include "mission.h"

Mission::Mission(QString name, QString description, QImage *image, int time) :
    name(name),
    description(description),
    image(image),
    time(time)
{
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


