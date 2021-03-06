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
        mainwindow.cpp \
    image.cpp \
    customlabel.cpp \
    resizable_label.cpp \
    joystick.cpp \
    mission.cpp

HEADERS  += mainwindow.h \
    image.h \
    customlabel.h \
    resizable_label.h \
    joystick.h \
    mission.h

FORMS    += mainwindow.ui \
    image.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/C:/Qt/5.4/msvc2013_64/lib/ -lsfml-window
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/C:/Qt/5.4/msvc2013_64/lib/ -lsfml-windowd
else:unix: LIBS += -L$$PWD/C:/Qt/5.4/msvc2013_64/lib/ -lsfml-window

INCLUDEPATH += $$PWD/C:/Qt/5.4/msvc2013_64/include/SFML
DEPENDPATH += $$PWD/C:/Qt/5.4/msvc2013_64/include/SFML

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/C:/Qt/Qt5.4.1/5.4/mingw491_32/lib/ -lsfml-window
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/C:/Qt/Qt5.4.1/5.4/mingw491_32/lib/ -lsfml-windowd
else:unix: LIBS += -L$$PWD/C:/Qt/Qt5.4.1/5.4/mingw491_32/lib/ -lsfml-window

INCLUDEPATH += $$PWD/C:/Qt/Qt5.4.1/5.4/mingw491_32/include
DEPENDPATH += $$PWD/C:/Qt/Qt5.4.1/5.4/mingw491_32/include
