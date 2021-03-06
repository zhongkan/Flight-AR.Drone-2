#include <iostream>
#include "..//common/ardrone.h"
#include "..//common/functional.h"

ARDrone::ARDrone(const QString ipAddr /* = "192.168.1.1" */, QObject *parent /* = 0 */)
	:QThread(parent)
{
    qRegisterMetaType<string>("std::string");
    qRegisterMetaType<RESULT>("RESULT");
    this->_seq = 1;
    this->_ipAddrStr = ipAddr;
    this->_stop = false;
}

ARDrone::~ARDrone()
{
    delete this->_ATSocket;
    delete this->_ipAddr;
}

void ARDrone::ardroneInit()
{
    string cmd;
    RESULT res = A_OK;

    this->print("Initialize the ardrone...");

	/* Send undocumented command */
    cmd = ctos("AT*PMODE=") + itos(this->getSeqNum()) + ctos(",2");
    res &= this->sendAT_CMD(cmd);
    msleep(INTERVAL);

	/* Send undocumented command */
    cmd = ctos("AT*MISC=") + itos(this->getSeqNum()) + ctos(",2,20,2000,3000");
    res &= this->sendAT_CMD(cmd);
    msleep(INTERVAL);

	/* Send flat trim */
    cmd = "AT*FTRIM=" + itos(this->getSeqNum());
    res &= this->sendAT_CMD(cmd);
    msleep(INTERVAL);

	/* Send ref command */
    cmd = "AT*REF=" + itos(this->getSeqNum()) + ",290717696";
    res &= this->sendAT_CMD(cmd);
    msleep(INTERVAL);

	/* Set the configuration IDs */
	res &= this->sendf("AT*CONFIG_IDS=%d,\"%s\",\"%s\",\"%s\"\r", getSeqNum(), ARDRONE_SESSION_ID, ARDRONE_PROFILE_ID, ARDRONE_APPLOCATION_ID);
	res &= this->sendf("AT*CONFIG=%d,\"custom:session_id\",\"%s\"\r", getSeqNum(), ARDRONE_SESSION_ID);
	msleep(INTERVAL);
	res &= this->sendf("AT*CONFIG_IDS=%d,\"%s\",\"%s\",\"%s\"\r", getSeqNum(), ARDRONE_SESSION_ID, ARDRONE_PROFILE_ID, ARDRONE_APPLOCATION_ID);
	res &= this->sendf("AT*CONFIG=%d,\"custom:profile_id\",\"%s\"\r", getSeqNum(), ARDRONE_PROFILE_ID);
	msleep(INTERVAL);
	res &= this->sendf("AT*CONFIG_IDS=%d,\"%s\",\"%s\",\"%s\"\r", getSeqNum(), ARDRONE_SESSION_ID, ARDRONE_PROFILE_ID, ARDRONE_APPLOCATION_ID);
	res &= this->sendf("AT*CONFIG=%d,\"custom:application_id\",\"%s\"\r", getSeqNum(), ARDRONE_APPLOCATION_ID);
	msleep(INTERVAL);

	/* Set video codec */
	res &= this->sendf("AT*CONFIG_IDS=%d,\"%s\",\"%s\",\"%s\"\r", getSeqNum(), ARDRONE_SESSION_ID, ARDRONE_PROFILE_ID, ARDRONE_APPLOCATION_ID);
	res &= this->sendf("AT*CONFIG=%d,\"video:video_codec\",\"%d\"\r", getSeqNum(), H264_360P_CODEC);
	msleep(INTERVAL);

	/* Set to default video channel */
	res &= this->sendf("AT*CONFIG_IDS=%d,\"%s\",\"%s\",\"%s\"\r", getSeqNum(), ARDRONE_SESSION_ID, ARDRONE_PROFILE_ID, ARDRONE_APPLOCATION_ID);
	res &= this->sendf("AT*CONFIG=%d,\"video:video_cha  nnel\",\"0\"\r", getSeqNum());

    if (M_SUCCEEDED(res))
    {
		/* start navdata thread and video thread */
        emit this->navStart();
		emit this->videoStart();
    }
    else
	{
		/* notify main window, initialize failed */
        emit this->ardroneInitRes(ARDRONE_INIT_FAILED);
	}
}

RESULT ARDrone::sendf(char *str, ...)
{
	char *arg;
	char msg[1024];

	/* Apply format */
	va_start(arg, str);
	vsprintf_s(msg, sizeof(msg), str, arg);
	va_end(arg);
	#ifdef DEBUG_NAVDATA_C
	print(msg);
	#endif
	string cmd(msg);
	return this->sendAT_CMD(cmd);
}


RESULT ARDrone::configure(const string attribute, const string value)
{
    string cmd;
    RESULT res;

    cmd = "AT*CONFIG=" + itos(this->getSeqNum()) +
            + ",\"" + attribute + "\",\"" + value + "\"";
    res = this->sendAT_CMD(cmd);
    #ifdef DEBUG_NAVDATA_C
    this->print(cmd, res);
    #endif
    return res;
}

RESULT ARDrone::sendAT_CMD(string cmd)
{
    cmd = cmd + "\r";
    QMutexLocker locker(&this->_mSend);
    QByteArray datagram = QByteArray(cmd.c_str());
    qint64 cnt = this->_ATSocket->writeDatagram(datagram, *(this->_ipAddr), AT_CMD_PORT);
    if (-1 == cnt)
    {
        return A_ERROR;
    }
    else if (cnt < datagram.size())
    {
        return A_FAILED;
    }
    else
    {
        return A_OK;
    }
}

/*
 * drone left-right tilt - floating-point value in range [−1..1]
 * drone front-back tilt - floating-point value in range [−1..1]
 * drone vertical speed  - floating-point value in range [−1..1]
 * drone angular speed   - floating-point value in range [−1..1]
 */
RESULT ARDrone::sendPCMD(int enable, float roll, float pitch, float gaz, float yaw)
{
    char buf[1024];
	RESULT res;
	float_to_int_u convert[4];
    convert[0].f_value = roll;
    convert[1].f_value = pitch;
    convert[2].f_value = gaz;
    convert[3].f_value = yaw;

    sprintf(buf, "AT*PCMD=%d,%d,%d,%d,%d,%d",
            this->getSeqNum(),
            enable,
            convert[0].i_value,
            convert[1].i_value,
            convert[2].i_value,
            convert[3].i_value);

    string cmd(buf);
    res = this->sendAT_CMD(cmd);
    return res;
}

RESULT ARDrone::sendPCMD_MAG(int enable, float roll, float pitch, float gaz, float yaw, float psi, float psi_accuracy)
{
	char buf[1024];
	RESULT res;
	float_to_int_u convert[6];
	convert[0].f_value = roll;
	convert[1].f_value = pitch;
	convert[2].f_value = gaz;
	convert[3].f_value = yaw;
	convert[4].f_value = psi;
	convert[5].f_value = psi_accuracy;

	sprintf(buf, "AT*PCMD_MAG=%d,%d,%d,%d,%d,%d,%d,%d",
		this->getSeqNum(),
		enable,
		convert[0].i_value,
		convert[1].i_value,
		convert[2].i_value,
		convert[3].i_value,
		convert[4].i_value,
		convert[5].i_value);

	string cmd(buf);
	res = this->sendAT_CMD(cmd);
	return res;
}


int ARDrone::getSeqNum()
{
    QMutexLocker locker(&this->_mSeq);
    return this->_seq++;
}

void ARDrone::go()
{
	this->_stop = false;
	this->start();
}

void ARDrone::stop()
{
    this->_stop = true;
}

void ARDrone::print(const char *mesg)
{
    emit this->mesgShow(QString::fromLocal8Bit(mesg));
}

void ARDrone::print(const char *mesg, const RESULT res)
{
    emit this->mesgShow(QString::fromLocal8Bit(mesg), res);
}

void ARDrone::print(const QString mesg)
{
    emit this->mesgShow(mesg);
}

void ARDrone::print(const QString mesg, const RESULT res)
{
    emit this->mesgShow(mesg, res);
}

void ARDrone::print(const string mesg)
{
    emit this->mesgShow(QString::fromStdString(mesg));
}

void ARDrone::print(const string mesg, const RESULT res)
{
    emit this->mesgShow(QString::fromStdString(mesg), res);
}

void ARDrone::run()
{
    this->_ipAddr = new QHostAddress(this->_ipAddrStr);
    this->_ATSocket = new QUdpSocket();
    this->_ATSocket->bind(AT_CMD_PORT);
    this->ardroneInit();

    string cmd;
    while (!this->_stop)
    {
        cmd = ctos("AT*COMWDG=") + itos(this->getSeqNum());
        this->sendAT_CMD(cmd);
        msleep(100);
    }
}

void ARDrone::startUp()
{
    this->go();
}

void ARDrone::shutDown()
{
    this->_seq = 0;
    emit this->navStop();
	emit this->videoStop();
	this->stop();
    emit this->mesgShow("AR.Drone disconnected");
}
