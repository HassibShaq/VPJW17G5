/****************************************************************************
 * This is the RobotDetection main programm. It is part of the camera
 * calibration and robot detection system created by  "Gewerk 5"  during the
 * cooperative project of automation technologies in 2013/2014 at the Hamburg
 * University of Applied Sciences.
 *
 * Filename: robotdetectionmainwindow.cpp
 * Author:   Markus Baden, Benjamin Wilberger
 * Created:  2013-12-24
 * Changed:  2014-04-17
 * Changed:  2016-04-12 : Extend by ARUCO and Clock-Synchronisation
 ***************************************************************************/

#include "robotdetectionmainwindow.h"
#include "ui_robotdetectionmainwindow.h"
#include <iostream>
#include <fstream>

using namespace cv;



RobotDetectionMainWindow::RobotDetectionMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RobotDetectionMainWindow),
    udpClient(this),
    threadPool(this)
{
    ui->setupUi(this);
    // read settings from ini file
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup("RobotDetectionSettings");
    sendToIp = settings.value("SendToIP", "192.168.0.255").toString();
    sendToPort = settings.value("SendToPort", 25000).toInt();

    sendToIp_SyncService = settings.value("SendToIP_SyncService", "192.168.0.255").toString();
    sendToPort_SyncService = settings.value("SendToPort", 25110).toInt();
    reciveIp_SyncService = settings.value("ReciveIP_SyncService", "192.168.0.20").toString();
    recivePort_SyncService = settings.value("RecivePort_SyncService", 25111).toInt();

    timerMilSecs = settings.value("TimerMilSecs", 20).toInt();
    ui->slider_cornerRefinementMaxIterations->setValue( settings.value("cornerRefinementMaxIterations", 1).toInt());
    ui->slider_cornerRefinementMinAccuracy->setValue  ( settings.value("cornerRefinementMinAccuracy", 10).toDouble() );
    ui->slider_errorCorrectionRate->setValue  ( settings.value("errorCorrectionRate", 511).toDouble());
    ui->slider_adaptiveThreshWinSizeMin->setValue( settings.value("adaptiveThreshWinSizeMin", 10).toInt());
    ui->slider_adaptiveThreshWinSizeStep->setValue( settings.value("adaptiveThreshWinSizeStep", 99).toInt());
    ui->slider_adaptiveThreshConstant->setValue( settings.value("adaptiveThreshConstant", 10).toInt() );
    ui->slider_minMarkerPerimeterRate->setValue( settings.value("minMarkerPerimeterRate", 91).toDouble());
    ui->slider_maxMarkerPerimeterRate->setValue( settings.value("slider_maxMarkerPerimeterRate", 300).toDouble() );
    ui->slider_polygonalApproxAccuracyRate->setValue( settings.value("slider_polygonalApproxAccuracyRate", 11).toDouble());
    ui->slider_perspectiveRemovePixelPerCell->setValue( settings.value("slider_perspectiveRemovePixelPerCell", 1).toInt());
    ui->slider_threshold->setValue( settings.value("threshold", 160).toInt());
    ui->slider_MinSizeofRects->setValue( settings.value("MinSizeofRects", 8000).toInt());

    //Set Offset for Robots
    QList<RobotOffset> RobotOffsets;
    for(int i = 0; i < MAX_NR_OF_ROBOTS; i++)
    {
        RobotOffset temp = {i,settings.value("Robot"+QString::number(i)+"MEven",2).toFloat(),settings.value("Robot"+QString::number(i)+"MNotEven",2).toFloat()};
        RobotOffsets.append(temp);
    }
    settings.endGroup();

    // create UDP-Client and open socket
    //udpClient.setSendIP(sendToIp, sendToPort);


    udpClient.setSendConfig (sendToIp,sendToPort);

    udpClient.setReciveConfig_SyncService(reciveIp_SyncService,recivePort_SyncService);
    udpClient.setSendConfig_SyncService(sendToIp_SyncService,sendToPort_SyncService);

    mainloopIsActive = false;
    calibrateOffset_ON_OFF = false;

    // init timers for mainloop and frames-per-second counting
    connect(&timer, SIGNAL(timeout()), this, SLOT(operate()));
    connect(&timerFPS, SIGNAL(timeout()), this, SLOT(fpsCounter()));
    fpsCount = 0;

    // read camera calibration data and perspective transform matrices from xml
    readXmlCalibrationFile();

    // init list of robotLocations
    robotLocations.clear();

    for (int i = 0; i < MAX_NR_OF_ROBOTS; i++)
    {
        robotLocations.append(Point3f(0.0, 0.0, 0.0));
    }

    // open threadpool for image analysis
    threadPool.setMaxThreadCount(NR_OF_CAMS);
    for (int i = 0; i < NR_OF_CAMS; i++)
    {
        tasks[i] = new ImgTask();
        tasks[i]->setAutoDelete(false);
        tasks[i]->setCameraMatrix(cameraMatrix.at(i));
        tasks[i]->setDistCoeffs(distCoeffs.at(i));
        tasks[i]->setPerspTransfMatrix(perspTransfMatrix.at(i));
        tasks[i]->setRobotOffsets(RobotOffsets);
    }

    ui->labelVersion->setText("Version: " + QString(__DATE__) + " " + QString(__TIME__));

    setMaximumSize(size());
}

RobotDetectionMainWindow::~RobotDetectionMainWindow()
{
    // write settings to ini file
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup("RobotDetectionSettings");
    settings.setValue("SendToIP", sendToIp);
    settings.setValue("SendToPort", sendToPort);
    settings.setValue("TimerMilSecs", timerMilSecs);

    settings.setValue("cornerRefinementMaxIterations", ui->slider_cornerRefinementMaxIterations->value());
    settings.setValue("cornerRefinementMinAccuracy", ui->slider_cornerRefinementMinAccuracy->value());
    settings.setValue("errorCorrectionRate", ui->slider_errorCorrectionRate->value());
    settings.setValue("adaptiveThreshWinSizeMin", ui->slider_adaptiveThreshWinSizeMin->value());
    settings.setValue("adaptiveThreshWinSizeStep", ui->slider_adaptiveThreshWinSizeStep->value());
    settings.setValue("adaptiveThreshConstant", ui->slider_adaptiveThreshConstant->value());
    settings.setValue("minMarkerPerimeterRate", ui->slider_minMarkerPerimeterRate->value());
    settings.setValue("maxMarkerPerimeterRate", ui->slider_maxMarkerPerimeterRate->value());
    settings.setValue("polygonalApproxAccuracyRate", ui->slider_polygonalApproxAccuracyRate->value());
    settings.setValue("perspectiveRemovePixelPerCell", ui->slider_perspectiveRemovePixelPerCell->value());
    settings.setValue("threshold", ui->slider_threshold->value());
    settings.setValue("MinSizeofRects", ui->slider_MinSizeofRects->value());
    settings.endGroup();

    // clean up
    for (int i = 0; i < NR_OF_CAMS; i++)
    {
        delete tasks[i];
    }
    delete ui;
}
void RobotDetectionMainWindow::on_pushButtonCalibrateOffset_clicked()
{
    calibrateOffset_ON_OFF = true;
}
void RobotDetectionMainWindow::on_pushButtonStartStop_clicked()
{
    if(mainloopIsActive)
    {
        timer.stop();
        timerFPS.stop();
        for (int i = 0; i < NR_OF_CAMS; i++)
        {
            videoCapture[i].release();
        }
        ui->statusBar->showMessage("Releasing cameras...", 2000);
        ui->pushButtonStartStop->setText("Start Detection");
        mainloopIsActive = false;
    }
    else
    {
        for (int i = 0; i < NR_OF_CAMS; i++)
        {
            videoCapture[i].open(i);
            videoCapture[i].set(CV_CAP_PROP_FRAME_WIDTH, CAMERA_IMG_WIDTH);
            videoCapture[i].set(CV_CAP_PROP_FRAME_HEIGHT, CAMERA_IMG_HEIGTH);
            videoCapture[i].set(CV_CAP_PROP_BRIGHTNESS, brightnessValue[i]);
            videoCapture[i].set(CV_CAP_PROP_CONTRAST, contrastValue[i]);
            videoCapture[i].set(CV_CAP_PROP_EXPOSURE, exposureValue[i]);
        }
        ui->statusBar->showMessage("Connecting to cameras...", 3000);
        // give cameras some time to get connected,
        // otherwise program might crash (depends on camera driver)
        Sleep(3000);
        //timer.start(timerMilSecs);
        timer.start(1);
        timerFPS.start(1000);
        ui->pushButtonStartStop->setText("Stop Detection");
        mainloopIsActive = true;
    }
}

void RobotDetectionMainWindow::operate()
{
    if(!workerMutex.tryLock())
    {
        return;
    }

    // GRAB TIMESTAMP
    timeStamp = QTime::currentTime(); // read system time
    QString stime = timeStamp.toString(Qt::LocalDate);  // convert time to string

    fpsCount++;

    QList<double> temp;

    //Read Settings
    aruco::DetectorParameters arucoParameters;

    if(ui->slider_cornerRefinementMaxIterations->value() > 2)
        arucoParameters.doCornerRefinement = true;
    else
        arucoParameters.doCornerRefinement = false;

    ui->t4_label->setText(QString::number(udpClient.msg_4));

    arucoParameters.perspectiveRemovePixelPerCell = ui->slider_perspectiveRemovePixelPerCell->value();
    arucoParameters.cornerRefinementMaxIterations = ui->slider_cornerRefinementMaxIterations->value();
    arucoParameters.adaptiveThreshWinSizeMin = ui->slider_adaptiveThreshWinSizeMin->value();
    arucoParameters.adaptiveThreshWinSizeStep = ui->slider_adaptiveThreshWinSizeStep->value();
    arucoParameters.adaptiveThreshConstant = ui->slider_adaptiveThreshConstant->value();

    temp.append(ui->slider_minMarkerPerimeterRate->value());
    temp.append(ui->slider_maxMarkerPerimeterRate->value());
    temp.append(ui->slider_polygonalApproxAccuracyRate->value());
    temp.append(ui->slider_errorCorrectionRate->value());
    temp.append(ui->slider_cornerRefinementMinAccuracy->value());

    arucoParameters.minMarkerPerimeterRate = temp[0]/100;
    arucoParameters.maxMarkerPerimeterRate = temp[1]/100;
    arucoParameters.polygonalApproxAccuracyRate = temp[2]/100;
    arucoParameters.errorCorrectionRate = temp[3]/100;
    arucoParameters.cornerRefinementMinAccuracy = temp[4]/100;

    // READ IMAGES FROM CAMERAS
    // grab frames with smallest time difference possible
    // "grab()" + "retrieve()" is faster than the combined function "read()"

    for (int i = 0; i < NR_OF_CAMS; i++)
    {
        if (videoCapture[i].isOpened())
        {
            videoCapture[i].grab();
        }
    }


    // Fetch detected Robots from tasks
    QList<RobotPosition> detectedRobots;

    for (int i = 0; i < NR_OF_CAMS; i++)
    {
        if (videoCapture[i].isOpened())
        {
            videoCapture[i].retrieve(cameraImages[i], 0);   // get frames from camera
        }
        else     // no camera connected, use black image
        {
            Mat blackImage(CAMERA_IMG_HEIGTH, CAMERA_IMG_WIDTH, CV_8UC3);
            blackImage.setTo(COLOR_BLACK);
            cameraImages[i] = blackImage;
        }
        // start seperate tasks
        tasks[i]->setImage( cameraImages[i] );
        tasks[i]->setDebugMode( ui->checkBoxLiveView->isChecked() );
        tasks[i]->setCalibrateOffset(calibrateOffset_ON_OFF);
        tasks[i]->setArucoParameters(arucoParameters);
        tasks[i]->setthreshold(ui->slider_threshold->value());
        tasks[i]->setMinSizeofRects(ui->slider_MinSizeofRects->value());
        threadPool.start( tasks[i] );
    }
    threadPool.waitForDone();

    for (int i = 0; i < NR_OF_CAMS; i++)
    {
        detectedRobots.append(tasks[i]->getdetectRobots());
    }


    //User clicked on Calibration
    if(calibrateOffset_ON_OFF)
    {
        //Fetch Calibration Data from Tasks
        QList<RobotOffset> foundOffsets;

        for (int i = 0; i < NR_OF_CAMS; i++)
        {
            foundOffsets.append(tasks[i]->getNewRobotOffsets());
            tasks[i]->clearNewRobotOffsets();
        }

        if(foundOffsets.size() != MAX_NR_OF_ROBOTS)
        {
            QString alarmtxt = "Calibration failed! Please place all Robots in Field! Founded Robots are: ";
            for(int i = 0; i < foundOffsets.size() ;i++)
            {
                alarmtxt = alarmtxt + QString::number(foundOffsets.at(i).id+1);

                if(i+1 != foundOffsets.size())
                    alarmtxt = alarmtxt + ",";
            }
            QMessageBox msgBox;
            msgBox.setText(alarmtxt);
            msgBox.exec();
        }
        else
        {
            //Save Offsets
            QSettings settings("settings.ini", QSettings::IniFormat);
            settings.beginGroup("RobotDetectionSettings");
            QString optionNameQString;
            for(int i = 0; i < foundOffsets.size() ;i++)
            {
                //Build String
                optionNameQString = "Robot" + QString::number(foundOffsets.at(i).id);
                settings.setValue(optionNameQString + "MEven",QString::number(-foundOffsets.at(i).offsetMarkerEven));
                settings.setValue(optionNameQString + "MNotEven",QString::number(foundOffsets.at(i).offsetMarkerNotEven));
            }
            settings.endGroup();
        }

        calibrateOffset_ON_OFF = false;
        foundOffsets.clear();
    }


    // Display either camera images or white background
    Mat guiImage(GUI_HEIGTH, GUI_WIDTH, CV_8UC3);
    if (ui->checkBoxLiveView->isChecked())   // show real frames
    {
        guiImage.setTo(COLOR_BLACK);
        for (int i = 0; i < NR_OF_CAMS; i++)
        {
            addWeighted(guiImage, 1, tasks[i]->getWarpedImage(), 1, 0, guiImage, -1);
        }
    }
    else     // show white background with grid
    {
        guiImage.setTo(COLOR_WHITE);
        for (unsigned int i = 0; i < 40; i++)
        {
            if (i % 5 == 0)
            {
                line(guiImage, Point(i * 200 / GUI_SCALING, 0), Point(i * 200 / GUI_SCALING, guiImage.rows), COLOR_DARK_GREY,  1, 8, 0);
            }
            else
            {
                line(guiImage, Point(i * 200 / GUI_SCALING, 0), Point(i * 200 / GUI_SCALING, guiImage.rows), COLOR_LIGHT_GREY, 1, 8, 0);
            }
        }
        for (unsigned int i = 0; i < 20; i++)
        {
            if (i % 5 == 0)
            {
                line(guiImage, Point(0, i * 200 / GUI_SCALING), Point(guiImage.cols, i * 200 / GUI_SCALING), COLOR_DARK_GREY,  1, 8, 0);
            }
            else
            {
                line(guiImage, Point(0, i * 200 / GUI_SCALING), Point(guiImage.cols, i * 200 / GUI_SCALING), COLOR_LIGHT_GREY, 1, 8, 0);
            }
        }
    }

    // init locations with zeros
    for (int i = 0; i < MAX_NR_OF_ROBOTS; i++)
    {
        robotLocations[i] = Point3f(0, 0, 0);
    }

    //Check all Robots of double detections
    //That mean, two or more Robots, are detected on the same Place (This is not possible!)
    QList<int> doubledetecterobots;
    for(int a = 0;a<MAX_NR_OF_ROBOTS;a++)
    {
        for(int i = 0; i < detectedRobots.size();i++)
        {
            if(detectedRobots[i].id == a)
            {
                for(int b = 0; b < detectedRobots.size(); b++)
                {

                    if(i != b
                            &&  detectedRobots[i].id != detectedRobots[b].id
                            && (detectedRobots[i].coordinates.x >= detectedRobots[b].coordinates.x-250 && detectedRobots[i].coordinates.x <= detectedRobots[b].coordinates.x+250)
                            && (detectedRobots[i].coordinates.y >= detectedRobots[b].coordinates.y-250 && detectedRobots[i].coordinates.y <= detectedRobots[b].coordinates.y+250))
                    {
                        doubledetecterobots.append(a);
                    }

                }
                robotLocations[a] = detectedRobots[i].coordinates;
            }
        }

        for(int i =0;i < doubledetecterobots.size();i++)
        {
            robotLocations[doubledetecterobots.at(i)] = Point3f(0, 0, 0);
        }
    }

    //If Measurement is checked, save all Robot Positions, in a txt. File
    if(ui->checkBox_Measurement->isChecked())
    {
        std::ofstream f;
        f.open("Measurement.txt",std::ios::app);

        for(int a = 0;a<robotLocations.size();a++)
        {
            f << robotLocations[a].x << "\t" << robotLocations[a].y << "\t" << robotLocations[a].z << "\t";
        }

        f << "\n";
        f.close();
    }

    // sendUDPdata...
    udpClient.sendUdpData(robotLocations, timeStamp);
    // write robotLocations to tablewidget
    writeRobotLocationsToTable();

    // invert y-axis
    flip(guiImage, guiImage, 0);


    //Draw Line and Circle for Robots in GUI
    for(int i = 0; i < robotLocations.size();i++)
    {

        //If a Robot are detected
        if(robotLocations[i].x != 0)
        {

            Point2f centerPoint = Point2f(robotLocations[i].x, FIELD_HEIGTH - robotLocations[i].y);
            double centerRadius = ROBOT_RADIUS;
            double angledegree = 2*3.14159265359-(robotLocations[i].z*3.14159265359/180);
            Point2f directionPoint = Point2f(scaleToGui(centerPoint).x + scaleToGui(ROBOT_RADIUS)*cos(angledegree ), scaleToGui(centerPoint).y + scaleToGui(ROBOT_RADIUS)*sin(angledegree));

            //Draw circles and lines for center and orientation circles
            circle(guiImage, scaleToGui(centerPoint), scaleToGui(centerRadius), COLOR_GREEN, 2, CV_AA);
            line  (guiImage, scaleToGui(centerPoint), directionPoint, COLOR_BLUE, 2, CV_AA);
        }
    }

    detectedRobots.clear();


    // show error message in image if camera is missing
    for (int i = 0; i < NR_OF_CAMS; i++ )
    {
        if( ! videoCapture[i].isOpened())
        {
            QString str = "Camera not connected. ID: ";
            str.append(QString::number(i));
            std::string str2 = str.toStdString();
            double x = 0, y = 0;
            if ( i % 2 == 0)
            {
                y = GUI_HEIGTH / 4 - 20;
            }
            else
            {
                y = GUI_HEIGTH * 3 / 4 + 20;
            }
            if ( (i == 0) || (i == 5) )
            {
                x = 60;
            }
            else if ( (i == 1) || (i == 2) )
            {
                x = 60 + GUI_WIDTH / 3;
            }
            else if ( (i == 3) || (i == 4) )
            {
                x = 60 + 2 * GUI_WIDTH / 3;
            }
            putText(guiImage, str2, Point2f(x, y), CV_FONT_HERSHEY_PLAIN, 1, COLOR_DARK_GREY, 1, 8, false);
        }
    }

    // Write robot IDs into guiImage
    writeRobotIDsToGui(guiImage);

    //convert colors and show image in GUI
    cvtColor(guiImage, guiImage, CV_BGR2RGB);
    QPixmap pixmap;
    pixmap = QPixmap::fromImage(QImage((unsigned char*) guiImage.data, guiImage.cols, guiImage.rows, QImage::Format_RGB888));
    ui->labelImage->setPixmap(pixmap);

    workerMutex.unlock();
}

void RobotDetectionMainWindow::fpsCounter()
{
    QString str = "Frames per second: ";
    str.append(QString::number(fpsCount));
    ui->labelFPS->setText(str);
    fpsCount = 0;
}

void RobotDetectionMainWindow::writeRobotLocationsToTable()
{
    // init tableWidget during first run
    for(int row = 0; row < MAX_NR_OF_ROBOTS; row++)
        for(int col = 0; col < 3; col++)
            if (ui->tableWidget->item(row, col) == 0)
            {
                QTableWidgetItem *itab = new QTableWidgetItem;
                itab->setText("init");
                ui->tableWidget->setItem(row, col, itab);
            }

    // write RobotLocations to tableWidget
    for(int row = 0; row < MAX_NR_OF_ROBOTS; row++)
    {
        ui->tableWidget->item(row, 0)->setText(QString::number(robotLocations.at(row).x, 'f', 1));
        ui->tableWidget->item(row, 1)->setText(QString::number(robotLocations.at(row).y, 'f', 1));
        ui->tableWidget->item(row, 2)->setText(QString::number(robotLocations.at(row).z, 'f', 1));
    }
}

void RobotDetectionMainWindow::writeRobotIDsToGui(cv::Mat guiImage)
{
    Point2f offset = Point2f(ROBOT_RADIUS, - ROBOT_RADIUS);
    for (unsigned int i = 0; i < MAX_NR_OF_ROBOTS; i++)
    {
        Point2f center = Point2f(robotLocations.at(i).x, FIELD_HEIGTH - robotLocations.at(i).y);
        if (center.y != FIELD_HEIGTH)
        {
            putText( guiImage, QString::number(i + 1).toStdString(), scaleToGui(center) + scaleToGui(offset),
                     CV_FONT_HERSHEY_PLAIN, 2, COLOR_RED, 2, CV_AA, false);
        }
    }
}

double RobotDetectionMainWindow::distanceBetweenPoints(cv::Point2f a, cv::Point2f b)
{
    return sqrt( (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) );
}

Point2f RobotDetectionMainWindow::scaleToGui(Point2f srcDot)
{
    return Point2f( (srcDot.x / GUI_SCALING) , (srcDot.y / GUI_SCALING) );
}

Point3f RobotDetectionMainWindow::scaleToGui(Point3f srcDot)
{
    return Point3f( (srcDot.x / GUI_SCALING) , (srcDot.y / GUI_SCALING) , (srcDot.z / GUI_SCALING));
}

double RobotDetectionMainWindow::scaleToGui(double value)
{
    return (value / GUI_SCALING);
}

void RobotDetectionMainWindow::readXmlCalibrationFile()
{
    QString fileName = "CalibrationData.xml";
    QFile* file = new QFile(fileName);

    // If file can't be found, choose other file.
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,
                             "File Error",
                             "Couldn't find CalibrationData.xml. Please choose calibration file.",
                             QMessageBox::Ok);
        fileName = QFileDialog::getOpenFileName(this, "Load Calibration File", "", "Calibration Files (*.xml)");
        file = new QFile(fileName);

        // If file still can't be opened, show an error message.
        if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::critical(this,
                                  "File Error",
                                  "Couldn't open XML File",
                                  QMessageBox::Ok);
            return;
        }
    }

    QXmlStreamReader xmlReader(file);
    QList< QMap<QString, QXmlStreamAttributes> > cameraList;

    // parse until end of file or until error.
    while(!xmlReader.atEnd() && !xmlReader.hasError())
    {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument)
        {
            continue;
        }
        if(token == QXmlStreamReader::StartElement)
        {
            // CameraCalibrationData has no data for itself
            if(xmlReader.name() == "CameraCalibrationData")
            {
                continue;
            }
            // Camera does have the data, so it is parsed
            if(xmlReader.name() == "Camera")
            {
                cameraList.append(this->parseCamera(xmlReader));
            }
        }
    }
    // Error handling
    if(xmlReader.hasError())
    {
        QMessageBox::critical(this, "Parse Error", xmlReader.errorString(), QMessageBox::Ok);
    }
    xmlReader.clear();

    for(int i = 0; i < cameraList.size(); i++)
    {
        QXmlStreamAttributes attribCamera, attribMatrix, attribLensDist, attribTrafoMatrix, attribSettings;
        attribCamera = cameraList.at(i)["Camera"];
        attribMatrix = cameraList.at(i)["Matrix"];
        attribLensDist = cameraList.at(i)["LensDistortion"];
        attribTrafoMatrix = cameraList.at(i)["TransformationMatrix"];
        attribSettings = cameraList.at(i)["Settings"];

        // build cameraMatrix
        cameraMatrix.insert(i, Mat::eye(3, 3, CV_64F));
        cameraMatrix[i].at<double>(0, 0) = attribMatrix.value("fx").toDouble();
        cameraMatrix[i].at<double>(0, 2) = attribMatrix.value("cx").toDouble();
        cameraMatrix[i].at<double>(1, 1) = attribMatrix.value("fy").toDouble();
        cameraMatrix[i].at<double>(1, 2) = attribMatrix.value("cy").toDouble();

        // build distCoeffs
        distCoeffs.insert(i, Mat::zeros(8, 1, CV_64F));
        distCoeffs[i].at<double>(0) = attribLensDist.value("k1").toDouble();
        distCoeffs[i].at<double>(1) = attribLensDist.value("k2").toDouble();
        distCoeffs[i].at<double>(2) = attribLensDist.value("p1").toDouble();
        distCoeffs[i].at<double>(3) = attribLensDist.value("p2").toDouble();
        distCoeffs[i].at<double>(4) = attribLensDist.value("k3").toDouble();

        // build perspective transformation matrix
        perspTransfMatrix.insert(i, Mat::eye(3, 3, CV_64F));
        perspTransfMatrix[i].at<double>(0, 0) = attribTrafoMatrix.value("t1").toDouble();
        perspTransfMatrix[i].at<double>(0, 1) = attribTrafoMatrix.value("t2").toDouble();
        perspTransfMatrix[i].at<double>(0, 2) = attribTrafoMatrix.value("t3").toDouble();
        perspTransfMatrix[i].at<double>(1, 0) = attribTrafoMatrix.value("t4").toDouble();
        perspTransfMatrix[i].at<double>(1, 1) = attribTrafoMatrix.value("t5").toDouble();
        perspTransfMatrix[i].at<double>(1, 2) = attribTrafoMatrix.value("t6").toDouble();
        perspTransfMatrix[i].at<double>(2, 0) = attribTrafoMatrix.value("t7").toDouble();
        perspTransfMatrix[i].at<double>(2, 1) = attribTrafoMatrix.value("t8").toDouble();
        perspTransfMatrix[i].at<double>(2, 2) = attribTrafoMatrix.value("t9").toDouble();

        // build settings
        exposureValue.insert(i, attribSettings.value("exp").toInt());
        contrastValue.insert(i, attribSettings.value("cnt").toInt());
        brightnessValue.insert(i, attribSettings.value("brg").toInt());
    }

    file->close();
    ui->statusBar->showMessage("XML-File was read successfully...", 2000);
}

QMap<QString, QXmlStreamAttributes> RobotDetectionMainWindow::parseCamera(QXmlStreamReader& xmlReader)
{
    QMap<QString, QXmlStreamAttributes> camera;
    if(xmlReader.tokenType() != QXmlStreamReader::StartElement && xmlReader.name() == "Camera")
    {
        return camera;
    }
    // read the attributes for Camera
    QXmlStreamAttributes attributes = xmlReader.attributes();
    // check if Camera has id attribute and add attributes to the map
    if(attributes.hasAttribute("id"))
    {
        camera.insert("Camera", xmlReader.attributes());
    }
    // Next element...
    xmlReader.readNext();

    // read until endElement by the name "Camera" occurs
    while(!(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "Camera"))
    {
        if(xmlReader.tokenType() == QXmlStreamReader::StartElement)
        {
            // elements by the name "Matrix", "LensDistortion" or "Settings" are added
            camera.insert(xmlReader.name().toString(), xmlReader.attributes());
        }
        xmlReader.readNext();
    }
    return camera;
}
