#ifndef CONSTANTS_H
#define CONSTANTS_H

#define CAMERA_IMG_WIDTH  640
#define CAMERA_IMG_HEIGTH 480
#define FIELD_WIDTH  8000  // millimeters
#define FIELD_HEIGTH 4000  // millimeters
#define GUI_WIDTH 1280
#define GUI_HEIGTH 640
#define GUI_SCALING  6.25  // = 8000mm / 1280pixel

#define NR_OF_CAMS 6
#define MAX_NR_OF_ROBOTS 4
#define ROBOT_RADIUS 180.0 // millimeters

#define COLOR_RED        cv::Scalar(  0,  0,255)
#define COLOR_GREEN      cv::Scalar(  0,255,  0)
#define COLOR_BLUE       cv::Scalar(255,  0,  0)
#define COLOR_YELLOW     cv::Scalar(  0,255,255)
#define COLOR_CYAN       cv::Scalar(255,255,  0)
#define COLOR_MAGENTA    cv::Scalar(255,  0,255)
#define COLOR_WHITE      cv::Scalar(255,255,255)
#define COLOR_BLACK      cv::Scalar(  0,  0,  0)
#define COLOR_ORANGE     cv::Scalar(  0,165,255)
#define COLOR_DARK_GREY  cv::Scalar(120,120,120)
#define COLOR_LIGHT_GREY cv::Scalar(220,220,220)

#endif // CONSTANTS_H
