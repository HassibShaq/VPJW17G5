#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //F5 Shortcut for Start/Stop Camera
    QShortcut *shortcutF5 = new QShortcut(QKeySequence(Qt::Key_F5), this);
    connect(shortcutF5, SIGNAL(activated()), ui->startCameraButton, SLOT(click()));

    ui->stopCameraButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startCameraButton_clicked()
{
    cameraTimer.start(33); // 33 ms = 30 fps

    //SIGNAL AND SLOT CONNECTION
    //at the timeout() event, execute the cameraTimerTimeout() method
    //sender (source of signal): of type QTimer; signal (value of signal): timeout()
    //receiver: this Window - slot:a function of the receiver that processes the incoming signal

    capture.open(ui->spinBoxCaptureID->value());

    if (capture.isOpened())
    {
        connect(&cameraTimer, SIGNAL(timeout()), this, SLOT(frameReady()));
        ui->startCameraButton->setEnabled(false);
        ui->stopCameraButton->setEnabled(true);
    }
    else
    {
        ui->statusBar->showMessage("Unable to open capture", 3000);
    }
}

void MainWindow::frameReady()
{
    if(capture.isOpened())
    {
        // store the frame to show in a Qt window
        QImage frameToShow;

        // get the current frame from the video stream
        // Mat image wird beschrieben mit Hilfe des überladenen Operators >>
        //Aus VideoCapture wird hier also ein 2D Mat
        capture.read(image);
        image.copyTo(imageForGui);

        // prepare the image for the Qt format...
        // ... change color channel ordering (from BGR in our Mat to RGB in QImage)
        cvtColor(imageForGui, imageForGui, CV_BGR2RGB);

        cv::Mat threshold;

        cv::threshold(imageForGui, threshold, ui->horizontalSliderThreshold->value(), ui->horizontalSliderMaxvalue->value(), THRESH_BINARY);

        cv::imshow("Test", threshold);

        //3. Bild resizen und altes imageForGui mit neuem ersetzen
        //Resizes an image in order to adapt to the GUI size

        cv::resize(imageForGui, imageForGui, Size(ui->labelImage->width(), ui->labelImage->height()));


        //QImage
        frameToShow = QImage((const unsigned char*)(imageForGui.data), imageForGui.cols, imageForGui.rows, imageForGui.step, QImage::Format_RGB888);


        ui->labelImage->setScaledContents(true);
        // display on label (left label)
        ui->labelImage->setPixmap(QPixmap::fromImage(frameToShow));

    }
}



void MainWindow::on_stopCameraButton_clicked()
{
    if(capture.isOpened())
    {

        // release camera stream - Closes video file or capturing device.
        capture.release();
    }

    // stop timer
    cameraTimer.stop();

    ui->stopCameraButton->setEnabled(false);
    ui->startCameraButton->setEnabled(true);
}
