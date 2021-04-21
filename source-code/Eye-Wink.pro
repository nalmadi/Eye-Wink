#-------------------------------------------------
#
# Project created by QtCreator 2017-05-05T22:14:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Eye-Wink
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    trialview.cpp \
    graphicsview.cpp \
    experiment.cpp \
    trial.cpp \
    ievents.cpp \
    aoi.cpp

HEADERS  += mainwindow.h \
    trialview.h \
    graphicsview.h \
    experiment.h \
    trial.h \
    ievents.h \
    aoi.h

FORMS    += mainwindow.ui
