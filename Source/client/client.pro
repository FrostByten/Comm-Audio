#-------------------------------------------------
#
# Project created by QtCreator 2015-02-18T19:10:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

install_it.path = %{buildDir}
install_it.files += %{sourceDir}/../dlls/libvlc.dll
install_it.files += %{sourceDir}/../dlls/libvlccore.dll
install_it.files += %{sourceDir}/../dlls/plugins/

TARGET = client
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INSTALLS += install_it

INCLUDEPATH += $$PWD/../include/
