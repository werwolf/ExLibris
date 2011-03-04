#-------------------------------------------------
#
# Project created by QtCreator 2011-02-13T22:55:59
#
#-------------------------------------------------

QT       += core gui sql
CONFIG += qt warn_on debug

TARGET = exlibris
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    loginform.cpp \
#    euser.cpp \
#    eclient.cpp \
#    esupplier.cpp \
#    eauthor.cpp \
    edbconnection.cpp

HEADERS  += mainwindow.h \
    loginform.h \
#    euser.h \
#    eclient.h \
#    esupplier.h \
#    eauthor.h \
    edbconnection.h

FORMS    += mainwindow.ui \
    loginform.ui

RESOURCES += \
    exlibris.qrc
