#-------------------------------------------------
#
# Project created by QtCreator 2013-09-04T21:11:50
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Interrogator
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        interrogator.cpp \
    xml.cpp \
    pdf.cpp \
    settingsmanager.cpp \
    toolsdialog.cpp

HEADERS  += interrogator.h \
    xml.h \
    pdf.h \
    settingsmanager.h \
    toolsdialog.h

FORMS    += interrogator.ui \
    toolsdialog.ui

RESOURCES += \
    images.qrc

DISTFILES +=

RC_ICONS = images/interrogator.ico
