/*******************************************
 * @file    ardrone.h
 * @author  kzhong1991<kzhong1991@gmail.com>
 * @date    2012-12-11
 * @brief   Define the ardrone class
 ******************************************/

#ifndef ARDRONE_H
#define ARDRONE_H

#include <string>
#include <windows.h>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtCore/QThread>
#include <QtCore/QByteArray>
#include <QtGui/QTextCursor>
#include "../video/video_codec.h"
#include "functional.h"


Q_DECLARE_METATYPE(string)
Q_DECLARE_METATYPE(RESULT)

using namespace std;

/* Receive nav_data on UPD port 5554 */
#define NAV_DATA_PORT   (5554)

/* Receive video stream on TCP port 5555 */
#define VIDEO_PORT      (5555)

/* Send AT commands on UPD port 5556 */
#define AT_CMD_PORT     (5556)

/*
 * Used to retrieve configuration data, and to acknowledge important information
 * such as the sending of configuration information.
 */
#define CONTROL_PORT    (5559)

/* The interval milliseconds between two AT commands */
#define INTERVAL        (100)

/* Session ID */
#define ARDRONE_SESSION_ID          "d2e081a3"

/* Profile ID */
#define ARDRONE_PROFILE_ID          "be27e2e4"

/* Application ID */
#define ARDRONE_APPLOCATION_ID      "d87f7e0c"

class ARDrone : public QThread{

    Q_OBJECT

public:
    ARDrone(const QString ipAddr = "192.168.1.1", QObject *parent = 0);
    ~ARDrone();

    /* Connect to the ardrone */
    void ardroneInit();

    /* Send the AT commands to ardrone */
    RESULT sendAT_CMD(string cmd);

	/* configure the ardrone */ 
	RESULT   sendf(char *str, ...);  // Send with format
    RESULT configure(const string name, const string value);

    /* move the ARDrone */
    RESULT sendPCMD(int enable, float roll, float pitch, float gaz, float yaw);
	RESULT sendPCMD_MAG(int enable, float roll, float pitch, float gaz, float yaw, float psi, float psi_accuracy);

    /* Return the command sequence */
    int getSeqNum();

    /* start / stop the thread */
	void go();
    void stop();

    void print(const string mesg);
    void print(const string mesg, const RESULT res);
    void print(const QString mesg);
    void print(const QString mesg, const RESULT res);
    void print(const char * mesg);
    void print(const char * mesg, const RESULT res);

private:
    bool                _stop;
    quint64             _seq;
    QString             _ipAddrStr;
    QHostAddress *      _ipAddr;
    QUdpSocket *        _ATSocket;
    QMutex              _mSeq;
    QMutex              _mSend;

protected:
    void run();

signals:
    void mesgShow(const QString cmd, const RESULT res);
    void mesgShow(const QString mesg);

	/* start or stop the navdata thread */
    void navStart();
    void navStop();

	/* start or stop the video thread */
	void videoStart();
	void videoStop();

	/* notify the main window, ardrone initialize failed */
    void ardroneInitRes(int res);

public slots:
    void startUp();
    void shutDown();
};

#endif // ARDRONE_H
