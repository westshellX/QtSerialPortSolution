#-------------------------------------------------
#
# Project created by QtCreator 2018-01-30T11:40:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SerialPortAssistant
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

RC_FILE  += app.rc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../Qt5.7.0/5.7/msvc2015_64/lib/ -lQt5SerialPort
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../Qt5.7.0/5.7/msvc2015_64/lib/ -lQt5SerialPortd
else:unix: LIBS += -L$$PWD/../../../Qt5.7.0/5.7/msvc2015_64/lib/ -lQt5SerialPort

INCLUDEPATH += $$PWD/../../../Qt5.7.0/5.7/msvc2015_64/include
DEPENDPATH += $$PWD/../../../Qt5.7.0/5.7/msvc2015_64/include
