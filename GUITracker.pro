#-------------------------------------------------
#
# Project created by QtCreator 2015-02-15T15:00:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUITracker
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    guiutils.cpp \
    templatematcher.cpp

HEADERS  += mainwindow.h \
    guiutils.h \
    templatematcher.h

LIBS += -L/usr/local/lib \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgcodecs \
    -lopencv_imgproc \
    -lopencv_video \
    -lopencv_videoio \
    -lopencv_text

INCLUDEPATH += /usr/local/include

CONFIG   -= x86_64

INCLUDEPATH += /usr/local/include/opencv2

DEPENDPATH += /usr/local/lib

DEPENDPATH += /usr/local/include/opencv

QMAKE_CXXFLAGS+=-std=c++11

QMAKE_CXXFLAGS+=-stdlib=libc++

QMAKE_LFLAGS+=-std=c++11

QMAKE_LFLAGS+=-stdlib=libc++

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
