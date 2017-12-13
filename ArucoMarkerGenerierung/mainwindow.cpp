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
    BaseDict = cv::makePtr<cv::aruco::Dictionary>();
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
        ui->statusBar->showMessage("Wrong Input Format", 3000);
        return;
    }

    cv::Mat Marker;

    this->GeneratedDict = cv::aruco::generateCustomDictionary(this->MarkerCount, this->MarkerSizeBit, BaseDict);
    ui->statusBar->showMessage("Dictionary generated.", 3000);
}

void MainWindow::on_pushButtonSave_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save File",
                                                    QDir::currentPath(),
                                                    "Aruco Dictonary (*.yml)");
    if (!fileName.isEmpty()) {
        ArucoSerializer::saveArucoDictionary(this->GeneratedDict, fileName);
        ui->statusBar->showMessage("Dictionary saved.", 3000);
    }
}

void MainWindow::on_pushButtonLoadBase_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File",
                                                    QDir::currentPath(),
                                                    "Aruco Dictonary (*.yml)");
    if (!fileName.isEmpty()) {
        BaseDict = ArucoSerializer::loadArucoDictionary(fileName);
        GeneratedDict = BaseDict;
        MarkerSizeBit = GeneratedDict.get()->markerSize;
        ui->lineEditMSizeBit->setText(QString::number(MarkerSizeBit));
        ui->lineEditMSizeBit->setEnabled(false);
        ui->pushButtonLoadBase->setEnabled(false);
        ui->pushButtonClearBase->setEnabled(true);
        ui->statusBar->showMessage("Dictionary loaded.", 3000);
    }

}

void MainWindow::on_pushButtonSaveImages_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this, "Open Directory",
                                                    QDir::currentPath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (!fileName.isEmpty()) {
        ArucoSerializer::drawArucoDictionary(GeneratedDict, fileName, QString::number(QDateTime::currentMSecsSinceEpoch()), MarkerSizePixel);
    }
}

void MainWindow::on_pushButtonGeneratePreview_clicked()
{
    cv::Mat Marker;
    for (int i = 0; i<this->MarkerCount; i++)
    {
        cv::aruco::drawMarker(this->GeneratedDict, i, this->MarkerSizePixel, Marker);
        cv::imshow(QString("Marker %1").arg(i).toUtf8().constData(), Marker);
    }
}

void MainWindow::on_pushButtonClearBase_clicked()
{
    BaseDict = cv::makePtr<cv::aruco::Dictionary>();
    ui->lineEditMSizeBit->setText("");
    ui->lineEditMSizeBit->setEnabled(true);
    ui->pushButtonLoadBase->setEnabled(true);
    ui->pushButtonClearBase->setEnabled(false);
    ui->statusBar->showMessage("Dictionary cleared.", 3000);
}
