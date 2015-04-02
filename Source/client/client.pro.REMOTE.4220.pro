#-------------------------------------------------
#
# Project created by QtCreator 2015-02-18T19:10:51
#
#-------------------------------------------------

QT       += core gui \
            multimedia \
            network \
            opengl
QT += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = client
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
        processmic.cpp \
    audiothread.cpp \
    audioreceive.cpp \
    settings.cpp

HEADERS += mainwindow.h \
        processmic.h \
    audiothread.h \
    audioreceive.h \
    settings.h

FORMS    += mainwindow.ui \
    settings.ui

INSTALLS += install_it


LIBS     += -L ../lib -lvlc-qt -lvlc-qt-widgets
INCLUDEPATH += ../include

RESOURCES += \
    client.qrc
