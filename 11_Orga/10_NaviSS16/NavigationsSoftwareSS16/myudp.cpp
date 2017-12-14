/****************************************************************************
 * This class provides methods for sending robot positions and timestamps
 * via UDP.
 *
 * Filename: myudp.cpp
 * Author:   Markus Baden, Benjamin Willberger
 * Created:  2013-03-14
 * Changed:  2014-04-10
 * Changed:  2016-04-12 : Extend by Clock-Synchronisation
 ***************************************************************************/

#include "myudp.h"

MyUDP::MyUDP(QObject *parent) : QObject(parent)
{
    start_log=0;
    write_diff_log_file = true;
}


//Set configs to send data with the camera-data port
void MyUDP::setSendConfig(QString sendToIp, int sendToPort)
{
    this->sendIP = sendToIp;
    this->sendPort = sendToPort;
}

//Set configs to send data with the SyncService port
void MyUDP::setSendConfig_SyncService(QString sendToIp_SyncService, int sendToPort_SyncService)
{
    this->sendIP_SyncService = sendToIp_SyncService;
    this->sendPort_SyncService = sendToPort_SyncService;
}


//Set configs to recive data with the camera-data port
void MyUDP::setReciveConfig (QString ReciveFromIp,int ReciveFromPort)
{
    this->reciveIP = ReciveFromIp;
    this->recivePort = ReciveFromPort;

    //Bind this socked to recive data
    this->recive_socket_open = udpsocket_recive.bind(QHostAddress(reciveIP),recivePort);

    //Check if the socked is bind correctly to recive data
    if (this->recive_socket_open)
    {
        qDebug()<<"Recive-Socket Camera-Data successfull";
    }
    else
    {
        qDebug() <<"Recive-Socket Camera-Data NOT successfull";
    }


    //Create a Signal/Slot connection between to call a function when data is recived:
    //Signal (readyRead): Occours when new data is recived
    //Slot   (readReady): Function which should be called when the Signal occours
    connect(&udpsocket_recive,
            SIGNAL(readyRead()),
            this,
            SLOT(readReady()));

}


//Set configs to recive data with the SyncService port
void MyUDP::setReciveConfig_SyncService (QString ReciveFromIp,int ReciveFromPort)
{
    this->reciveIP_SyncService = ReciveFromIp;
    this->recivePort_SyncService = ReciveFromPort;

    //Bind this socked to recive data
    this->recive_socket_open_SyncService = udpSocked_recive_SyncService.bind(QHostAddress(reciveIP_SyncService),recivePort_SyncService);

    //Check if the socked is bind correctly to recive data
    if (this->recive_socket_open_SyncService)
    {
        qDebug()<<"Recive-Socket Sync-Service successfull";
    }
    else
    {
        qDebug() << "Recive-Socket Sync-Service NOT successfull";
    }


    //Create a Signal/Slot connection between to call a function when data is recived:
    //Signal (readyRead): Occours when new data is recived
    //Slot   (readReady): Function which should be called when the Signal occours
    connect(&udpSocked_recive_SyncService,
            SIGNAL(readyRead()),
            this,
            SLOT(readReady_SyncService()));

}




// This function builds the telegram which contains the robot positions and some more data.
// Input timeStamp need to be the timeStamp on which the last picture was grabbed.
void MyUDP::sendUdpData( QList<cv::Point3f> robotLocations, QTime timeStamp)
{
    /// All values are converted to datatype double prior to sending. This is to make
    /// sure the Robotinos running Matlab/XPC-Target receive the data correctly.
    ///
    /// double values 1-24 are reserved for a maximum of 8 robot positions
    /// double values 25-27 contains a time stamp
    /// double value 28 contains the time stamp when the last picture was grapped
    /// double values 29-30 --reserve--
    /// double values 31-38 contains ID (for each robot: 31 for robot 1, 32 for robot 2 , ...)
    /// double values 39-55 --reserve--
    /// double value 56 contains a time stamp for the clock synchronization (latest position as possible)

    //VARIABLES
    QByteArray message;
    double x, y, phi,second,minute,hour,MSsMN;
    double reserve = 0.0;


    for(int i = 0; i < 4; i++)
    {
        x = robotLocations.at(i).x;
        y = robotLocations.at(i).y;
        phi = robotLocations.at(i).z / 180.0 * CV_PI; // orientation angle in radiant
        message.append( (const char*) &x, sizeof(double));
        message.append( (const char*) &y, sizeof(double));
        message.append( (const char*) &phi, sizeof(double));
    }

//    for(int i = 0; i < 4; i++)
//    {
//        x = 6666;
//        y = 1903;
//        phi = -1.296; // orientation angle in radiant
//        message.append( (const char*) &x, sizeof(double));
//        message.append( (const char*) &y, sizeof(double));
//        message.append( (const char*) &phi, sizeof(double));
//    }





    /// fill message with more values ("robots 5-8") (to be consistent with the old Matlab solution)
    /// this is used as a special reserve to use more robots
    for(int i = 0; i < 4; i++)
    {
        x = robotLocations.at(i).x;
        y = robotLocations.at(i).y;
        phi = robotLocations.at(i).z / 180.0 * CV_PI; // orientation angle in radiant
        message.append( (const char*) &x, sizeof(double));
        message.append( (const char*) &y, sizeof(double));
        message.append( (const char*) &phi, sizeof(double));
    }

    /// append timestamp to message (double 25-27)
    hour = timeStamp.hour();
    minute = timeStamp.minute();
    second = timeStamp.second() + (0.001 * timeStamp.msec());
    message.append( (const char*) &hour, sizeof(double));
    message.append( (const char*) &minute, sizeof(double));
    message.append( (const char*) &second, sizeof(double));


    /// timestamp of the image for which the postion is send above (double 28)
    double temp_MSsMN = this->calc_MSsMN(timeStamp);
    message.append ( (const char*) &temp_MSsMN, sizeof(double)) ;

    /// fill message with zeroes - - reserve - -
    for (int i = 29; i <= 30; i++)
    {
        message.append( (const char*) &reserve, sizeof(double));
    }

    /// append ID for the robots (double 31-38)
    for (int temp_Robot_no= 1; temp_Robot_no<=8; temp_Robot_no++)
    {
        // Set telegram ID - at the moment only ID 111X is used
        robotID[temp_Robot_no-1] = 1110+temp_Robot_no;

        message.append((const char*) &robotID[temp_Robot_no-1], sizeof(double));
    }

    /// fill message with zeroes - - reserve - -
    for (int i = 39; i <= 55; i++)
    {
        message.append( (const char*) &reserve, sizeof(double));
    }

    /// append time Milliseconds sine Midnight (MSsMN) (double 56)
    MSsMN = this->get_MSsMN();
    message.append((const char*) &MSsMN, sizeof(double));



    /// send message
    udpsocket_send.writeDatagram(message, QHostAddress(sendIP), sendPort);
    //  qDebug () << "SEND:" << message.size() << "Bytes";
    // clean up
    message.clear();


}

void MyUDP::readReady()
{
    // This port is never used for reciving data
}

//This function is called when data is recived on the SyncService socket
void MyUDP::readReady_SyncService()
{
    //VARIABLES
    QByteArray data;
    QByteArray reverse_data;
    QHostAddress sender;
    quint16 port;
    double var_in_1, var_in_2,var_in_3,var_in_4,var_in_5;
    double recived_ID,recived_robot_no =0, recived_data_1,recived_data_2,recived_data_3;
    double timeStamp_t4 = this->get_MSsMN();


    //To save recived data: Resize the QByteArray to the size of the pending data,
    //extract the information out of the data (save data, save sender ip and save the sender port)
    do {
        data.resize(udpSocked_recive_SyncService.pendingDatagramSize());
        udpSocked_recive_SyncService.readDatagram(data.data(),data.size(),&sender, &port);
    } while (udpSocked_recive_SyncService.hasPendingDatagrams());

    //reverse recived data, otherwise Qt can´t interpret the values as doubles.
    reverse_data.resize(data.size());
    for (int i=0; i<data.size(); i++)
    {
        reverse_data[i] = data[data.size()-i-1];
    }

    //Generate a QDataStream with the reverse data
    QDataStream data_in (&reverse_data,QIODevice::ReadOnly);

    //Load data from the QDataStream to variables of the type double
    data_in >> var_in_5 >> var_in_4 >> var_in_3 >> var_in_2 >> var_in_1;

    recived_ID = var_in_1;
    recived_robot_no = var_in_2;
    recived_data_1 = var_in_3;
    recived_data_2 = var_in_4;
    recived_data_3 = var_in_5;


    //    qDebug () <<  reverse_data.size() << "Empfangene Bytes <-> " << reverse_data.size()/8 <<"-Double-Werte";
    //    qDebug () << "1.Variable: "  << var_in_1;  // Enthält die ID
    //    qDebug () << "2.Variable: "  << var_in_2;  // Enthält die Roboter Nummer
    //    qDebug () << "3.Variable: "  << var_in_3;  // Enthält die mitgesendeten Daten i.d.R. Zeit
    //    qDebug () << "4.Variable: "  << var_in_4;
    //    qDebug () << "5.Variable: "  << var_in_5;

    if (start_log==0)
    {
        timestamp_start_log = this->get_MSsMN();
        timestamp_stop_log = this->get_MSsMN() + 10*60*1000; //Test 10minutes
        start_log =1;
        qDebug () << "-------------------------------------INIT MESSUNG ---------------------------------------------";
    }

    if (this->get_MSsMN() <= timestamp_stop_log)
    {

        QString filename = "Diff_time.txt";
        QFile file(filename);
        QTextStream stream(&file);
        file.open(QIODevice::ReadWrite| QIODevice::Text);

        stream << this->get_MSsMN() << "\t" << recived_robot_no << "\t" <<  recived_data_1 << "\t" << recived_data_2 << "\t" << recived_data_3 << endl;

        qDebug () << "Restdauer = " << (timestamp_stop_log-this->get_MSsMN())/1000;

    }

    //Robi aked for latenz check, if ID 333X is recived
    if (recived_ID >= 3330 && recived_ID <=3339)
    {
        //     qDebug () << "recive ID333X, save t4 and send answer to: " << recived_robot_no << "<<-";

        //Send answer telegram to the robot which has asked for latenz check.
        this->send_ID4(recived_robot_no,timeStamp_t4);

    }

}

//Send ID 444X to the robot which has asked for latenz check with ID 333X before.
void MyUDP::send_ID4 (double recived_Robot_no,double timeStamp_t4)
{
    /// All values are converted to datatype double prior to sending. This is to make
    /// sure the Robotinos running Matlab/XPC-Target receive the data correctly.
    ///
    /// double value 1 Telegram ID
    /// double value 2 Contains the time stamp t4
    /// double value 3 --reserve--
    /// double value 4 --reserve--
    /// double value 5 --reserve--

    //VARIABLES
    QByteArray message;
    double sendID;
    double reserve = 0.0;

    ///First double value: ID 444X
    sendID = 4440+recived_Robot_no;
    message.append( (const char*) &sendID, sizeof(double));

    ///Secound double value: TimeStamp t4
    message.append((const char*) &timeStamp_t4, sizeof(double));

    ///Third double value: Reserve
    message.append((const char*) &reserve, sizeof(double));
    ///Four double value: Reserve
    message.append((const char*) &reserve, sizeof(double));
    ///Five double value: Reserve
    message.append((const char*) &reserve, sizeof(double));


    /// send message
    udpsocked_send_SyncService.writeDatagram(message, QHostAddress(sendIP_SyncService), sendPort_SyncService);


    //qDebug () << "ID4 send / TimeStamp t4 send" << message.size();

    msg_4++;
    // clean up
    message.clear();

}



//Generate a timestamp which is the time in millisec since midnight
double MyUDP::get_MSsMN ()
{
    double millisec, hour, minute, second, milli_since_midnight;
    // GRAB TIMESTAMP
    timeStamp = QTime::currentTime(); // read system time

    hour = timeStamp.hour();
    minute = timeStamp.minute();
    second = timeStamp.second();
    millisec = timeStamp.msec();
    milli_since_midnight= millisec+(1000*second)+(60000*minute)+(3600000*hour);

    return milli_since_midnight;
}

//Generate a timestamp out of a given timeStamp which is the time in millisec since midnight
double MyUDP::calc_MSsMN (QTime timeStamp)
{
    double millisec, hour, minute, second, milli_since_midnight;

    hour = timeStamp.hour();
    minute = timeStamp.minute();
    second = timeStamp.second();
    millisec = timeStamp.msec();
    milli_since_midnight= millisec+(1000*second)+(60000*minute)+(3600000*hour);

    return milli_since_midnight;

}

QBitArray MyUDP::get_Robots_Sync_Status ()
{
    return this->Robots_Sync_Status;
}

bool MyUDP::getconfiginformation()
{
    return this->recive_socket_open;
}
bool MyUDP::getsyncconfiginformation()
{
    return this->recive_socket_open_SyncService;
}
