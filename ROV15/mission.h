#ifndef MISSION_H
#define MISSION_H

#include <QString>
#include <QImage>

class Mission
{
public:
    Mission(QString name, QString description, QImage *image);
    ~Mission();
    QString getName();
    QString getDescription();
    QImage  getImage();

private:
    QString name;
    QString description;
    QImage *image;
};

#endif // MISSION_H
