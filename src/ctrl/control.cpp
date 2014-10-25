#include "..//ctrl/control.h"
#include <iostream>

Control::Control(ARDrone * ardrone, QObject *parent) :
    QThread(parent)
{
    this->_ardrone		= ardrone;
    this->_keyboard		= true;
    this->_evt			= NULL;
    this->_speed		= 0.6f;
    this->_ctrl_state	= 131072;
    this->_stop			= false;
	this->_pcmd			= false;
    this->_semaphore	= new QSemaphore(0);
    memset(this->_last_movement, 0, sizeof(this->_last_movement));
    memset(this->_movement, 0, sizeof(this->_movement));
}

Control::~Control()
{

}

void Control::run()
{
    while (!this->_stop)
    {
        this->_semaphore->acquire();
        float roll  = this->_movement[0] * this->_speed;
        float pitch = this->_movement[1] * this->_speed;
        float gaz   = this->_movement[2] * this->_speed;
        float yaw   = this->_movement[3] * this->_speed;

		if (fabs(roll) <= eps && fabs(pitch) <= eps && fabs(gaz) <= eps && fabs(yaw) <= eps)
		{
			if (_pcmd)
			{
				_ardrone->sendPCMD_MAG(1 ,0 ,0 ,0 ,0 ,0 ,0);
				msleep(10);
			}
			_ardrone->sendPCMD_MAG(0 ,0 ,0 ,0 ,0 ,0 ,0);
			_pcmd = false;
			continue;
		}

		if(!_pcmd)
		{
			_ardrone->sendPCMD_MAG(1, 0, 0, 0, 0, 0, 0);
			msleep(10);
		}

		this->_ardrone->sendPCMD_MAG(1, roll, pitch, gaz, yaw, 0, 0);
		_pcmd = true;
    }
}

void Control::takeOff()
{
    string cmd = "AT*REF=" + itos(this->_ardrone->getSeqNum()) + ",290718208";
    this->_ardrone->sendAT_CMD(cmd);
}

void Control::landing()
{
    string cmd = "AT*REF=" + itos(this->_ardrone->getSeqNum()) + ",290717696";
    this->_ardrone->sendAT_CMD(cmd);
}

void Control::emergency()
{
    string cmd = "AT*REF=" + itos(this->_ardrone->getSeqNum()) + ",290717952";
    this->_ardrone->sendAT_CMD(cmd);
}

/* gaz+ */
void Control::goUp(const float speed)
{
	this->_ardrone->sendPCMD_MAG(1, 0, 0, 0, 0, 0, 0);
    this->_ardrone->sendPCMD_MAG(1, 0, 0, speed, 0, 0, 0);
}

/* gaz- */
void Control::goDown(const float speed)
{
	this->_ardrone->sendPCMD_MAG(1, 0, 0, 0, 0, 0, 0);
    this->_ardrone->sendPCMD_MAG(1, 0, 0, -speed, 0, 0, 0);
}

/* roll- */
void Control::goLeft(const float speed)
{
	this->_ardrone->sendPCMD_MAG(1, 0, 0, 0, 0, 0, 0);
    this->_ardrone->sendPCMD_MAG(1, -speed, 0, 0, 0, 0, 0);
}

/* roll+ */
void Control::goRight(const float speed)
{
	this->_ardrone->sendPCMD_MAG(1, 0, 0, 0, 0, 0, 0);
	this->_ardrone->sendPCMD_MAG(1, speed, 0, 0, 0, 0, 0);
}

/* pitch- */
void Control::forward(const float speed)
{
	this->_ardrone->sendPCMD_MAG(1, 0, 0, 0, 0, 0, 0);
    this->_ardrone->sendPCMD_MAG(1, 0, -speed, 0, 0, 0, 0);
}

/* pitch+ */
void Control::backward(const float speed)
{
	this->_ardrone->sendPCMD_MAG(1, 0, 0, 0, 0, 0, 0);
	this->_ardrone->sendPCMD_MAG(1, 0, speed, 0, 0, 0, 0);
}

/* yaw- */
void Control::rotateL(const float speed)
{
	this->_ardrone->sendPCMD_MAG(1, 0, 0, 0, 0, 0, 0);
    this->_ardrone->sendPCMD_MAG(1, 0, 0, 0, -speed, 0, 0);
}

/* yaw+ */
void Control::rotateR(const float speed)
{
	this->_ardrone->sendPCMD_MAG(1, 0, 0, 0, 0, 0, 0);
	this->_ardrone->sendPCMD_MAG(1, 0, 0, 0, speed, 0, 0);
}

void Control::hovering()
{
    this->_ardrone->sendPCMD_MAG(0, 0, 0, 0, 0, 0, 0);
}

void Control::setSpeed(const float speed)
{
    if (speed > 1.0)
    {
        this->_speed = 1.0;
    }
    else if (speed < -1.0)
    {
        this->_speed = -1.0;
    }
    else
    {
        this->_speed = speed;
    }
}

void Control::keyBoardCtrlEnable(const bool enable)
{
    this->_keyboard = enable;
}

void Control::keyPressed(QKeyEvent *evt)
{
    if (!this->_keyboard)
        return;

    if (evt->key() == KEY_TAKEOFF_LAND)
    {
        if (this->_state == CTRL_LANDED)
        {
			while(this->_state == CTRL_LANDED)
			{
				this->takeOff();
			}
		}
		else
		{
			while(this->_state != CTRL_LANDED)
			{
				this->landing();
			}
		}
    }
    else if (evt->key() == KEY_FORWARD)
    {
        this->_movement[1] = -1;
        this->_semaphore->release();
    }
    else if (evt->key() == KEY_BACKWARD)
    {
        this->_movement[1] = 1;
        this->_semaphore->release();
    }
    else if (evt->key() == KEY_GOLEFT)
    {
        this->_movement[0] = -1;
        this->_semaphore->release();
    }
    else if (evt->key() == KEY_GORIGHT)
    {
        this->_movement[0] = 1;
        this->_semaphore->release();
    }
    else if (evt->key() == KEY_GOUP)
    {
        this->_movement[2] = 1;
        this->_semaphore->release();
    }
    else if (evt->key() == KEY_GODOWN)
    {
        this->_movement[2] = -1;
        this->_semaphore->release();
    }
    else if (evt->key() == KEY_ROTATELEFT)
    {
        this->_movement[3] = -1;
        this->_semaphore->release();
    }
    else if (evt->key() == KEY_ROTATERIGHT)
    {
        this->_movement[3] = 1;
        this->_semaphore->release();
    }
    else if (evt->key() == KEY_EMERGENCY)
    {
        this->emergency();
    }
    else
    {
        // Do nothing
    }
}

void Control::keyReleased(QKeyEvent *evt)
{
    if (!this->_keyboard)
        return;
    if (evt->key() == KEY_FORWARD)
    {
        this->_movement[1] = 0;
        this->_semaphore->release();
        //this->brake();
    }
    else if (evt->key() == KEY_BACKWARD)
    {
        this->_movement[1] = 0;
        this->_semaphore->release();
        //this->brake();

    }
    else if (evt->key() == KEY_GOLEFT)
    {
        this->_movement[0] = 0;
        this->_semaphore->release();
        //this->brake();
    }
    else if (evt->key() == KEY_GORIGHT)
    {
        this->_movement[0] = 0;
        this->_semaphore->release();
        //this->brake();
    }
    else if (evt->key() == KEY_GOUP)
    {
        this->_movement[2] = 0;
        this->_semaphore->release();
    }
    else if (evt->key() == KEY_GODOWN)
    {
        this->_movement[2] = 0;
        this->_semaphore->release();
    }
    else if (evt->key() == KEY_ROTATELEFT)
    {
        this->_movement[3] = 0;
        this->_semaphore->release();
    }
    else if (evt->key() == KEY_ROTATERIGHT)
    {
        this->_movement[3] = 0;
        this->_semaphore->release();
    }
    else
    {
        // Do nothing
    }
}

void Control::ctrlStateUpdate(const quint32 ctrl_state)
{
    this->_ctrl_state = ctrl_state;
    this->_state = ctrl_state >> 16;
}

void Control::go()
{
	this->_stop = false;
	this->start();
}

void Control::stop()
{
    this->_stop = true;
}
