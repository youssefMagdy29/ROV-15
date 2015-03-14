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

    void setupJoystick();
};

#endif // MAINWINDOW_H
