#include "arucoserializer.h"

ArucoSerializer::ArucoSerializer(){}

bool ArucoSerializer::saveArucoDictionary(cv::aruco::Dictionary Dict, QString Path) {
    try {
        cv::FileStorage Storage(Path.toUtf8().constData(), cv::FileStorage::WRITE);
        Storage << "MarkerSize" << Dict.markerSize;
        Storage << "MaxCorrectionBits" << Dict.maxCorrectionBits;
        Storage << "ByteList" << Dict.bytesList;
        Storage.release();
    } catch (...) {
        return false;
    }
    return true;
}

bool ArucoSerializer::drawArucoDictionary(cv::Ptr<cv::aruco::Dictionary> Dict, QString Path, QString NamePrefix, int Size) {
    try {
        cv::Mat Marker;
        cv::aruco::Dictionary* DictObject;
        DictObject = Dict.get();
        //qDebug() << DictObject.bytesList.rows;
        for (int i = 0; i<DictObject->bytesList.rows; i++)
        {
            cv::aruco::drawMarker(Dict, i, Size, Marker);
            cv::imwrite((NamePrefix+QString("%1.png")).arg(i).toUtf8().constData(), Marker);
        }
    } catch (...) {
        return false;
    }
    return true;
}

cv::Ptr<cv::aruco::Dictionary> ArucoSerializer::loadArucoDictionary(QString Path) {
     cv::aruco::Dictionary Dictionary;
    try {
        cv::FileStorage Storage(Path.toUtf8().constData(), cv::FileStorage::READ);
        int MSize, MCBits;
        cv::Mat BytesList;
        Storage["MarkerSize"] >> MSize;
        Storage["MaxCorrectionBits"] >> MCBits;
        Storage["ByteList"] >> BytesList;
        Storage.release();
        Dictionary = cv::aruco::Dictionary(BytesList, MSize, MCBits);
    } catch (...) {
        cv::Ptr<cv::aruco::Dictionary> Empty;
        return Empty;
    }
    return cv::makePtr<cv::aruco::Dictionary>(Dictionary);
}
