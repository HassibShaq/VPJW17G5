#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <arucoserializer.h>
#include <QDebug>
#include <QFileDialog>
#include <QDateTime>

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

    void on_pushButtonLoadBase_clicked();

    void on_pushButtonSaveImages_clicked();

    void on_pushButtonGeneratePreview_clicked();

    void on_pushButtonClearBase_clicked();

private:
    Ui::MainWindow *ui;
    cv::Ptr<cv::aruco::Dictionary> BaseDict;
    cv::Ptr<cv::aruco::Dictionary> GeneratedDict;
    int MarkerCount;
    int MarkerSizeBit;
    int MarkerSizePixel;
};

#endif // MAINWINDOW_H
