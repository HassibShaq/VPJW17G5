/****************************************************************************
 * In this File
 *
 * Filename: imgtask.cpp
 * Author:   Markus Baden, Mike Zander
 * Revised by: Jochen Maaß
 * Created:  2014-03-27
 * Changed:  2015-10-14
 * Changed:  2016-04-12 : Extend by ARUCO
 *
 ***************************************************************************/

#include "imgtask.h"
using namespace cv;

bool operator< (const RobotMarker & lhs, const RobotMarker & rhs)
{
    return lhs.id < rhs.id;
}

void ImgTask::run()
{
    if(image.cols < 1)
    {
        return;
    }

    //VARIABLES
    int t = 0;                                                  //Variable for Index of Vectors

    double tempangle;                                           //Contains the temporary Angle
    double angleMiddleValue;                                    //Contains the Middle Value of all Angles (resulted- Angle)

    cv::Point2f position;                                       //Contains the Position of Roboter (x,y)

    QList<double> angles;                                       //Vector with all Angles
    std::vector<int> markerIds;                                 //Vector with all detected Marker ID´s
    std::vector<int> tempmarkerIds;                             //Vector with all detected Marker ID´s
    std::vector< std::vector<Point2f> > markerCorners;          //Vector contains the Cornerposition of detected Marker in Order by Vector Marker Corners
    std::vector< std::vector<Point2f> > tempmarkerCorners;      //Vector contains the Cornerposition of detected Marker in Order by Vector Marker Corners
    std::vector< std::vector<Point2f> > rejectedCandidates;     //Contains the Position of rejectedCandidates

    std::vector<std::vector<Point> > contours;
    std::vector<Vec4i> hierarchy;

    //FUNCTION
    //Use Aruco Dictionary (Original standard Dictionary) and Parameters
    aruco::Dictionary dictionary = aruco::getPredefinedDictionary(aruco::DICT_ARUCO_ORIGINAL);

    //Clear Variables for this run
    detectedRobots.clear();

    QList<RobotMarker> markerList;

    Mat workImage = image.clone();

    cvtColor(workImage, workImage, CV_BGR2GRAY);

    //Thresholding

    threshold(workImage, workImage, threshold_max, 200, THRESH_TOZERO);//Definiert den Schwellwert auf 200

    //Find Contours
    findContours(workImage, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    Rect rect;
    Mat roi;
    for( unsigned int i =0;i< contours.size();i++)
    {
        //Build Rect´s (AOI´s)
        rect = boundingRect(contours[i]);

        //Filter the Rect´s by size
        if(rect.size().area() > 1000 && rect.size().area() < MinSizeofRects)
        {
            //Transfer relevant AOI´s to ARUCO Detection
            roi = cv::Mat(image, rect).clone();
            cv::aruco::detectMarkers(roi, dictionary, tempmarkerCorners, tempmarkerIds, arucoParameters, rejectedCandidates);
            cv::rectangle(image,rect,COLOR_BLUE, 1, CV_AA);

            //Safe detected Marker´s
            for( unsigned int a = 0; a < tempmarkerCorners.size();a++)
            {
                for(int b = 0; b < 4; b++)
                {
                    tempmarkerCorners[a][b].x = tempmarkerCorners[a][b].x + rect.x;
                    tempmarkerCorners[a][b].y = tempmarkerCorners[a][b].y + rect.y;
                }
            }

            markerCorners.insert(markerCorners.end(), tempmarkerCorners.begin(), tempmarkerCorners.end());
            markerIds.insert(markerIds.end(), tempmarkerIds.begin(), tempmarkerIds.end());

         }
    }

    for (unsigned int i=0; i< markerIds.size(); i++)
    {
        RobotMarker marker;
        marker.id = markerIds[i];
        marker.cornerPoints.append(stdVectorToPointlist(markerCorners[i]));
        markerList.append(marker);
    }

    //Sort the ID´s and Corners in Order by ID´s
    qSort(markerList);

    for(int j = 0; j < markerList.size(); j++ )
    {
        std::vector<Point2f> tempVec, tempVec2;
        tempVec = PointlistToStdVector(markerList.at(j).cornerPoints);

        // APPLY CAMERA CALIBRATION DATA
        // undistortPoints needs cameraMatrix twice (second time as projection matrix P)
        // otherwise the destination points are not calculated correctly
        // this is not a bug in OpenCV ast thought, but rather a poorly documented function
        undistortPoints(tempVec, tempVec2, cameraMatrix, distCoeffs, noArray(), cameraMatrix);

        // Apply perspective Transformation
        perspectiveTransform(tempVec2, tempVec, perspTransfMatrix);     //Umrechnung auf das große Feld!

        markerList[j].warpedCornerPoints.append(stdVectorToPointlist(tempVec));
    }

    if (liveViewMode)
    {
        //Draw the detected Marker
        aruco::drawDetectedMarkers(image, markerCorners, markerIds);

        // Undistort Image
        Mat temp;
        undistort(image, temp, cameraMatrix, distCoeffs);

        // Apply perspective Transformation
        warpPerspective(temp, warpedImage, guiTransfMatrix, Size(GUI_WIDTH, GUI_HEIGTH), INTER_NEAREST, BORDER_CONSTANT, 0);
    }

    //CALCULATE ANGLE AND POSITION OF ALL DETECTED MARKER

    //For each detected Marker...
    for(int i = 0;i<markerList.size();i++)
    {
        RobotMarker currentMarker = markerList[i];

        //find the detected ID (Each Robot have two Marker, that are all possible ID´s)
        for( unsigned int a = 0; a<MAX_NR_OF_ROBOTS*2; a++)
        {
            //Current MarkerID is in a
            if(currentMarker.id == a)
            {
                //Calculate the Angle from Marker i and safe it
                tempangle = calculateangle(currentMarker.warpedCornerPoints);
                if(tempangle != 0)
                    angles.append(tempangle);

                //Is the current ID not even, check the second (paired) marker
                //Note the Order for the ID´s on the Robot (See Report).
                if(a % 2 != 0)
                {
                    //If the second marker (paired from even Marker) also exists
                    t = i;
                        if(t-1 >= 0 && markerList[t-1].id == a-1)
                        {
                            //Calculate additionally the Angle between booth marker
                            tempangle = calculateanglebetweenmarker(markerList[i-1].warpedCornerPoints,currentMarker.warpedCornerPoints);
                            angles.append(tempangle);

                            //Calculate the Middlevalue with Prio for Angle between Marker
                            angleMiddleValue = calculatemiddleangle(angles,angles.size());

                            //Change Angle for old System
                            if(angleMiddleValue > 180)
                                angleMiddleValue = -(360-angleMiddleValue);

                            //Calculate the Position of Robot, and Offsets for Marker
                            Pointlist Pointinformation = calculatemiddlepointandOffset(markerList[i-1].warpedCornerPoints, currentMarker.warpedCornerPoints);
                            position = Pointinformation.at(0);

                            //Get the detected Robot ID (not Marker ID)
                            int roboterID = getRobotId(a);

                            //If the Robot with detect Marker exist,
                            if(roboterID != InvalidRobotId)
                            {
                                //Copy detect Marker in detectRobot
                                Point3f coordinates = Point3f(position.x, position.y, angleMiddleValue);
                                RobotPosition temp = {roboterID, coordinates};
                                detectedRobots.append(temp);

                                //If Offset should be set
                                if(calibrateOffset && Pointinformation.size() > 0)
                                {
                                    RobotOffset temp = {roboterID, std::sqrt(Pointinformation.at(1).x*Pointinformation.at(1).x+Pointinformation.at(1).y*Pointinformation.at(1).y), std::sqrt(Pointinformation.at(2).x*Pointinformation.at(2).x+Pointinformation.at(2).y*Pointinformation.at(2).y)};
                                    NewRobotOffsets.append(temp);
                                    RobotOffsets[roboterID] = temp;
                                }
                            }
                        }
                        //Otherwise (The second paired Marker does not exist), Add the Offset to get the correct Position of one Marker
                        else
                        {
                            //Calculate the Middlevalue
                            angleMiddleValue = calculatemiddleangle(angles,0);

                            //Get the detected Robot ID (not Marker ID)
                            int roboterID = getRobotId(a);

                            //Calculate the Position of Robot only with one Marker and a Offset
                            position = calculatemiddlepoint(currentMarker.warpedCornerPoints,RobotOffsets.at(roboterID).offsetMarkerNotEven,angleMiddleValue);

                            //Change Angle for old System
                            if(angleMiddleValue > 180)
                                angleMiddleValue = -(360-angleMiddleValue);

                            //If the Robot with detect Marker exist,
                            if(roboterID != InvalidRobotId)
                            {
                                //Copy detect Marker in detectRobot
                                Point3f coordinates = Point3f(position.x, position.y, angleMiddleValue);
                                RobotPosition temp = {roboterID, coordinates};
                                detectedRobots.append(temp);
                            }
                        }

                    //Clear the temporary Variables for next two Markers
                    angles.clear();
                    angleMiddleValue = 0;
                }
                //If the current ID even, calculate Position with Offset
                //and clear the Variables for next two Marker.
                else if((markerList.size() > i+1 && markerList[i+1].id != a+1) || (markerList.at(markerList.size() - 1).id == a))
                {
                    //Calculate the Middlevalue
                    angleMiddleValue = calculatemiddleangle(angles,0);

                    //Get the detected Robot ID (not Marker ID)
                    int roboterID = getRobotId(a);

                    //Calculate the Position of Robot only with one Marker and a Offset
                    position = calculatemiddlepoint(currentMarker.warpedCornerPoints,RobotOffsets.at(roboterID).offsetMarkerEven,angleMiddleValue);

                    //Change Angle for old System
                    if(angleMiddleValue > 180)
                        angleMiddleValue = -(360-angleMiddleValue);

                    //If the Robot with detect Marker exist,
                    if(roboterID != InvalidRobotId)
                    {
                        //Copy detect Marker in detectRobot
                        Point3f coordinates = Point3f(position.x, position.y, angleMiddleValue);
                        RobotPosition temp = {roboterID, coordinates};
                        detectedRobots.append(temp);
                    }

                    //Clear the temporary Variables for next two Markers
                    angles.clear();
                    angleMiddleValue = 0;
                }
            }
        }
    }
}

void ImgTask::setImage(cv::Mat image)
{
    this->image = image;
}

void ImgTask::setCameraMatrix(cv::Mat cameraMatrix)
{
    this->cameraMatrix = cameraMatrix;
}

void ImgTask::setDistCoeffs(cv::Mat distCoeffs)
{
    this->distCoeffs = distCoeffs;
}

void ImgTask::setPerspTransfMatrix(cv::Mat perspTransfMatrix)
{
    this->perspTransfMatrix = perspTransfMatrix;

    // calculate GUI Transformation Matrix by scaling down perspTransfMatrix
    Mat scaleMatrix = Mat::zeros(3, 3, CV_64F);
    scaleMatrix.at<double>(0, 0) = 1.0 / GUI_SCALING;
    scaleMatrix.at<double>(1, 1) = 1.0 / GUI_SCALING;
    scaleMatrix.at<double>(2, 2) = 1.0;
    guiTransfMatrix = scaleMatrix * perspTransfMatrix;
}

void ImgTask::setDebugMode(bool debugMode)
{
    this->liveViewMode = debugMode;
}
void ImgTask::setCalibrateOffset(bool  setValue)
{
    calibrateOffset = setValue;
}

QList<RobotOffset> ImgTask::getNewRobotOffsets()
{
    return this->NewRobotOffsets;
}

void ImgTask::setRobotOffsets(QList<RobotOffset> offsets)
{
    this->RobotOffsets = (offsets);
}

void ImgTask::setArucoParameters(cv::aruco::DetectorParameters arucoParameters)
{
    this->arucoParameters = arucoParameters;
}

void ImgTask::setthreshold(int threshold)
{
    this->threshold_max = threshold;
}

void ImgTask::setMinSizeofRects(int minSizeofRects)
{
    this->MinSizeofRects = minSizeofRects;
}

void ImgTask::clearNewRobotOffsets()
{
    this->NewRobotOffsets.clear();
}

double ImgTask::calculateangle(Pointlist marker)
///This Function calculate the Angle between four Corners of marker.
///The calculation occurs clockwise and compute the middlevalue of all four Corners. See also in the documentation.
{
    //VARIABLES
    double tempangle = 0.0;             //Temporary Angle for Loops
    double angle = 0.0;                 //Return Value
    std::vector<double> delta(4,0.0);   //Difference between corners. x. y
    QList<double> angles;               //Vector with all calculated Angles
    int a = 3;                          //a contain the Index for Vector for the second Corner. For example:
                                        //for the Angle between Corner 0 and 1 is i=0, and a=3! Default Value is 3

    //FUNCTION
    //Calculate Angle based on the Quadrant of Angles
    for(int i =0;i<4;i++)
    {

        //for Angle between Corner 0->1 and 2->3
        if(i!=0)
        {
            a = -1;
        }

        //Calculate Angle in °
        delta[i] = -marker[i+a].x + marker[i].x;
        delta[i+a] = -marker[i].y + marker[i+a].y;
        tempangle = atan2(delta[i+a],delta[i])*180/3.14159265359;


        //Offset for the schift between World co-ordinate system and marker co-ordinates.
        if(i==0)
        {
            tempangle = -tempangle;

        }

        if(i==1)
        {
            tempangle = 90-tempangle;
            if(tempangle > 180)
                tempangle = -(360-tempangle);

        }

        if(i==2)
        {
            tempangle = 180-tempangle;
        }

        if(i==3)
        {
            tempangle = -(tempangle+90);
            if(tempangle<-180)
                tempangle = 360+tempangle;
        }

        if(tempangle < 0)
            tempangle = 360 + tempangle;

        //If the Angle is overflowed
        if(tempangle>360)
            tempangle=tempangle-360;

        angles.push_back(tempangle);
    }

    //CHECK AND CALCULATE the middlevalue without Prioritization (second Handing over parametre is Zero)
    angle = calculatemiddleangle(angles, 0);
    return angle;

}

double ImgTask::calculateanglebetweenmarker(Pointlist marker1, Pointlist marker2)
{
    //Variablen deklaration
    double angle = 0.0;                //Angle
    std::vector<double> delta(2,0.0);  //diff. x. y


    //Calculate Angle between the first Corner of booth Markers
    delta[0] = marker1[0].x - marker2[0].x;
    delta[1] = marker2[0].y - marker1[0].y;
    angle = -atan2(delta[1],delta[0])*180/3.14159265359;

    if(angle < 0)
        angle = 360 + angle;

    //If the Angle is overflowed
    if(angle>360)
        angle=angle-360;

    return angle;

}

double ImgTask::calculatemiddleangle(QList<double> angles, int prio)
///It is necessary to prove the Angle in Area between 350° and 10°
///That mean, it can be not guaranteed, that the Angles are all in the same Quadrant->
///The average value is not correct. Example: (0.1°+359°)/2 = 180°
///In this Function, all Transmitted parametres will be check for Deviation .
///It is possible to set a Priority (Correct Angles to alpha>0° or alpha<360°) for a
///determined Value in angles, for this, prio must include the Index+1 of prio in Vector angles.
{
    //VARIABLES
    double angle = 0.0;                 //Calculated angle and Returnvalue
    int n = 0;                          //Contain the Size of Angles

    std::vector<double> smallangles;    //Includes the smaller Angles(alpha<10°)
    std::vector<double> bigangles;      //Includes the bigger Angles (alpha>350°)

    //FUNCTION
    //Sort the Angles
    for(int i =0; i<angles.size();i++)
    {
        //Is the Angle in a difficult Area?
        if(angles[i] < 10)
            smallangles.push_back(angles[i]);

        if(angles[i] > 350)
            bigangles.push_back(angles[i]);

        angle = angle+angles[i];
        n++;
    }

    //Are ALL Angles in a difficult Range?
    if(smallangles.size() + bigangles.size() == angles.size() && bigangles.size() != 0 && smallangles.size() != 0)
    {

        //Reset the Angle
        angle = 0.0;

        //Check Prio
        //That mean, one area is preferable (Example: The Angle between Marker is more certainly than the Angle between Corners of Marker)
        //prio contains the Position of the preferable Angle
        if(prio != 0)
            if(angles[prio-1] < 10)
                prio = 10;
            else if(angles[prio-1] > 350)
                prio = 350;

        //Are more Angles in Area of 0-10°, or Prio is set for minimum
        if((smallangles.size() > bigangles.size() || smallangles.size() == bigangles.size() || prio == 10) && prio != 350)
        {
            //correct the bigger Angles
            for(unsigned int i = 0;i<bigangles.size();i++)
                bigangles[i] = -(360-bigangles[i]);
        }
        else
        {
            //correct the smaller Angles
            for(unsigned int i = 0;i<smallangles.size();i++)
                smallangles[i] = 360+smallangles[i];
        }

        //Put all Angles in Vector bigangles
        bigangles.insert(bigangles.end(),smallangles.begin(),smallangles.end());

        //Calculate the Sum of all Angles
        for(unsigned int i=0;i<bigangles.size();i++)
        {
            angle = angle+bigangles[i];
        }

        //Calculate the average
        angle = angle/n;

        //If the Angle is overflowed (Angle >360)
        if(angle>360)
            angle=angle-360;

        //If the Angle is negativ
        if(angle<0)
            angle=360+angle;

    }
    else angle = angle/n;

    return angle;
}

int ImgTask::getRobotId(int markerID)
{

    if(markerID == 0||markerID == 1)
        return 0;

    if(markerID == 2||markerID == 3)
        return 1;

    if(markerID == 4||markerID == 5)
        return 2;

    if(markerID == 6||markerID == 7)
        return 3;

    return InvalidRobotId;
}

QList<RobotPosition> ImgTask::getdetectRobots()
{
    return detectedRobots;
}

Mat ImgTask::getWarpedImage()
{
    return warpedImage;
}

Pointlist ImgTask::stdVectorToPointlist(std::vector<Point2f> vec)
{
    return Pointlist::fromVector(QVector<Point2f>::fromStdVector(vec));
}

std::vector<Point2f> ImgTask::PointlistToStdVector(Pointlist vec)
{
    return vec.toVector().toStdVector();
}

Point2f ImgTask::calculatemiddlepoint(Pointlist marker1, double offset, double angle)
{
    // VARIABLES
    double angleRAD;                                //Angle in Radiant
    Point2f temp1 = Point2f(0,0);                   //Temporary
    Point2f middlepointMarker1 = Point2f(0,0);      //Vector with calculated Middlepoint for Marker 1

    //Convert from DEG to RAD
    angleRAD = angle*(3.14159265359/180);


    //Calculate Middlevalue for Marker 1
    if(marker1.size()>0)
    {
        for(int i = 0;i<4;i++)
        {
            temp1.x=temp1.x+marker1[i].x;
            temp1.y=temp1.y+marker1[i].y;
        }

        middlepointMarker1.x = temp1.x/4;
        middlepointMarker1.y = temp1.y/4;
    }

    //Add Offset
    middlepointMarker1.x = middlepointMarker1.x + (offset * std::cos(angleRAD));
    middlepointMarker1.y = middlepointMarker1.y + (offset * std::sin(angleRAD));

    return middlepointMarker1;

}

Pointlist ImgTask::calculatemiddlepointandOffset(Pointlist marker1, Pointlist marker2)
///Calculate the Middlepoint (x,y) between two Marker, based on the Corner of booth Marker
{
    // VARIABLES
    Point2f temp1 = Point2f(0,0);                   //Temporary
    Point2f temp2 = Point2f(0,0);                   //Temporary
    Point2f middlepointMarker1 = Point2f(0,0);      //calculated Middlepoint for Marker 1
    Point2f middlepointMarker2 = Point2f(0,0);      //calculated Middlepoint for Marker 2
    Point2f middlepoint = Point2f(0,0);             //Vector with calculated Middlepoint for Robot
    Pointlist Pointinformations;                    //List with: 0:Middlepoint of Robot, 1:Offset for Marker1, 2: Offset for Marker 2 (Returnvalue!)
    int n = 0;                                      //Divisor for Middlevalue

    //Calculate Middlevalue for Marker 1
    if(marker1.size()>0)
    {
        for(int i = 0;i<4;i++)
        {
            temp1.x=temp1.x+marker1[i].x;
            temp1.y=temp1.y+marker1[i].y;
        }
        n++;

        middlepointMarker1.x = temp1.x/4;
        middlepointMarker1.y = temp1.y/4;
    }


    //Calculate Middlevalue for Marker 2
    if(marker2.size()>0)
    {
        for(int i = 0;i<4;i++)
        {
            temp2.x=temp2.x+marker2[i].x;
            temp2.y=temp2.y+marker2[i].y;
        }
        n++;

        middlepointMarker2.x = temp2.x/4;
        middlepointMarker2.y = temp2.y/4;
    }

    //Calculate Middlepoint of Robot
    middlepoint.x = (middlepointMarker1.x+middlepointMarker2.x)/2;
    middlepoint.y = (middlepointMarker1.y+middlepointMarker2.y)/2;
    Pointinformations.append(middlepoint);

    //Calculate Difference between Markers and Middlepoint (Offset), and saf it at Position 1 and 2 in Returnvalue
    Pointinformations.append(Point2f(middlepoint.x-middlepointMarker1.x,middlepoint.y-middlepointMarker1.y));
    Pointinformations.append(Point2f(middlepoint.x-middlepointMarker2.x,middlepoint.y-middlepointMarker2.y));

    return Pointinformations;
}
