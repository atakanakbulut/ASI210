#-------------------------------------------------
#
# Project created by QtCreator 2018-04-10T12:20:48
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ASI210
TEMPLATE = app


target.path=/home/root/atakan
INSTALLS+=target

SOURCES += main.cpp\
        mainwindow.cpp \
    serialcom.cpp \
    startup.cpp \
    application.cpp \
    netman.cpp \
    keyboard.cpp\
    converter.cpp

HEADERS  += mainwindow.h \
    serialcom.h \
    startup.h \
    application.h \
    netman.h \
    keyboard.h\
    converter.h

FORMS    += mainwindow.ui \
    keyboard.ui\
    startup.ui

DISTFILES +=

RESOURCES += \
    images.qrc
