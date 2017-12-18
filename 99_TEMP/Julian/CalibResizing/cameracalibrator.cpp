#include "cameracalibrator.h"

CameraCalibrator::CameraCalibrator() :
    flag(0),
    mustInitUndistort(true),
    distanceHeight(55),
    distanceWidth(55)
{
}


// open circle_grid_board images and extract corner points
int CameraCalibrator::addCirclePoints(const std::vector<Mat>& imageList, cv::Size & boardSize)
{
    // points on the circle_grid_board
    //Output Array für erkannte Kreismittelpunkte
    /* Finds the pattern in the current input. The formation of the equations aims to finding major patterns in the input:
     * in case of the circles, these are the circles themselves.
     * The position of the circles will form the result which will be written into the pointBuf vector.
     */

    vector<Point2f> pointBuf;
    vector<Point3f> circlePoints3d;
    cout << "Abstand zwischen den Mittelpunkten in der Breite: " << distanceWidth << "mm" << endl;
    cout << "Abstand zwischen den Mittelpunkten in der Hoehe: " << distanceHeight << "mm" << endl;
    // 3D Scene Points
    // Initialize the circle_grid_board corners in the circle_grid_board reference frame.
    // The corners are at 3D location (X,Y,Z)= (i,j,0)
    for (int i=0; i<boardSize.height; i++)
        for (int j=0; j<boardSize.width; j++)
            circlePoints3d.push_back(Point3f(j * distanceWidth, i * distanceHeight, 0.0f));
            // Breite: j - distanceWidth
            // Höhe: i - distanceHeight

    // 2D Image Points
    Mat image; // to contain the current circle grid image
    int successes = 0;

    // for all viewpoints
    int listSize = (int) imageList.size();
    for (int i=0; i<listSize; i++)
    {
        // get the image in grayscale
        image = imageList[i];
        //cout << i << "ter Loop" << endl;
        //imshow("Image", image);
        cvtColor(image, image, cv::COLOR_RGB2GRAY);

        bool circlesGridFound = findCirclesGrid(cv::Scalar::all(255) - image, boardSize, pointBuf);

        // get subpixel accuracy on the corners

        cv::cornerSubPix(
                    image,
                    pointBuf,
                    Size(5,5),
                    Size(-1,-1),
                    TermCriteria(
                        TermCriteria::MAX_ITER +
                        TermCriteria::EPS,
                        30,      // max number of iterations
                        0.1) //min accuracy
                    );


        // if we have a good board, add it to our data
        if (pointBuf.size() == (unsigned int) boardSize.area()) {
            // add image and scene points from one view
            // wird hier in der for Schleife für jedes Bild aus der imageList gemacht
            // also insgesamt 4 Vektoren mit den 3D Bildpunkten (circlePoints3d) werden in objectPoints geschrieben
            addPoints(pointBuf, circlePoints3d);
            successes++;
        }
    }
    return successes;
}

// add scene points and corresponding image points
void CameraCalibrator::addPoints(const vector<Point2f>& pointBuf, const vector<Point3f>& circlePoints3d)
{
    // 2D image points from one view
    imagePoints.push_back(pointBuf);
    // corresponding 3D scene points
    objectPoints.push_back(circlePoints3d);
}



//@return
//The function returns the final re-projection error.
// calibrate the camera and returns the re-projection error
double CameraCalibrator::calibrate(Size &imageSize)
{
    // undistorter must be reinitialized
    mustInitUndistort = true;
    //Output rotations and translations vectors
    vector<Mat> rvecs, tvecs;


    //Finds the camera intrinsic and extrinsic parameters from several views of a calibration pattern.
    /*
     * double calibrateCamera(InputArrayOfArrays objectPoints,
     *                        InputArrayOfArrays imagePoints,
     *                        Size imageSize, InputOutputArray cameraMatrix,
     *                        InputOutputArray distCoeffs, OutputArrayOfArrays rvecs,
     *                        OutputArrayOfArrays tvecs, int flags=0,
     *                        TermCriteria criteria=TermCriteria( TermCriteria::COUNT+TermCriteria::EPS, 30, DBL_EPSILON) )
     *
     *
     * The function estimates the intrinsic camera parameters and extrinsic parameters for each of the views.
     * The algorithm is based on [Zhang2000] and [BouguetMCT]. The coordinates of 3D object points and their
     * corresponding 2D projections in each view must be specified. That may be achieved by using an object with a known
     * geometry and easily detectable feature points. Such an object is called a calibration rig or calibration pattern,
     * and OpenCV has built-in support for a chessboard as a calibration rig (see findChessboardCorners() ).
     * Currently, initialization of intrinsic parameters (when CV_CALIB_USE_INTRINSIC_GUESS is not set) is only
     * implemented for planar calibration patterns (where Z-coordinates of the object points must be all zeros).
     * 3D calibration rigs can also be used as long as initial cameraMatrix is provided.

        The algorithm performs the following steps:

        1. Compute the initial intrinsic parameters (the option only available for planar calibration patterns) or read them from the input parameters. The distortion coefficients are all set to zeros initially unless some of CV_CALIB_FIX_K? are specified.
        2. Estimate the initial camera pose as if the intrinsic parameters have been already known. This is done using solvePnP() .
        3. Run the global Levenberg-Marquardt optimization algorithm to minimize the reprojection error, that is, the total sum of squared distances between the observed feature points imagePoints and the projected (using the current estimates for camera parameters and the poses) object points objectPoints. See projectPoints() for details.

        @return
        The function returns the final re-projection error.
     *
     *
    */

    double ret = calibrateCamera(objectPoints, // the 3D points - real world 3D coordinates
                           imagePoints,  // the image points - 2D coordinates (pixel values)
                            imageSize,    // image size
                            cameraMatrix, // output camera matrix
                            distCoeffs,   // output distortion matrix
                            rvecs, tvecs, // Rs, Ts
                            flag);        // set options


    //Verhindern, dass Point Duplikate in den Vektor geschrieben werden.
    //In der Form 1|12|123|1234
    //Am Ende bleiben die Punkte jetzt nur in der Form 1234 im Vektor.
    //Durchgang 1: 1
    //Durchgang 2: 12
    //Durchgang 3: 123
    //Durchgang 4: 1234

    if(imagePoints.size() <= 3)
    {
        imagePoints.clear();
        objectPoints.clear();
    }


    return ret;
}




// remove distortion in the image (after calibration)
Mat CameraCalibrator::remap(const Mat &image)
{
    Mat undistorted;
    if (mustInitUndistort) { // called once per calibration - wird deswegen im Konstruktor initial auf true gesetzt
        initUndistortRectifyMap(
                        cameraMatrix,  // computed camera matrix
                        distCoeffs, // computed distortion matrix
                        Mat(), // optional rectification
                        getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, image.size(), 1, image.size(), 0), // camera matrix to generate undistorted
                            image.size(),  // size of undistorted
                            CV_32FC1,      // type of output map
                            map1, map2);   // the x and y mapping functions
        mustInitUndistort = false;
    }

    // apply mapping functions
    cv::remap(image, undistorted, map1, map2, INTER_LINEAR); // interpolation type

    return undistorted;
}

void CameraCalibrator::writeXML(QString fileName, double exposureValue, double contrastValue, double brightnessValue){

    QFile file(fileName);//Anpassen für Windows
    if(!file.open(QIODevice::WriteOnly)){
        cout << "Failed to open xml file for writing.";
        return;
    }
    else
    {

        QXmlStreamWriter* xmlWriter = new QXmlStreamWriter();
        xmlWriter->setDevice(&file);
        xmlWriter->setAutoFormatting(true);
        xmlWriter->writeStartDocument();
        xmlWriter->writeStartElement("CameraCalibrationData");

        xmlWriter->writeStartElement("Camera");
        xmlWriter->writeAttribute("name", "");
        xmlWriter->writeAttribute("pos", "");
        xmlWriter->writeAttribute("id", QString("%1").arg(selectedCamID));

        xmlWriter->writeStartElement("Matrix");
        xmlWriter->writeAttribute("fx",QString("%1").arg(cameraMatrix.at<double>(0,0)));
        xmlWriter->writeAttribute("cx",QString("%1").arg(cameraMatrix.at<double>(0,2)));
        xmlWriter->writeAttribute("fy",QString("%1").arg(cameraMatrix.at<double>(1,1)));
        xmlWriter->writeAttribute("cy",QString("%1").arg(cameraMatrix.at<double>(1,2)));
        xmlWriter->writeEndElement();

        xmlWriter->writeStartElement("LensDistortion");
        xmlWriter->writeAttribute("k1", QString("%1").arg(distCoeffs.at<double>(0),0,'e',10));
        xmlWriter->writeAttribute("k2", QString("%1").arg(distCoeffs.at<double>(1),0,'e',10));
        xmlWriter->writeAttribute("p1", QString("%1").arg(distCoeffs.at<double>(2),0,'e',10));
        xmlWriter->writeAttribute("p2", QString("%1").arg(distCoeffs.at<double>(3),0,'e',10));
        xmlWriter->writeAttribute("k3", QString("%1").arg(distCoeffs.at<double>(4),0,'e',10));
        xmlWriter->writeEndElement();

        xmlWriter->writeStartElement("Settings");
        xmlWriter->writeAttribute("exp", QString("%1").arg(exposureValue));
        xmlWriter->writeAttribute("cnt", QString("%1").arg(contrastValue));
        xmlWriter->writeAttribute("brg", QString("%1").arg(brightnessValue));
        xmlWriter->writeEndElement();

        xmlWriter->writeEndElement();
        xmlWriter->writeEndElement();

        xmlWriter->writeEndDocument();
        file.close();

        delete xmlWriter;
    }


}


//TODELETE - Kontrollausgabe
Mat CameraCalibrator::getCameraMatrix(){
    return this->cameraMatrix;
}

//TODELETE - Kontrollausgabe
Mat CameraCalibrator::getDistCoeffs(){
    return this->distCoeffs;
}

int CameraCalibrator::getSelectedCamID(){
    return selectedCamID;
}

void CameraCalibrator::setSelectedCamID(int camID){
    selectedCamID = camID;
}

double CameraCalibrator::getCnt(){
    return cnt;
}

double CameraCalibrator::getBrg(){
    return brg;
}

double CameraCalibrator::getExp(){
    return exp;
}

bool CameraCalibrator::resetVals(){
    objectPoints.clear();
    imagePoints.clear();
    cameraMatrix.release();
    distCoeffs.release();
    flag = false;
    mustInitUndistort = true;
    cnt = NULL;
    brg = NULL;
    exp = NULL;
    return 1;
}

void CameraCalibrator::setDistanceWidth(float distanceWidth){
    this->distanceWidth = distanceWidth;
}

void CameraCalibrator::setDistanceHeight(float distanceHeight){
    this->distanceHeight = distanceHeight;
}
