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
    settings.cpp \
    playaudio.cpp \
    networking.cpp \
    entercatch.cpp \
    controlthread.cpp \
    customwidget.cpp \
    fft.cpp

HEADERS += mainwindow.h \
        processmic.h \
    audiothread.h \
    audioreceive.h \
    settings.h \
    playaudio.h \
    playaudio.h \
    networking.h \
    entercatch.h \
    controlthread.h \
    customwidget.h \
    fft.h

FORMS    += mainwindow.ui \
    settings.ui

INSTALLS += install_it


#link windows lib
LIBS += -lws2_32
LIBS += -lwinmm

INCLUDEPATH += $$PWD/../include/

#unix|win32: LIBS += -L$$PWD/../libs/ -llibvlc

INCLUDEPATH += $$PWD/../include/vlc
DEPENDPATH += $$PWD/../include/vlc

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../libs/libvlc.lib
win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../libs/libvlccore.lib
#else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../libs/liblibvlc.a

RESOURCES += \
    client.qrc
