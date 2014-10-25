/*****************************************************************************
 * @file    navData.h
 * @author  kzhong1991<kzhong1991@gmail.com>
 * @date    2012-12-11
 * @brief   This thread is used to receive navigation data send by ardrone
 *****************************************************************************/

#ifndef NAVDATA_H
#define NAVDATA_H

#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include "..//common/ardrone.h"
#include "..//common/functional.h"
#include "..//common/config.h"
#include "..//navdata/navdata_common.h"

#define ardrone_navdata_unpack( navdata_ptr, option ) (navdata_option_t*) navdata_unpack_option( (quint8*) navdata_ptr, \
                                                                                         navdata_ptr->size,              \
                                                                                         (quint8*) &option,             \
                                                                                         sizeof (option) )

class navData : public QThread
{
    Q_OBJECT

public:
    navData(ARDrone * ardrone, const QString ipAddr = "192.168.1.1", QObject *parent = 0);
    ~navData();

    /* initialize the navigation data stream */
    RESULT initNavData();

    /* get the 32 bit status from the navdata */
    quint32 getState(QByteArray byteArray);

    /* Disassembles a buffer of received navdata, and dispatches it inside 'navdata_unpacked' structure */
    RESULT ardrone_navdata_unpack_all(navdata_unpacked_t* navdata_unpacked, navdata_t* navdata, quint32* cks);

    /*Extract an 'option' from the navdata network packet sent by the drone*/
    quint8* navdata_unpack_option( quint8* navdata_ptr, quint32 ptrsize, quint8* data, quint32 datasize );

    /* make sure the data udp packet is navdata datagram*/
    bool isNavPkt(const QByteArray datagram);

    /* start / stop the thread */
	void go();
    void stop();

	/* Print message to the main window */
    void print(const string mesg);
    void print(const string mesg, const RESULT res);

    void print(const QString mesg);
    void print(const QString mesg, const RESULT res);

    void print(const char * mesg);
    void print(const char * mesg, const RESULT res);

    quint32 get_battery();
    float   get_pitch();
    float   get_roll();
    float   get_yaw();
    qint32   get_altitude();
    float   get_vx();
    float   get_vy();
    float   get_vz();
    quint32 get_ctrl_state();
    bool    isEmergency();

private:
    bool                _stop;
	
	/* Ardrone ip address */
    QHostAddress *      _ipAddr;
    QString             _ipAddrStr;

	/* Use UDP to receive navigation data */
    QUdpSocket *        _navSocket;
	QByteArray          _datagram;

    ARDrone *           _ardrone;

	/* navdata */
    navdata_t *         _navdata;
    navdata_unpacked_t  _navdata_unpacked;

    bool                _setup;

protected:
    void run();

signals:

	/* notify the main window, ardrone initialize ok / failed */
	void initNavStreamRes(int res);

	/* notify the nav_update thread to start/stop */
	void startNavUpdate();
	void stopNavUpdate();

private slots:
	/* receive signal from thread ardrone, start / stop navdata thread */
    void navStart();
    void navStop();
};

#endif // NAVDATA_H
