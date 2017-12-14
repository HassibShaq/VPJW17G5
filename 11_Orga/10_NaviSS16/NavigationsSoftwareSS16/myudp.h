#ifndef MYUDP_H
#define MYUDP_H

#include <QObject>
#include <QUdpSocket>
#include <QTime>
#include <opencv2/opencv.hpp>
#include <QBitArray>
#include <QDataStream>
#include <QFile>
#include <QTextStream>

class MyUDP : public QObject
{
    Q_OBJECT
public:
    explicit MyUDP(QObject *parent = 0);
    void sendUdpData(QList<cv::Point3f> robotLocations, QTime timeStamp);
    void setSendConfig( QString SendToIp, int SendToPort);
    void setReciveConfig(QString ReciveFromIp,int ReciveFromPort);
    void setSendConfig_SyncService (QString SendToIp_SyncService, int SendToPort_SyncService);
    void setReciveConfig_SyncService(QString ReciveFromIp_SyncService,int ReciveFromPort_SyncService);
    bool getconfiginformation();
    bool getsyncconfiginformation();
    void send_ID4 (double recived_Robot_no, double timeStamp_t4);

    double get_MSsMN ();
    double calc_MSsMN (QTime timeStamp);

    QBitArray get_Robots_Sync_Status ();


    double msg_4;


signals:

public slots:
    void readReady();
    void readReady_SyncService();

private:

    QTime timeStamp;

    //UDP-Sockets
    QUdpSocket udpsocket_send;
    QUdpSocket udpsocket_recive;

    QUdpSocket udpsocked_send_SyncService;
    QUdpSocket udpSocked_recive_SyncService;
    bool recive_socket_open;
    bool recive_socket_open_SyncService;
    bool write_diff_log_file;

    bool start_log;
    double timestamp_start_log;
    double timestamp_stop_log;



    QString sendIP;
    int sendPort;
    QString sendIP_SyncService;
    int sendPort_SyncService;

    QString reciveIP;
    int recivePort;
    QString reciveIP_SyncService;
    int recivePort_SyncService;


    //Sync-Handling
    QBitArray Robots_Sync_Status;

    //UDP-Variables
    double send_data_time_sync [8];
    double robotID[8];
    double robots_t4_stamps [8];


};

#endif // MYUDP_H
