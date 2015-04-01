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

install_it.path = %{buildDir}
install_it.files += %{sourceDir}/../dlls/libvlc.dll
install_it.files += %{sourceDir}/../dlls/libvlccore.dll
install_it.files += %{sourceDir}/../dlls/plugins/

TARGET = client
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
        processmic.cpp \
    audiothread.cpp \
    audioreceive.cpp

HEADERS += mainwindow.h \
        processmic.h \
    audiothread.h \
    audioreceive.h

FORMS    += mainwindow.ui

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
