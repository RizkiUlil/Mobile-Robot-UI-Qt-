#-------------------------------------------------
#
# Project created by QtCreator 2017-01-21T04:17:47
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MobileRobot_UI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    glwidget.h

FORMS    += mainwindow.ui

LIBS += -lglut -lGLU -lGL
