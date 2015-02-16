/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QLineEdit *lineEditCOMNumber;
    QLabel *label;
    QPushButton *buttonConnect;
    QPushButton *buttonDisconnect;
    QLabel *labelConnectionStatus;
    QLabel *label_2;
    QLabel *labelSentCommand;
    QMenuBar *menuBar;
    QMenu *menuJunior_ROVers;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(178, 241);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 20, 151, 131));
        lineEditCOMNumber = new QLineEdit(groupBox);
        lineEditCOMNumber->setObjectName(QStringLiteral("lineEditCOMNumber"));
        lineEditCOMNumber->setGeometry(QRect(20, 40, 113, 20));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 20, 61, 16));
        buttonConnect = new QPushButton(groupBox);
        buttonConnect->setObjectName(QStringLiteral("buttonConnect"));
        buttonConnect->setGeometry(QRect(10, 100, 61, 23));
        buttonDisconnect = new QPushButton(groupBox);
        buttonDisconnect->setObjectName(QStringLiteral("buttonDisconnect"));
        buttonDisconnect->setGeometry(QRect(80, 100, 61, 23));
        labelConnectionStatus = new QLabel(groupBox);
        labelConnectionStatus->setObjectName(QStringLiteral("labelConnectionStatus"));
        labelConnectionStatus->setGeometry(QRect(40, 70, 71, 16));
        labelConnectionStatus->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0)"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(50, 160, 71, 16));
        labelSentCommand = new QLabel(centralWidget);
        labelSentCommand->setObjectName(QStringLiteral("labelSentCommand"));
        labelSentCommand->setGeometry(QRect(70, 180, 31, 16));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 178, 21));
        menuJunior_ROVers = new QMenu(menuBar);
        menuJunior_ROVers->setObjectName(QStringLiteral("menuJunior_ROVers"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuJunior_ROVers->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Connection", 0));
        lineEditCOMNumber->setText(QApplication::translate("MainWindow", "COM 5", 0));
        label->setText(QApplication::translate("MainWindow", "COM number", 0));
        buttonConnect->setText(QApplication::translate("MainWindow", "Connect", 0));
        buttonDisconnect->setText(QApplication::translate("MainWindow", "Disconnect", 0));
        labelConnectionStatus->setText(QApplication::translate("MainWindow", "Not Connected", 0));
        label_2->setText(QApplication::translate("MainWindow", "Sent Command", 0));
        labelSentCommand->setText(QApplication::translate("MainWindow", "None", 0));
        menuJunior_ROVers->setTitle(QApplication::translate("MainWindow", "Junior ROVers", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
