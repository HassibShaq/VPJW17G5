#-------------------------------------------------
#
# Project created by QtCreator 2013-12-05T09:25:13
#
#-------------------------------------------------

QT       += core gui widgets network
CONFIG += qt thread

INCLUDEPATH += "C:\OpenCv30\opencv\build\install\include"
LIBS += "-LC:\OpenCv30\opencv\build\install\x86\vc10\lib"


debug:LIBS +=    -lopencv_core300d \
    -lopencv_videoio300d \
    -lopencv_imgcodecs300d \
    -lopencv_highgui300d \
    -lopencv_imgproc300d \
    -lopencv_calib3d300d \
    -lopencv_features2d300d \
    -lopencv_flann300d \
    -lopencv_aruco300d


release:LIBS +=   -lopencv_core300d \
    -lopencv_videoio300d \
    -lopencv_imgcodecs300d \
    -lopencv_highgui300d \
    -lopencv_imgproc300d \
    -lopencv_calib3d300d \
    -lopencv_features2d300d \
    -lopencv_flann300d \
    -lopencv_aruco300d


#INCLUDEPATH += "C:/OpenCv/build/include"
#LIBS += "-LC:/OpenCv/build/x86/vc10/lib" \
#    -lopencv_core247 \
#    -lopencv_highgui247 \
#    -lopencv_imgproc247 \
#    -lopencv_calib3d247 \
#    -lopencv_features2d247


#    -lopencv_core243 \
#    -lopencv_highgui243 \
#    -lopencv_imgproc243 \
#    -lopencv_calib3d243 \
#    -lopencv_features2d243

TARGET = RobotDetection
TEMPLATE = app

SOURCES += main.cpp\
        robotdetectionmainwindow.cpp \
    myudp.cpp \
    imgtask.cpp

HEADERS  += robotdetectionmainwindow.h \
    myudp.h \
    imgtask.h \
    constants.h

FORMS    += robotdetectionmainwindow.ui

RESOURCES += \
    ressource.qrc

QMAKE_POST_LINK += copy "release\\*.exe  \"deploy\\\""

