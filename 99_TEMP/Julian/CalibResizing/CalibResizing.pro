#------------------------------
#
# Project created by QtCreator
#
#------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CalibResizing
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

## SET THE CORRECT PATH HEREAFTER ##



INCLUDEPATH += \
    "C:/OpenCV/build/include" \
 #   "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/include"

LIBS += \
 #   "-LC:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/lib/amd64" \
    "-LC:/OpenCV/build/x86/vc10/lib" \
    #if library name is lib[name].lib, you should write -l[name]
    -lopencv_core247 \
    -lopencv_highgui247 \
    -lopencv_imgproc247 \
    -lopencv_calib3d247 \
    -lopencv_features2d247 \
    -lopencv_contrib247 \
    -lopencv_flann247 \
    -lopencv_gpu247 \
    -lopencv_legacy247 \
    -lopencv_ml247 \
    -lopencv_objdetect247 \
    -lopencv_ts247 \
    -lopencv_video247

DESTDIR = ./dist
CONFIG += c++11
