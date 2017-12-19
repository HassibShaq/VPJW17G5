#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ArucoDict = loadArucoDictionary("test.yml");
    ImageDog = cv::imread("neal.jpg");
    cv::cvtColor(ImageDog,ImageDog, cv::COLOR_BGR2RGB);
    cv::resize(ImageDog, ImageDog, cv::Size(50, 50));
    CameraTimer.start(33); // 33 ms = 30 fps
    connect(&CameraTimer, SIGNAL(timeout()), this, SLOT(on_pushButtonSnap_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonSnap_clicked()
{
    if(capture.isOpened())
    {
        QImage frameToShow;
        capture.read(image);
        image.copyTo(imageForGui);

        cv::cvtColor(imageForGui, imageForGui, cv::COLOR_BGR2RGB);
        cv::aruco::detectMarkers(imageForGui, ArucoDict, corners, ids);
        //3. Bild resizen und altes imageForGui mit neuem ersetzen
        //Resizes an image in order to adapt to the GUI size
        cv::aruco::drawDetectedMarkers(imageForGui, corners, ids);

        for(int i = 0; i < corners.size(); i++)
        {
            ImageDog.copyTo(imageForGui(cv::Rect(corners[i][0].x,corners[i][0].y,ImageDog.cols, ImageDog.rows)));
        }

        cv::resize(imageForGui, imageForGui, cv::Size(ui->labelImage->width(), ui->labelImage->height()));
        //QImage
        frameToShow = QImage((const unsigned char*)(imageForGui.data), imageForGui.cols, imageForGui.rows, imageForGui.step, QImage::Format_RGB888);

        ui->labelImage->setScaledContents(true);
        // display on label (left label)
        ui->labelImage->setPixmap(QPixmap::fromImage(frameToShow));

    }
}

void MainWindow::initCamera()
{
    capture.open(0);

    if (!capture.isOpened())
    {
        ui->statusBar->showMessage("Unable to open capture", 3000);
    }
}

cv::Ptr<cv::aruco::Dictionary> MainWindow::loadArucoDictionary(QString Path)
{
    cv::aruco::Dictionary Dictionary;
    cv::FileStorage Storage(Path.toUtf8().constData(), cv::FileStorage::READ);
    int MSize, MCBits;
    cv::Mat BytesList;
    Storage["MarkerSize"] >> MSize;
    Storage["MaxCorrectionBits"] >> MCBits;
    Storage["ByteList"] >> BytesList;
    Storage.release();
    Dictionary = cv::aruco::Dictionary(BytesList, MSize, MCBits);
    return cv::makePtr<cv::aruco::Dictionary>(Dictionary);
}
