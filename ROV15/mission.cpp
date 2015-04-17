#include "mission.h"

Mission::Mission(QString name, QString description, QImage *image) :
    name(name),
    description(description),
    image(image)
{
}

QString Mission::getName() {
    return name;
}

QString Mission::getDescription() {
    return description;
}

QImage Mission::getImage() {
    return *image;
}


