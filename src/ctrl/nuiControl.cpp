#include "..//CTRL/nuiControl.h"

NuiControl::NuiControl(ARDrone *ardrone, Control *control, SkeletonForm * skelForm, QObject *parent /* = 0 */) :
	QThread(parent)
{
	this->_ardrone	   = ardrone;
	this->_control     = control;
	this->_skelForm    = skelForm;
	this->_pNuiSensor  = NULL;
	this->_stop		   = false;
	this->_sensorState = false;
	this->_enable	   = false;
	this->_ctrl_state  = 131072;
	this->_state	   = 2;
	this->_pcmd		   = false;
	this->_hNextSkeletonEvent = INVALID_HANDLE_VALUE;
	memset(&this->_msg, 0, sizeof(this->_msg));

}

NuiControl::~NuiControl()
{
	if (this->_pNuiSensor != NULL)
	{
		this->_pNuiSensor->NuiShutdown();
	}
	if (this->_hNextSkeletonEvent && (this->_hNextSkeletonEvent != INVALID_HANDLE_VALUE))
	{
		CloseHandle(this->_hNextSkeletonEvent);
	}
}

HRESULT NuiControl::connectKinect()
{
	int cnt = 0;
	HRESULT hr = NuiGetSensorCount(&cnt);

	if (FAILED(hr)) // hr < 0
	{
		return hr;
	}

	for (int i = 0; i < cnt; i++)
	{
		hr = NuiCreateSensorByIndex(i, &this->_pNuiSensor);
		if (FAILED(hr))
		{
			continue;
		}

		/* Get the status of Kinect */
		hr = this->_pNuiSensor->NuiStatus();

		if (S_OK == hr)
		{
			break;
		}

		/* This Kinect is not ok, release it */
		this->_pNuiSensor->Release();
	}

	if (NULL != this->_pNuiSensor)
	{
		/* Initialize the Kinect and specify that we'll using skeleton */
		hr = this->_pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);

		if (SUCCEEDED(hr))
		{
			/* Create an event that will be signaled when skeleton data is available */
			this->_hNextSkeletonEvent = CreateEventW(NULL, TRUE, FALSE, NULL);

			/* Open a skeleton stream to receive skeleton data */
			hr = this->_pNuiSensor->NuiSkeletonTrackingEnable(this->_hNextSkeletonEvent, 0);
		}
	}

	if (NULL == this->_pNuiSensor || FAILED(hr))
	{
		return E_FAIL;
	}

	return hr;
}

void NuiControl::handleSkeletonData()
{
	NUI_SKELETON_FRAME skeletonFrame = {0};
	float roll, pitch, gaz, yaw;

	if (NULL != this->_pNuiSensor)
	{
		HRESULT hr = this->_pNuiSensor->NuiSkeletonGetNextFrame(0, &skeletonFrame);

		if (FAILED(hr))
		{
			return;
		}
	}
	else
	{
		this->print("Kinect disconnected!");
		return;
	}

	/* Smooth out the skeleton data */
	HRESULT hr = this->_pNuiSensor->NuiTransformSmooth(&skeletonFrame, NULL);
	if (FAILED(hr))
	{
		this->print("Smooth skeleton frame failed!");
		return;
	}

	int i;
	for (i = 0; i < NUI_SKELETON_COUNT; i++)
	{
		NUI_SKELETON_TRACKING_STATE skeletonStatus = skeletonFrame.SkeletonData[i].eTrackingState;

		/* Find one tracked skeleton, draw it, and then break */
		if (NUI_SKELETON_TRACKED == skeletonStatus)
		{
			this->_skelForm->drawSkeleton(skeletonFrame.SkeletonData[i]);
			this->gestureRecognize(skeletonFrame.SkeletonData[i], roll, pitch, gaz, yaw);

			if (fabs(roll) <= eps && fabs(pitch) <= eps && fabs(gaz) <= eps && fabs(yaw) <= eps)
			{
				if (_pcmd)
				{
					_ardrone->sendPCMD_MAG(1 ,0 ,0 ,0 ,0 ,0 ,0);
					msleep(5);
				}
				_ardrone->sendPCMD_MAG(0 ,0 ,0 ,0 ,0 ,0 ,0);
				_pcmd = false;
				break;
			}

			if(!_pcmd)
			{
				_ardrone->sendPCMD_MAG(1, 0, 0, 0, 0, 0, 0);
				msleep(5);
			}

			this->_ardrone->sendPCMD_MAG(1, roll, pitch, gaz, yaw, 0, 0);
			_pcmd = true;
			break;
		}
	}

	/* No tracked skeleton found */
	if (i >= NUI_SKELETON_COUNT)
	{
		this->_ardrone->sendPCMD_MAG(1, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0);
	}
}

/*************************************************************************/
/* This function is used to control the ardrone with the kinect skeleton */
/* data. Use the point of the skeleton to identification the instruction,*/
/* it's a very stupid method, must be updated one day.					 */
/* Version: 0.0.1														 */ 
/* Create data: 2013-03-26												 */
/* roll[-1, 1]:	 Left/Right												 */
/* pitch[-1, 1]: Forward/Backward										 */
/* gaz[-1, 1]:   Down/Up												 */
/* yaw[-1, 1]:	 Rotate Left/Rotate Right								 */
/*************************************************************************/
void NuiControl::gestureRecognize(NUI_SKELETON_DATA skeletonData, float &roll, float &pitch, float &gaz, float &yaw)
{
	/*********************************************************************
								^    ^
								| Y /X
								|  /
								| /
								|/
					  ----------/---------->Z(Sensor Direction)
							   /|
							  / |
							 /  |
							/   |

				 Skeleton-space coordinate system
		This is a right-handed coordinate system that places the sensor array 
	at the origin point with the positive z axis extending in the direction in 
	which the sensor array points. The positive y axis extends upward, and the 
	positive x axis extends to the left (with respect to the sensor array).
	***********************************************************************/

	/* the point of skeleton position in the skeleton-space coordinate system */
	vector31_t P[NUI_SKELETON_POSITION_COUNT];

	for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; i++)
	{
		P[i].x = skeletonData.SkeletonPositions[i].x;
		P[i].y = skeletonData.SkeletonPositions[i].y;
		P[i].z = skeletonData.SkeletonPositions[i].z;
		
	}

	roll = 0.0f, pitch = 0.0f, gaz = 0.0f, yaw = 0.0f;


	/* takeoff or landing */
	if (
		(P[NUI_SKELETON_POSITION_HIP_LEFT].y   - P[NUI_SKELETON_POSITION_HAND_LEFT].y) > 0.1f &&
		(P[NUI_SKELETON_POSITION_HAND_RIGHT].y - P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y) > 0.45f
		)
		{
			if (this->_state == CTRL_LANDED)
			{
				this->_control->takeOff();
				this->print("take off");
				return;
			}
			else 
			{
				this->_control->landing();
				this->print("landing");
				return;
			}
		}

	/* go right */
	if (
		P[NUI_SKELETON_POSITION_WRIST_RIGHT].y		< P[NUI_SKELETON_POSITION_ELBOW_RIGHT].y && 
		P[NUI_SKELETON_POSITION_ELBOW_RIGHT].y		< P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y &&
		P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y	< P[NUI_SKELETON_POSITION_SHOULDER_LEFT].y &&
		P[NUI_SKELETON_POSITION_SHOULDER_LEFT].y	< P[NUI_SKELETON_POSITION_ELBOW_LEFT].y &&
		P[NUI_SKELETON_POSITION_ELBOW_LEFT].y		< P[NUI_SKELETON_POSITION_WRIST_LEFT].y &&
		(P[NUI_SKELETON_POSITION_WRIST_LEFT].y - P[NUI_SKELETON_POSITION_WRIST_RIGHT].y) > 0.2f &&

		P[NUI_SKELETON_POSITION_WRIST_RIGHT].x		> P[NUI_SKELETON_POSITION_ELBOW_RIGHT].x && 
		P[NUI_SKELETON_POSITION_ELBOW_RIGHT].x		> P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x &&
		P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x	> P[NUI_SKELETON_POSITION_SHOULDER_LEFT].x&&
		P[NUI_SKELETON_POSITION_SHOULDER_LEFT].x	> P[NUI_SKELETON_POSITION_ELBOW_LEFT].x &&
		P[NUI_SKELETON_POSITION_ELBOW_LEFT].x		> P[NUI_SKELETON_POSITION_WRIST_LEFT].x 
		)
	{
		roll = this->_speed;
		#ifdef DEBUG_KINECT_CTRL 
		this->print("go right : " + ftos(roll));
		#endif
	}

	/* go left */
	else if (
		P[NUI_SKELETON_POSITION_WRIST_LEFT].y		< P[NUI_SKELETON_POSITION_ELBOW_LEFT].y && 
		P[NUI_SKELETON_POSITION_ELBOW_LEFT].y		< P[NUI_SKELETON_POSITION_SHOULDER_LEFT].y &&
		P[NUI_SKELETON_POSITION_SHOULDER_LEFT].y	< P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y&&
		P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y	< P[NUI_SKELETON_POSITION_ELBOW_RIGHT].y &&
		P[NUI_SKELETON_POSITION_ELBOW_RIGHT].y		< P[NUI_SKELETON_POSITION_WRIST_RIGHT].y &&
		(P[NUI_SKELETON_POSITION_WRIST_RIGHT].y - P[NUI_SKELETON_POSITION_WRIST_LEFT].y) > 0.2f &&

		P[NUI_SKELETON_POSITION_WRIST_RIGHT].x		> P[NUI_SKELETON_POSITION_ELBOW_RIGHT].x && 
		P[NUI_SKELETON_POSITION_ELBOW_RIGHT].x		> P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x &&
		P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x	> P[NUI_SKELETON_POSITION_SHOULDER_LEFT].x&&
		P[NUI_SKELETON_POSITION_SHOULDER_LEFT].x	> P[NUI_SKELETON_POSITION_ELBOW_LEFT].x &&
		P[NUI_SKELETON_POSITION_ELBOW_LEFT].x		> P[NUI_SKELETON_POSITION_WRIST_LEFT].x 
		)
	{
		roll = -this->_speed;
		#ifdef DEBUG_KINECT_CTRL 
		this->print("go left: " + ftos(roll));
		#endif;
	}
	/* go up */
	if(
		P[NUI_SKELETON_POSITION_WRIST_RIGHT].y		> P[NUI_SKELETON_POSITION_ELBOW_RIGHT].y &&
		P[NUI_SKELETON_POSITION_ELBOW_RIGHT].y		> P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y &&
		P[NUI_SKELETON_POSITION_WRIST_LEFT].y		> P[NUI_SKELETON_POSITION_ELBOW_LEFT].y &&
		P[NUI_SKELETON_POSITION_ELBOW_LEFT].y		> P[NUI_SKELETON_POSITION_SHOULDER_LEFT].y &&
		(P[NUI_SKELETON_POSITION_WRIST_RIGHT].y - P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y) > 0.2f &&
		(P[NUI_SKELETON_POSITION_WRIST_LEFT].y  - P[NUI_SKELETON_POSITION_SHOULDER_LEFT].y)  > 0.2f &&

		P[NUI_SKELETON_POSITION_ELBOW_RIGHT].x		> P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x &&
		P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x	> P[NUI_SKELETON_POSITION_SHOULDER_LEFT].x&&
		P[NUI_SKELETON_POSITION_SHOULDER_LEFT].x	> P[NUI_SKELETON_POSITION_ELBOW_LEFT].x &&
		P[NUI_SKELETON_POSITION_ELBOW_LEFT].x		> P[NUI_SKELETON_POSITION_WRIST_LEFT].x 
		)
	{
		gaz = this->_speed;
		#ifdef DEBUG_KINECT_CTRL 
		this->print("go up: " + ftos(gaz));
		#endif
	}
	/* go down */
	else if (
		P[NUI_SKELETON_POSITION_WRIST_RIGHT].y		< P[NUI_SKELETON_POSITION_ELBOW_RIGHT].y &&
		P[NUI_SKELETON_POSITION_ELBOW_RIGHT].y		< P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y&&
		P[NUI_SKELETON_POSITION_WRIST_LEFT].y		< P[NUI_SKELETON_POSITION_ELBOW_LEFT].y &&
		P[NUI_SKELETON_POSITION_ELBOW_LEFT].y		< P[NUI_SKELETON_POSITION_SHOULDER_LEFT].y &&
		(P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y - P[NUI_SKELETON_POSITION_WRIST_RIGHT].y) > 0.15f &&
		(P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y - P[NUI_SKELETON_POSITION_WRIST_RIGHT].y) < 0.35f &&
		(P[NUI_SKELETON_POSITION_SHOULDER_LEFT].y  - P[NUI_SKELETON_POSITION_WRIST_LEFT].y)  > 0.15f &&
		(P[NUI_SKELETON_POSITION_SHOULDER_LEFT].y  - P[NUI_SKELETON_POSITION_WRIST_LEFT].y)  < 0.35f &&

		P[NUI_SKELETON_POSITION_ELBOW_RIGHT].x		> P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x &&
		P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x	> P[NUI_SKELETON_POSITION_SHOULDER_LEFT].x&&
		P[NUI_SKELETON_POSITION_SHOULDER_LEFT].x	> P[NUI_SKELETON_POSITION_ELBOW_LEFT].x &&
		P[NUI_SKELETON_POSITION_ELBOW_LEFT].x		> P[NUI_SKELETON_POSITION_WRIST_LEFT].x
		)

	{
		gaz = -this->_speed;
		#ifdef DEBUG_KINECT_CTRL
		this->print("go down: " + ftos(gaz));
		#endif // DEBUG_KINECT_CTRL
	}
	
	/* forward */
	if ( (P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].z - P[NUI_SKELETON_POSITION_HAND_RIGHT].z) > 0.3f &&
		 (P[NUI_SKELETON_POSITION_SHOULDER_LEFT].z - P[NUI_SKELETON_POSITION_HAND_LEFT].z)   > 0.3f
		)
	{
		pitch = -this->_speed;
		this->print("forward: " + ftos(pitch));
	}
	/* backward */
	else if (
		(P[NUI_SKELETON_POSITION_HAND_RIGHT].z - P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].z) > 0.1f &&
		(P[NUI_SKELETON_POSITION_HAND_LEFT].z  - P[NUI_SKELETON_POSITION_SHOULDER_LEFT].z)   > 0.1f
		)
	{
		pitch = this->_speed;
		#ifdef DEBUG_KINECT_CTRL
		this->print("backward: " + ftos(pitch));
		#endif
	}

	/* rotate left */
	if (
		P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].z - P[NUI_SKELETON_POSITION_SHOULDER_LEFT].z > 0.1f &&
		P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].z - P[NUI_SKELETON_POSITION_SHOULDER_LEFT].z < 0.3f
		)
	{
		yaw = -this->_speed;
		#ifdef DEBUG_KINECT_CTRL
		this->print("rotate left: " + ftos(yaw));
		#endif;
	}
	/* rotate right */
	else if(
		P[NUI_SKELETON_POSITION_SHOULDER_LEFT].z - P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].z > 0.1f &&
		P[NUI_SKELETON_POSITION_SHOULDER_LEFT].z - P[NUI_SKELETON_POSITION_SHOULDER_RIGHT].z < 0.3f
		)
	{
		yaw = this->_speed;
		#ifdef DEBUG_KINECT_CTRL
		this->print("rotate right: " + ftos(yaw));
		#endif;
	}
}

void NuiControl::run()
{
	/* Connect to the Kinect */
	HRESULT hr = this->connectKinect();

	if (SUCCEEDED(hr))
	{
		this->print("Kinect initialize OK.");
		this->_sensorState = true;
	}
	else
	{
		this->print("No ready Kinect found!");
	}

	/* Main message loop */
	while (WM_QUIT != this->_msg.message && !this->_stop)
	{
		/* wait for Kinect data */
		DWORD dwEvent = WaitForSingleObject(this->_hNextSkeletonEvent, INFINITE);

		if (WAIT_OBJECT_0 == dwEvent && this->_enable)
		{
			this->handleSkeletonData();
		}

		/* Discard message */
		if (PeekMessage(&this->_msg, NULL, 0, 0, PM_REMOVE))
		{
			continue;
		}
		TranslateMessage(&this->_msg);
		DispatchMessage(&this->_msg);
	}

	if (this->_pNuiSensor != NULL)
	{
		this->_pNuiSensor->NuiShutdown();
	}
	if (this->_hNextSkeletonEvent && (this->_hNextSkeletonEvent != INVALID_HANDLE_VALUE))
	{
		CloseHandle(this->_hNextSkeletonEvent);
	}
}

/* Update the ardrone control state */
void NuiControl::nuiCtrlStateUpdate(const quint32 ctrl_state)
{
	this->_ctrl_state = ctrl_state;
	this->_state = ctrl_state >> 16;
}

/* Start the thread */
void NuiControl::go()
{
	this->_stop   = false;
	this->_enable = true;
	this->start();
}

/* Stop the thread */
void NuiControl::stop()
{
	this->_stop = true;
}

/* enable or disable kinect control */
void NuiControl::NuiCtrlEnable(const bool enable)
{
	this->_enable = enable;
}

void NuiControl::setSpeed(const float speed)
{
	this->_speed = speed;
}

void NuiControl::print(const char * mesg)
{
	this->_ardrone->print(mesg);
}

void NuiControl::print(const string mesg)
{
	this->_ardrone->print(mesg);
}

void NuiControl::print(const QString mesg)
{
	this->_ardrone->print(mesg);
}