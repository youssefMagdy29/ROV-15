#ifndef MISSION_H
#define MISSION_H

#include <QString>
#include <QImage>

class Mission
{
public:
    Mission(QString demo, QString name, QString description, QImage *image, int time);
    ~Mission();
    QString getDemo();
    QString getName();
    QString getDescription();
    QImage  *getImage();
    int getTime();

private:
    QString demo;
    QString name;
    QString description;
    QImage *image;
    int time;
};

#endif // MISSION_H
