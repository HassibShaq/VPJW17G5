#ifndef NEWFUNCTION_H
#define NEWFUNCTION_H
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <stdio.h>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"
#include <opencv2/aruco.hpp>
#include <QXmlStreamAttributes>
#include <QXmlStreamReader>
#include <QtCore>
#include <QtGui>
#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QTime>
#include <opencv2/opencv.hpp>
#include <QMessageBox>
#include <QThread>
#include <QThreadPool>
#include <QList>
#include <fstream>

double calculateangle(std::vector<cv::Point2f> marker, int &quadrant);

double calculateanglebetweenmarker(std::vector<cv::Point2f> marker1, std::vector<cv::Point2f> marker2);

cv::Point2f calculatemiddlepoint(std::vector<cv::Point2f> marker1, std::vector<cv::Point2f> marker2, double angle);

void sortbyfirstvector(std::vector<int> *markerIds, std::vector<std::vector<cv::Point2f>> *markerCorners);

int getroboterid(int markerID);

#endif // NEWFUNCTION_H
