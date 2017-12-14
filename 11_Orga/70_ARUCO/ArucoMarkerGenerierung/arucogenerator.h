#ifndef ARUCOGENERATOR_H
#define ARUCOGENERATOR_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

class ArucoGenerator
{
public:
    ArucoGenerator();
    void setBaseDict(cv::Ptr<cv::aruco::Dictionary> BaseDict);
    cv::Ptr<cv::aruco::Dictionary> getBaseDict();

    cv::Ptr<cv::aruco::Dictionary> getGeneratedDict();
    bool generateDict();

    cv::Ptr<cv::aruco::Dictionary> saveGeneratedDict(QString DictPath);

    static cv::Ptr<cv::aruco::Dictionary> loadDict(QString DictPath);
    static cv::Ptr<cv::aruco::Dictionary> saveDict(QString DictPath);

private:
    cv::Ptr<cv::aruco::Dictionary> BaseDict;
    cv::Ptr<cv::aruco::Dictionary> GeneratedDict;
    QString BaseDictPath;
    int MarkerCount;
    int MarkerSizeBit;
    int MarkerSizePixel;
};

#endif // ARUCOGENERATOR_H
