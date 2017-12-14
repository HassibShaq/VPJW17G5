#include "robotdetectionmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RobotDetectionMainWindow w;
    w.show();

    return a.exec();
}
