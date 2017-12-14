#ifndef ROBOTDETECTIONMAINWINDOW_H
#define ROBOTDETECTIONMAINWINDOW_H


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
#include "myudp.h"
#include "imgtask.h"
#include "constants.h"

namespace Ui {
class RobotDetectionMainWindow;
}

class RobotDetectionMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RobotDetectionMainWindow(QWidget *parent = 0);
    ~RobotDetectionMainWindow();

private:
    Ui::RobotDetectionMainWindow *ui;
    MyUDP udpClient;
    int timerMilSecs;
    QTimer timer;
    QTimer timerFPS;
    QThreadPool threadPool;
    ImgTask *tasks[NR_OF_CAMS];
    QMutex workerMutex;    
    QString sendToIp;
    int sendToPort;
    QString sendToIp_SyncService;
    int sendToPort_SyncService;
    QString reciveIp_SyncService;
    int recivePort_SyncService;
    QList<cv::Mat> cameraMatrix;
    QList<cv::Mat> distCoeffs;
    QList<cv::Mat> perspTransfMatrix;
    QList<int> exposureValue;
    QList<int> contrastValue;
    QList<int> brightnessValue;
    QList<cv::Point3f> robotLocations;
    cv::VideoCapture videoCapture[NR_OF_CAMS];
    cv::Mat cameraImages[NR_OF_CAMS];
    cv::Mat originalImages[NR_OF_CAMS];
    bool mainloopIsActive;
    bool calibrateOffset_ON_OFF;
    int fpsCount;
    QTime timeStamp;
protected:
    void writeRobotLocationsToTable();
    void writeRobotIDsToGui(cv::Mat guiImage);
    void readXmlCalibrationFile();

    double scaleToGui(double value);
    double distanceBetweenPoints(cv::Point2f a, cv::Point2f b);

    cv::Point2f scaleToGui(cv::Point2f srcDot);
    cv::Point3f scaleToGui(cv::Point3f srcDot);

    QMap<QString, QXmlStreamAttributes> parseCamera(QXmlStreamReader& xmlReader);

private slots:
    void on_pushButtonCalibrateOffset_clicked();
    void on_pushButtonStartStop_clicked();
    void operate();
    void fpsCounter();

};

#endif // ROBOTDETECTIONMAINWINDOW_H
