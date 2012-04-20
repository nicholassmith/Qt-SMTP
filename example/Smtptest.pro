#-------------------------------------------------
#
# Project created by QtCreator 2011-09-20T14:50:12
#
#-------------------------------------------------

QT       += core gui network

TARGET = Smtptest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../smtp.cpp

HEADERS  += mainwindow.h \
    ../smtp.h

FORMS    += mainwindow.ui

INCLUDEPATH += ..
