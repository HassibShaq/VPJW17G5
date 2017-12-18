#ifndef CAMERACALIBRATOR_H
#define CAMERACALIBRATOR_H

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <QMessageBox>
#include <time.h>
#include <QXmlStreamWriter>
#include <QFile>
#include <QDebug>

using namespace cv;
using namespace std;

class CameraCalibrator
{
public:
    CameraCalibrator();
    int addCirclePoints(const vector<Mat> &imageList, Size &boardSize);
    double calibrate(Size &imageSize);
    Mat remap(const Mat &image);
    void writeCalibration2File();
    void writeXML(QString fileName, double exposureValue, double contrastValue, double brightnessValue);
    //void readCalibrationFromFile(const string inputSettingsFilePath);
    Mat getCameraMatrix();
    Mat getDistCoeffs();
    int getSelectedCamID();
    void setSelectedCamID(int camID);
    double getExp();
    double getCnt();
    double getBrg();
    bool resetVals();
    void setDistanceWidth(float distanceWidth);
    void setDistanceHeight(float distanceHeight);


private:
    void addPoints(const vector<Point2f>& pointBuf, const vector<Point3f>& circlePoints3d);
    void run(); // from QThread

    vector< vector<Point3f> > objectPoints; // real world 3D coordinates
    vector< vector<Point2f> > imagePoints; // 2D coordinates (pixel values)

    //output
    Mat cameraMatrix;
    Mat distCoeffs;

    int flag;

    // for undistort
    Mat map1, map2;
    bool mustInitUndistort;
    int selectedCamID;
    double exp, cnt, brg;
    float distanceWidth; // Distance between the center of the circles - Width Direction
    float distanceHeight;// Distance between the center of the circles - Height Direction
};

#endif // CAMERACALIBRATOR_H
