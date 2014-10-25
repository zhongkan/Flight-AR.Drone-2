/**********************************************
 * @file    nuiControl.h
 * @author  kzhong1991
 * @date    2013-01-23
 * @brief   Control the ardrone by human body
 **********************************************/
#ifndef NUICONTROL_H
#define NUICONTROL_H

#include <QtCore/QThread>
#include <windows.h>
#include "NuiApi.h"
#include "..//entry/skeletonForm.h"
#include "..//ctrl/control.h"
#include "..//common/ardrone.h"
#include "..//common/functional.h"

#define DEBUG_KINECT_CTRL "Kinect control"

class NuiControl : public QThread
{
    Q_OBJECT
public:
    NuiControl(ARDrone * ardrone, Control *control, SkeletonForm * skelForm, QObject *parent = 0);
    ~NuiControl();
	
	/* Initialize the Kinect */
    HRESULT	connectKinect();

	/* Handle the skeleton data */
	void handleSkeletonData();

	/* Recognize the user body language */
	void gestureRecognize(NUI_SKELETON_DATA skeletonData, float &roll, float &pitch, float &gaz, float &yaw);

	/* start or stop the control thread */
	void go();
	void stop();

	/* enable or disable the ardrone control */
	void NuiCtrlEnable(const bool enable);

	void setSpeed(const float speed);

	void print(const string mesg);
	void print(const QString mesg);
	void print(const char * mesg);

	/* show the skeleton data, used by main window */
	SkeletonForm*		_skelForm;

private:
    INuiSensor*			_pNuiSensor;			// Point to the current Kinect
    HANDLE				_hNextSkeletonEvent;	// The Kinect Event
	MSG					_msg;
	bool				_sensorState;

    Control*			_control;
	ARDrone*			_ardrone;

	/* the ardrone speed when use kinect control */
	float				_speed;

	/* ardrone control state (takeoff, flying, landed, etc.) */
	quint32				_ctrl_state;
	quint32				_state;

	/* when set to true, the thread will be shutdown */
	bool				_stop;

	/* indicate if the kinect control is enabled or not */
	bool				_enable;

	bool				_pcmd;


    
signals:
    
public slots:
	void nuiCtrlStateUpdate(const quint32 ctrl_state);

protected:
	void run();
    
};

#endif // NUICONTROL_H
