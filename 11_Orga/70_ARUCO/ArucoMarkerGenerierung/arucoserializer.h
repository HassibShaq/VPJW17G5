#ifndef ARUCOSERIALIZER_H
#define ARUCOSERIALIZER_H

#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <QObject>

class ArucoSerializer
{
    public:
    bool static saveArucoDictionary(cv::aruco::Dictionary Dict, QString Path);
    bool static drawArucoDictionary(cv::Ptr<cv::aruco::Dictionary> Dict, QString Path, QString NamePrefix, int Size);
    cv::Ptr<cv::aruco::Dictionary> static loadArucoDictionary(QString Path);

    private:
    ArucoSerializer();
};

#endif // ARUCOSERIALIZER_H
