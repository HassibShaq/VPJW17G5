#include "arucogenerator.h"

ArucoGenerator::ArucoGenerator()
{

}


cv::Ptr<cv::aruco::Dictionary> ArucoGenerator::getBaseDict() {
    return this->BaseDict;
}

cv::Ptr<cv::aruco::Dictionary> ArucoGenerator::getGeneratedDict() {
    return this->GeneratedDict;
}

bool ArucoGenerator::generateDict() {

}

cv::Ptr<cv::aruco::Dictionary> saveGeneratedDict(QString DictPath);

static cv::Ptr<cv::aruco::Dictionary> loadDict(QString DictPath);
static cv::Ptr<cv::aruco::Dictionary> saveDict(QString DictPath);
