#-------------------------------------------------
#
# Project created by QtCreator 2018-04-10T22:07:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nyc
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


SOURCES += \
        src/main.cpp \
        src/mainwindow.cpp \
    src/projectitem.cpp \
    src/projecttree.cpp \
    src/syntaxlisp.cpp \
    src/editor/editor.cpp \
    src/editor/extensionbar.cpp \
    src/controller.cpp \
    src/nyquist.cpp \
    src/sourcefile.cpp

HEADERS += \
        src/mainwindow.h \
    src/projectitem.h \
    src/projecttree.h \
    src/syntaxlisp.h \
    src/editor/editor.h \
    src/editor/extensionbar.h \
    src/controller.h \
    src/action.h \
    src/nyquist.h \
    src/sourcefile.h

FORMS += \
        src/mainwindow.ui

RESOURCES += \
    fonts.qrc
