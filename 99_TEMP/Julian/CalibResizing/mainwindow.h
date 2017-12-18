#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QShortcut>
#include "cameracalibrator.h"
//#include <QPainter>
//#include <QPointF>
#include <QResizeEvent>


using namespace std;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
   explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void frameReady();
    void on_stopCameraButton_clicked();
    void on_startCameraButton_clicked();

private:
    Ui::MainWindow *ui;
    cv::Mat image,  imageForGui;
    cv::VideoCapture capture;
    QTimer cameraTimer;
};

#endif // MAINWINDOW_H
