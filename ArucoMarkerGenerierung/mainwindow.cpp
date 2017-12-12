#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
    connect(ui->pushButtonGenerate, SIGNAL(clicked()), this, SLOT(react()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::react()
{
    try {
        this->MarkerCount = ui->lineEditMNo->text().toInt();
        this->MarkerSizeBit = ui->lineEditMSizeBit->text().toInt();
        this->MarkerSizePixel = ui->lineEditMSizePix->text().toInt();
    }catch(...) {
        ui->statusBar->showMessage("Wrong Input Format", 1000);
        return;
    }

    cv::Mat Marker;

    this->GeneratedDict = cv::aruco::generateCustomDictionary(this->MarkerCount, this->MarkerSizeBit);
    ui->statusBar->showMessage("Dictionary generated.", 1000);
}

void MainWindow::saveArucoDictionary(cv::aruco::Dictionary Dictionary, QString Path)
{
    cv::FileStorage Storage(Path.toUtf8().constData(), cv::FileStorage::WRITE);
    Storage << "MarkerSize" << Dictionary.markerSize;
    Storage << "MaxCorrectionBits" << Dictionary.maxCorrectionBits;
    Storage << "ByteList" << Dictionary.bytesList;
    Storage.release();

    cv::Mat Marker;

    for (int i = 0; i<this->MarkerCount; i++)
    {
        cv::aruco::drawMarker(this->GeneratedDict, i, this->MarkerSizePixel, Marker);
        cv::imwrite(QString("Marker%1.png").arg(i).toUtf8().constData(), Marker);
    }
    ui->statusBar->showMessage("Dictionary saved.", 1000);
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

void MainWindow::on_pushButtonSave_clicked()
{
    this->saveArucoDictionary(this->GeneratedDict, "test.yml");
}

void MainWindow::on_pushButtonGenerate_2_clicked()
{
    cv::Mat Marker;
    for (int i = 0; i<this->MarkerCount; i++)
    {
        cv::aruco::drawMarker(this->GeneratedDict, i, this->MarkerSizePixel, Marker);
        cv::imshow(QString("Marker %1").arg(i).toUtf8().constData(), Marker);
    }
}
