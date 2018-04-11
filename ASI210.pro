#-------------------------------------------------
#
# Project created by QtCreator 2018-04-10T12:20:48
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ASI210
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serialcom.cpp

HEADERS  += mainwindow.h \
    serialcom.h

FORMS    += mainwindow.ui

DISTFILES += \
    back.png

RESOURCES += \
    images.qrc
