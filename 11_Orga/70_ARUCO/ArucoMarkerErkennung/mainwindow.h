#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
//#include <QResizeEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initCamera();
    cv::Ptr<cv::aruco::Dictionary> loadArucoDictionary(QString Path);

private slots:
    void on_pushButtonSnap_clicked();

private:
    Ui::MainWindow *ui;
    cv::Mat image, imageForGui, ImageDog;
    cv::Ptr<cv::aruco::Dictionary> ArucoDict;
    cv::VideoCapture capture;
    QTimer CameraTimer;
    std::vector<std::vector<cv::Point2f>> corners;
    std::vector<int> ids;
};

#endif // MAINWINDOW_H
