#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void keyPressEvent(QKeyEvent *e);

    void on_buttonConnect_clicked();

    void on_buttonDisconnect_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
