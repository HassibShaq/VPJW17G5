#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <QDebug>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void saveArucoDictionary(cv::aruco::Dictionary Dictionary, QString Path);
    cv::Ptr<cv::aruco::Dictionary> loadArucoDictionary(QString Path);

private slots:
    void react(void);

    void on_pushButtonSave_clicked();

    void on_pushButtonGenerate_2_clicked();

private:
    Ui::MainWindow *ui;
    cv::Ptr<cv::aruco::Dictionary> GeneratedDict;
    int MarkerCount;
    int MarkerSizeBit;
    int MarkerSizePixel;
};

#endif // MAINWINDOW_H
