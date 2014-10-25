/************************************************
 * @file    mainWindow.h
 * @author  kzhong1991<kzhong1991@gmail.com>
 * @date    2012-12-11
 * @brief   The application's main window
 ************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QGroupBox>
#include <QtGui/QLineEdit>
#include <QtGui/QTextBrowser>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QImage>
#include <QtGui/QStyle>
#include <QtGui/QStyleFactory>
#include <QtCore/QThread>

#include "..//common/functional.h"
#include "..//common/config.h"
#include "..//common/ardrone.h"

#include "..//meters/qmeter.h"
#include "..//meters/navigater.h"
#include "..//meters/altimeter.h"
#include "..//meters/pitchmeter.h"
#include "..//meters/rollmeter.h"

#include "..//ctrl/control.h"
#include "..//ctrl/control_states.h"
#include "..//ctrl/nuiControl.h"

#include "..//video/video.h"
#include "..//video/video_frame.h"

#include "..//navdata/navData.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(ARDrone *ardrone, Control *control, NuiControl *nuiCtrl, Video *video, navData *navdata, 
				QWidget *parent = 0, int width = 960, int heigth = 640);
    ~MainWindow();
	
	/* update the ardrone's velocity, called by  updateStatus() */
    void updateVx(const float vx);
    void updateVy(const float vy);
    void updateVz(const float vz);

	/* 
	** update the ardrone's control state: Takeoff, Landing, Flying, Hovering, Emergency 
	** called by updateStatus().
	*/
    void updateCrtlState();

	/******************Not used***********************/
    void updatePresure(const qint32 pression);
    void updateTemperature(const qint32 temperature);
	/*************************************************/

    void updateBattery(const quint32 percentage);
    void updateCompass(const float milliDegree);
    void updateRoll(const float milliDegree);
    void updatePitch(const float milliDegree);
    void updateAltitude(const quint32 centiMeter);

	/* update the ardrone status */
    void updateStatus(const float v_x, const float v_y, const float v_z,
                      const quint32 ctrl_state, const bool emergency);

	/* update video image */
	void updateCamera(QImage img);

	/* UI components */
	QMeter *            _batMeter;   // battery percentage
	QMeter *            _altiMeter;  // Altitide
	Navigater *         _navMeter;   // Compass
	PitchMeter *        _pMeter;
	RollMeter *         _rMeter;
	videoFrame *		_video_frame;


private:
    /****************** Buttons *****************/
    QPushButton *       _bStartup;
    QPushButton *       _bShutdown;
    QPushButton *       _bSwithCamera;
    QPushButton *       _bEmergency;
    QPushButton *       _bTakeoff_Land;
    QPushButton *       _bSnapshot;
    QPushButton *       _bRecord;
    QPushButton *       _bStopRecord;
    QPushButton *       _bLeft;
    QPushButton *       _bRight;
    QPushButton *       _bUp;
    QPushButton *       _bDown;
    QPushButton *       _bRoateLeft;
    QPushButton *       _bRoateRight;
    QPushButton *       _bForward;
    QPushButton *       _bBackward;
	QPushButton *		_bNuiCtrl;
	QPushButton *		_bShowSkeleton;
	QPushButton *		_bEnableNuiCtrl;
	/********************************************/


    /****************** Labels *****************/
    QLabel *            _lbConnect;
    QLabel *            _lbFlying;
    QLabel *            _lbtakeOff;
    QLabel *            _lbHover;
    QLabel *            _lblanding;
    QLabel *            _lbVx;
    QLabel *            _lbVy;
    QLabel *            _lbVz;
    QLabel *            _lbCameraType;
    QLabel *            _lbCamera;
    QLabel *            _lbPressure;
    QLabel *            _lbTemperature;
    QLabel *            _lbEmergency;

    //QLabel *            _lbVideo;
	/********************************************/

    /****************** Labels *****************/
    QHBoxLayout *       _layout;
    QVBoxLayout *       _layout1;
    QHBoxLayout *       _layout11;
    QHBoxLayout *       _layout12;
    QVBoxLayout *       _layout121;

    QVBoxLayout *       _layout2;
    QVBoxLayout *       _layout21;
    QGridLayout *       _layout22;

    QVBoxLayout *       _layout3;
    /********************************************/


    /*************** Message output *************/
    QTextBrowser *      _tMesg;

    /*************** Group Box ******************/
    QGroupBox *         _gpStatus;
    QGroupBox *         _gpVideo;
    QGroupBox *         _gpCrtl;


	/* The current ardrone */
	ARDrone *			_ardrone;

	/* navdata and video thread */
	Video *				_video;
	navData *			_navdata;

	/* control the ardrone to flight, see the detail of control in control.cpp */
    Control *           _control;

	/* control the ardrone use Kinect */
	NuiControl *		_nuiCtrl;





    /* ardrone status flag */
    bool                _connected;

    /* ardrone control state (takeoff, flying, landed, etc.) */
    quint32             _ctrl_state;

    /* ardrone speed[-1, 1], by default speed = 0.5 */
    float               _speed;

	/* Camera type:
	  -1: no camera
	  0: front camera 
	  1: bottom camera 
	*/
	int				    _cameraType;

	/* the ardrone's current state */
    quint32             _state;

	/* indicate if the ardrone is in emergency mode or not */
    bool                _emergency;

	/* indicate if the keyboard or joystick control is enabled or not */
    bool                _keyboard;

	/* indicate if the kinect control is enabled or not */
	bool				_kinect;

	/* indicate if the video/navdata stream is ok or not */
	bool				_videoOK;
	bool				_navDataOK;


signals:
    void startUpClicked();
    void shutDownClicked();

public slots:
    void updateMesg(const QString cmd, const RESULT res);
    void updateMesg(const QString cmd);

    void startUp();  // connect to the  ardrone
    void shutDown(); // disconnect ardrone

    void takeOff_Land();
    void emergency(); // cut off the engine immediately
    void goUp();
    void goDown();
    void goLeft();
    void goRight();
    void rotateL();
    void rotateR();
    void forward();
    void backward();
    void hovering();

	/* When press "Show Skeleton Data" button, show the kinect skeleton data */
	void showSkeleton();

	/* When press "Enable Kinect Control" button, enable or disable the kinect control */
	void enableNuiCtrl();

	/* Change camera */
	void changeCamera();

	/* Print screen */
	void snapShot();

	//void updateImage(QImage img); 


	/*
	/* receive signals from class ardrone or navData, 
	/* indicate the ardrone initialize succeed or failed 
	*/
    void ardroneInitRes(int res);

	/* is video/navdata available? */
	bool videoOK();
	bool navDataOK();

protected:
    void keyPressEvent(QKeyEvent * evt);
    void keyReleaseEvent(QKeyEvent *evt);

};

#endif // MAINWINDOW_H
