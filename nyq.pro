#-------------------------------------------------
#
# Project created by QtCreator 2018-04-10T22:07:40
#
#-------------------------------------------------

QT       += core gui sql svg xml

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

win32:RC_ICONS += res/ico/application.ico

SOURCES += \
        src/main.cpp \
        src/mainwindow.cpp \
    src/editor/syntaxlisp.cpp \
    src/editor/extensionbar.cpp \
    src/controller.cpp \
    src/nyquist.cpp \
    src/storage/storagefile.cpp \
    src/styler.cpp \
    src/storage/storage.cpp \
    src/preferences.cpp \
    src/plotter/canvas.cpp \
    src/swiss.cpp \
    src/logger.cpp \
    src/visual/nyactionbutton.cpp \
    src/prompter/nyprompter.cpp \
    src/project/nyprojectitem.cpp \
    src/storage/nyicon.cpp \
    src/project/nyprojectmodel.cpp \
    src/editor/nyeditor.cpp \
    src/nyquist/nyoutputanalyzer.cpp \
    src/nynodedialog.cpp \
    src/status/nystatusarea.cpp \
    src/plotter/nysoundframe.cpp \
    src/plotter/nyplotdata.cpp

HEADERS += \
        src/mainwindow.h \
    src/editor/syntaxlisp.h \
    src/editor/extensionbar.h \
    src/controller.h \
    src/nyquist.h \
    src/storage/storagefile.h \
    src/styler.h \
    src/storage/storage.h \
    src/preferences.h \
    src/plotter/canvas.h \
    src/swiss.h \
    src/storage/labels.h \
    src/logger.h \
    src/visual/nyactionbutton.h \
    src/editor/nyprompter.h \
    src/prompter/nyprompter.h \
    src/project/nyprojectitem.h \
    src/storage/nyicon.h \
    src/project/nyprojectmodel.h \
    src/editor/nyeditor.h \
    src/nyquist/nyoutputanalyzer.h \
    src/nynodedialog.h \
    src/status/nystatusarea.h \
    src/plotter/nysoundframe.h \
    src/plotter/nyplotdata.h

FORMS += \
        src/mainwindow.ui \
    src/preferences.ui \
    src/prompter/nyprompter.ui \
    src/nynodedialog.ui

RESOURCES += \
    resources.qrc
