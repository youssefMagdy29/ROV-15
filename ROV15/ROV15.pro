#-------------------------------------------------
#
# Project created by QtCreator 2015-02-16T12:21:59
#
#-------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport
} else {
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

QT       += core gui
QT       += multimedia
QT       += multimediawidgets

TARGET = ROV15
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
