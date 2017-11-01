#-------------------------------------------------
#
# Project created by QtCreator 2015-10-20T09:06:56
#
#-------------------------------------------------

QT       += core gui
LIBS     += -lpthread libwsock32 libws2_32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ModbusSender
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    m_client.cpp \
    send_dialog.cpp \
    addr_value.cpp \
    reset_dialog.cpp \
    select_dialog.cpp \
    copy_dialog.cpp

HEADERS  += mainwindow.h \
    m_client.h \
    send_dialog.h \
    addr_value.h \
    reset_dialog.h \
    select_dialog.h \
    copy_dialog.h



FORMS    += mainwindow.ui \
    send_dialog.ui \
    reset_dialog.ui \
    select_dialog.ui \
    copy_dialog.ui

RESOURCES +=

RC_FILE = icon.rc
