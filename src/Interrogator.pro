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
    binder/binderModel.cpp \
    binder/binderItem.cpp \
    interrogator.cpp \
    xml.cpp \
    pdf.cpp \
    settingsmanager.cpp \
    toolsdialog.cpp \
    binder/bindertools.cpp \
    binder/bindereditiondialog.cpp \
    bindertreewidget.cpp

HEADERS  += binder/binderItem.h \
    binder/bindermodel.h \
    interrogator.h \
    xml.h \
    pdf.h \
    settingsmanager.h \
    toolsdialog.h \
    binder/bindertools.h \
    binder/bindereditiondialog.h \
    tools/constants.h \
    bindertreewidget.h

FORMS    += interrogator.ui \
    toolsdialog.ui \
    binder/bindereditiondialog.ui

RESOURCES += \
    images.qrc

DISTFILES +=

RC_ICONS = images/interrogator.ico
