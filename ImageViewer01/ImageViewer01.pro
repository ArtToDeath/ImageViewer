#-------------------------------------------------
#
# Project created by QtCreator 2017-05-26T23:11:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageViewer01
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    corrosiondialog.cpp \
    expansiondialog.cpp \
    getbilateraldialog.cpp \
    getcdialog.cpp \
    getgaussparadialog.cpp \
    getkerneldialog.cpp \
    imgprocessor.cpp \
    medianfilterdialog.cpp \
    mymouseevent.cpp \
    settingsdialog.cpp \
    fft.cpp \
    transmatrixdialog.cpp \
    gaussfreqfiltersdialog.cpp

HEADERS  += mainwindow.h \
    corrosiondialog.h \
    expansiondialog.h \
    getbilateraldialog.h \
    getcdialog.h \
    getgaussparadialog.h \
    getkerneldialog.h \
    imgprocessor.h \
    medianfilterdialog.h \
    mymouseevent.h \
    settingsdialog.h \
    fft.h \
    matrix.h \
    transmatrixdialog.h \
    gaussfreqfiltersdialog.h

FORMS    += mainwindow.ui \
    corrosiondialog.ui \
    expansiondialog.ui \
    getbilateraldialog.ui \
    getcdialog.ui \
    getgaussparadialog.ui \
    getkerneldialog.ui \
    medianfilterdialog.ui \
    settingsdialog.ui \
    transmatrixdialog.ui \
    gaussfreqfiltersdialog.ui
