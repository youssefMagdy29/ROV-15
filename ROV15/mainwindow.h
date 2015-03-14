#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

#include "joystick.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void keyReleaseEvent(QKeyEvent *e);

    void on_buttonConnect_clicked();

    void on_buttonDisconnect_clicked();

    void readData();

    void on_captureButton_clicked();

    void imageSaved(int id, QString str);

    void keyPressEvent(QKeyEvent *e);

    void joystickConnected();

    void joystickDisconnected();

    void joystick1ButtonPressed(int id);

    void joystick2ButtonPressed(int id);

    void joystick1ButtonReleased(int id);

    void joystick2ButtonReleased(int id);

private:
    Ui::MainWindow *ui;
    Joystick *j1, *j2;
    QByteArray j1ActionPress[36], j2ActionPress[36];
    QByteArray j1ActionRelease[36], j2ActionRelease[36];

    void setupJoystick();
    void initializeJ1ActionPress();
    void initializeJ2ActionPress();
    void initializeJ1ActionRelease();
    void initializeJ2ActionRelease();
};

#endif // MAINWINDOW_H
