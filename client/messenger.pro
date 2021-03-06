#-------------------------------------------------
#
# Project created by QtCreator 2020-02-22T18:47:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = messenger
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        chat.cpp \
        client.cpp \
        clientinfo.cpp \
        main.cpp \
        message.cpp \
        ui/chatdisplay.cpp \
        ui/chatwidget.cpp \
        ui/clientlabel.cpp \
        ui/clientlist.cpp \
        ui/mainwindow.cpp \
        ui/messagewidget.cpp \
        ui/messenger_window.cpp \
        user.cpp \
        userinfo.cpp

HEADERS += \
        chat.h \
        client.h \
        clientinfo.h \
        message.h \
        ui/chatdisplay.h \
        ui/chatwidget.h \
        ui/clientlabel.h \
        ui/clientlist.h \
        ui/mainwindow.h \
        ui/messagewidget.h \
        ui/messenger_window.h \
        ui/settingsui.h \
        user.h \
        userinfo.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
