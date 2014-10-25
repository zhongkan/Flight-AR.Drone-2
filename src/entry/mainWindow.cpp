#include "mainWindow.h"

MainWindow::MainWindow(ARDrone *ardrone, Control *control, NuiControl *nuiCtrl, Video *video, navData *navdata, QWidget *parent, int width, int heigth)
    : QMainWindow(parent)
{
    this->setGeometry(530, 100, width, heigth);
    this->setWindowTitle(tr("Flight AR.Drone 2"));
    this->setWindowIcon(QIcon("logo.ico"));

	/* ardrone not connected */
    this->_connected = false;

	/* video/navdata stream is not initialized */
	_videoOK = false;
	_navDataOK = false;

	/* by default, keyboard control is enabled  */
    this->_keyboard = true;

	/* by default, kinect control is disabled */
	this->_kinect = false;

	/* set the default speed of ardrone when control with buttons */
    this->_speed = 0.6f;

	/* Camera: no camera */
	_cameraType = -1;

	/* init the control object, set the default speed of ardrone
	/* control with keyboard or joystick */
    _control = control;
    _control->setSpeed(0.6f);

	/* Kinect control */
	_nuiCtrl = nuiCtrl;
	_nuiCtrl->setSpeed(0.4f);

	/* AR.Drone */
	_ardrone = ardrone;

	/* video and navdata */
	_video = video;
	_navdata = navdata;



	_altiMeter = new QMeter("Altitude\n(m)");
	_altiMeter->setSize(150);
	_altiMeter->setBounds(0, 100);
	_batMeter = new QMeter("Battery\nLevel");
	_batMeter->setSize(150);
	_batMeter->setBounds(0, 100);
	_navMeter = new Navigater();
	_navMeter->setSize(150);
	_pMeter   = new PitchMeter();
	_pMeter->setSize(150);
	_rMeter   = new RollMeter();
	_rMeter->setSize(150);
	_video_frame = new videoFrame();
	

    /* Initialize Buttons */
    this->_bStartup       = new QPushButton(tr("&Startup"));
    this->_bShutdown      = new QPushButton(tr("&Shutdown"));
    this->_bShutdown->setEnabled(false);
    this->_bTakeoff_Land  = new QPushButton(tr("&Takeoff"));
    this->_bSwithCamera   = new QPushButton(tr("&ChangeCamera"));
    this->_bRecord        = new QPushButton(tr("&Record"));
    this->_bStopRecord    = new QPushButton(tr("&Stop"));
    this->_bSnapshot      = new QPushButton(tr("&Snapshot"));
    this->_bEmergency     = new QPushButton(tr("&Emergency"));
    this->_bLeft          = new QPushButton(tr("&Left"));
    this->_bRight         = new QPushButton(tr("&Right"));
    this->_bUp            = new QPushButton(tr("&Up"));
    this->_bDown          = new QPushButton(tr("&Down"));
    this->_bRoateLeft     = new QPushButton(tr("&RoateL"));
    this->_bRoateRight    = new QPushButton(tr("&RoateR"));
    this->_bForward       = new QPushButton(tr("&Forward"));
    this->_bBackward      = new QPushButton(tr("&Back"));
	this->_bNuiCtrl		  = new QPushButton(tr("&Open Kinect"));
	this->_bShowSkeleton  = new QPushButton(tr("&Show Skeleton Data"));
	this->_bEnableNuiCtrl = new QPushButton(tr("&Enable Kinect Control"));
	this->_bEnableNuiCtrl->setEnabled(false);

    /* Initialize Labels */
    this->_lbCamera      = new QLabel(tr("Camera:  \tN/A"));
    this->_lbCameraType  = new QLabel(tr("No Camera"));
    this->_lbConnect     = new QLabel(tr("Connected:\tN/A"));
    this->_lbtakeOff     = new QLabel(tr("TakeOff:    \tN/A"));
    this->_lbFlying      = new QLabel(tr("Flying:     \tN/A"));
    this->_lbHover       = new QLabel(tr("Hovering:\tN/A"));
    this->_lblanding     = new QLabel(tr("Landed:    \tN/A"));
    this->_lbEmergency   = new QLabel(tr("Emergency:\tN/A"));
    this->_lbPressure    = new QLabel(tr("Pressure:\tN/A"));
    this->_lbTemperature = new QLabel(tr("Temperature:\tN/A"));
    this->_lbVx          = new QLabel(tr("X_Velocity:\tN/A"));
    this->_lbVy          = new QLabel(tr("Y_Velocity:\tN/A"));
    this->_lbVz          = new QLabel(tr("Z_Velocity:\tN/A"));

    /* Initialize TextBroswer */
    this->_tMesg = new QTextBrowser();
    this->_tMesg->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    QPalette palette;
    palette.setColor(QPalette::Base, Qt::black);
    palette.setColor(QPalette::Text, Qt::yellow);
    this->_tMesg->setPalette(palette);


    /* Initialize the Layout */
    this->_layout    = new QHBoxLayout();
    this->_layout1   = new QVBoxLayout();
    this->_layout11  = new QHBoxLayout();
    this->_layout12  = new QHBoxLayout();
    this->_layout121 = new QVBoxLayout();
    this->_layout2   = new QVBoxLayout();
    this->_layout21  = new QVBoxLayout();
    this->_layout22  = new QGridLayout();
    this->_layout3   = new QVBoxLayout();

    /* Initialize group box */
    this->_gpStatus = new QGroupBox(tr("Status"));
    this->_gpStatus->setFlat(false);
    this->_gpVideo  = new QGroupBox(tr("Video"));
    this->_gpStatus->setFlat(false);
    this->_gpCrtl   = new QGroupBox(tr("Control"));

    this->_layout11->addWidget(this->_bStartup);
    this->_layout11->addStretch();
    this->_layout11->addWidget(this->_lbCameraType);
    this->_layout11->addStretch();
    this->_layout11->addWidget(this->_bShutdown);

    this->_layout121->addWidget(this->_bSwithCamera);
    this->_layout121->addStretch();
    this->_layout121->addWidget(this->_bSnapshot);
    this->_layout121->addStretch();
    this->_layout121->addWidget(this->_bRecord);
    this->_layout121->addStretch();
    this->_layout121->addWidget(this->_bStopRecord);
	this->_layout121->addStretch();
	this->_layout121->addWidget(this->_bShowSkeleton);


    this->_gpVideo->setLayout(this->_layout121);

    this->_layout12->addWidget(this->_tMesg);
    this->_layout12->addWidget(this->_gpVideo);

    this->_layout1->addLayout(this->_layout11);
    this->_layout1->addSpacing(5);
    this->_layout1->addWidget(this->_video_frame);
    this->_layout1->addSpacing(5);
    this->_layout1->addLayout(this->_layout12);

    this->_layout21->addWidget(this->_lbVx);
    this->_layout21->addWidget(this->_lbVy);
    this->_layout21->addWidget(this->_lbVz);
    this->_layout21->addWidget(this->_lbConnect);
    this->_layout21->addWidget(this->_lbCamera);
    this->_layout21->addWidget(this->_lbtakeOff);
    this->_layout21->addWidget(this->_lbFlying);
    this->_layout21->addWidget(this->_lbHover);
    this->_layout21->addWidget(this->_lblanding);
    this->_layout21->addWidget(this->_lbEmergency);
    //this->_layout21->addWidget(this->_lbPressure);
    //this->_layout21->addWidget(this->_lbTemperature);

    this->_gpStatus->setLayout(this->_layout21);

	this->_layout22->addWidget(this->_bTakeoff_Land, 0, 0, 1, 2);
	this->_layout22->addWidget(this->_bEmergency, 1, 0, 1, 2);
	this->_layout22->addWidget(this->_bForward, 2, 0);
	this->_layout22->addWidget(this->_bBackward, 2, 1);
	this->_layout22->addWidget(this->_bUp, 3, 0);
	this->_layout22->addWidget(this->_bDown, 3, 1);
	this->_layout22->addWidget(this->_bLeft, 4, 0);
	this->_layout22->addWidget(this->_bRight, 4, 1);
	this->_layout22->addWidget(this->_bRoateLeft, 5, 0);
	this->_layout22->addWidget(this->_bRoateRight, 5, 1);
	this->_layout22->addWidget(this->_bEnableNuiCtrl, 6, 0, 1, 2);

    this->_gpCrtl->setLayout(this->_layout22);

    this->_layout2->addWidget(this->_pMeter);
    this->_layout2->addWidget(this->_gpStatus);
    this->_layout2->addWidget(this->_gpCrtl);

    this->_layout3->addWidget(this->_altiMeter);
    this->_layout3->addWidget(this->_rMeter);
    this->_layout3->addWidget(this->_navMeter);
    this->_layout3->addWidget(this->_batMeter);

    this->_layout->addLayout(this->_layout1);
    this->_layout->addStretch();
    this->_layout->addLayout(this->_layout2);
    this->_layout->addStretch();
    this->_layout->addLayout(this->_layout3);

    QWidget * widget = new QWidget();
    widget->setLayout(_layout);

    this->setCentralWidget(widget);

    /* Connect signals and slots */
    QObject::connect(this->_bStartup, SIGNAL(clicked()), this, SLOT(startUp()));
    QObject::connect(this->_bShutdown, SIGNAL(clicked()), this, SLOT(shutDown()));
    QObject::connect(this->_bTakeoff_Land, SIGNAL(clicked()), this, SLOT(takeOff_Land()));
    QObject::connect(this->_bEmergency, SIGNAL(clicked()), this, SLOT(emergency()));

    QObject::connect(this->_bForward, SIGNAL(pressed()), this, SLOT(forward()));
    QObject::connect(this->_bForward, SIGNAL(released()), this, SLOT(hovering()));

    QObject::connect(this->_bBackward, SIGNAL(pressed()), this, SLOT(backward()));
    QObject::connect(this->_bBackward, SIGNAL(released()), this, SLOT(hovering()));

    QObject::connect(this->_bUp, SIGNAL(pressed()), this, SLOT(goUp()));
    QObject::connect(this->_bUp, SIGNAL(released()), this, SLOT(hovering()));

    QObject::connect(this->_bDown, SIGNAL(pressed()), this, SLOT(goDown()));
    QObject::connect(this->_bDown, SIGNAL(released()), this, SLOT(hovering()));

    QObject::connect(this->_bLeft, SIGNAL(pressed()), this, SLOT(goLeft()));
    QObject::connect(this->_bLeft, SIGNAL(released()), this, SLOT(hovering()));

    QObject::connect(this->_bRight, SIGNAL(pressed()), this, SLOT(goRight()));
    QObject::connect(this->_bRight, SIGNAL(released()), this, SLOT(hovering()));

    QObject::connect(this->_bRoateLeft, SIGNAL(pressed()), this, SLOT(rotateL()));
    QObject::connect(this->_bRoateLeft, SIGNAL(released()), this, SLOT(hovering()));

    QObject::connect(this->_bRoateRight, SIGNAL(pressed()), this, SLOT(rotateR()));
    QObject::connect(this->_bRoateRight, SIGNAL(released()), this, SLOT(hovering()));

	QObject::connect(this->_bShowSkeleton, SIGNAL(pressed()), this, SLOT(showSkeleton()));

	QObject::connect(this->_bEnableNuiCtrl, SIGNAL(pressed()), this, SLOT(enableNuiCtrl()));

	QObject::connect(_bSwithCamera, SIGNAL(pressed()), this, SLOT(changeCamera()));

	QObject::connect(_bSnapshot, SIGNAL(pressed()), this, SLOT(snapShot()));

}

MainWindow::~MainWindow()
{
    delete this->_tMesg;
    delete this->_layout;
}

void MainWindow::updateMesg(const QString cmd, const RESULT res)
{
    this->_tMesg->append(tr("Sending: ") + cmd + "\t" + reslut(res));
}

void MainWindow::updateMesg(const QString mesg)
{
    this->_tMesg->append(mesg);
}

void MainWindow::updateBattery(const quint32 percentage)
{
    this->_batMeter->updateValue(percentage);
}

void MainWindow::updateCompass(const float milliDegree)
{
    this->_navMeter->updateValue(milliDegree);
}

void MainWindow::updateAltitude(const quint32 centiMeter)
{
    this->_altiMeter->updateValue(centiMeter / 100.0);
}

void MainWindow::updatePitch(const float milliDegree)
{
    this->_pMeter->updateValue(milliDegree);
}

void MainWindow::updateRoll(const float milliDegree)
{
    this->_rMeter->updateValue(milliDegree);
}

void MainWindow::updateStatus(const float v_x, const float v_y, const float v_z,
                              const quint32 ctrl_state, const bool emergency)
{
    this->updateVx(v_x);
    this->updateVy(v_y);
    this->updateVz(v_z);
    this->_ctrl_state = ctrl_state;
    this->_state = ctrl_state >> 16;
    this->_emergency = emergency;

    if (this->_state == CTRL_LANDED)
        this->_bTakeoff_Land->setText(tr("&TakeOff"));
    else
        this->_bTakeoff_Land->setText(tr("&Landing"));

    this->updateCrtlState();
}

void MainWindow::updateVx(const float vx)
{
    this->_lbVx->setText("X_Velocity:\t" + ftos(vx));
}

void MainWindow::updateVy(const float vy)
{
    this->_lbVy->setText("Y_Velocity:\t" + ftos(vy));
}

void MainWindow::updateVz(const float vz)
{
    this->_lbVz->setText("Z_Velocity:\t" + ftos(vz));
}

void MainWindow::updateCrtlState()
{
    if (this->_connected)
    {
        this->_lbConnect->setText(tr("Connected:\ttrue"));
    }
    else
    {
        this->_lbConnect->setText(tr("Connected:\tfalse"));
    }

    switch(this->_state)
    {
    case CTRL_TRANS_TAKEOFF:
        this->_lbtakeOff->setText(tr("TakeOff:    \ttrue"));
        this->_lbFlying->setText(tr("Flying:     \tfalse"));
        this->_lbHover->setText(tr("Hovering:\tfalse"));
        this->_lblanding->setText(tr("Landing:    \tfalse"));
        break;
    case CTRL_FLYING:
        this->_lbtakeOff->setText(tr("TakeOff:    \tfalse"));
        this->_lbFlying->setText(tr("Flying:     \ttrue"));
        this->_lbHover->setText(tr("Hovering:\tfalse"));
        this->_lblanding->setText(tr("Landing:    \tfalse"));
        break;
    case CTRL_HOVERING:
        this->_lbtakeOff->setText(tr("TakeOff:    \tfalse"));
        this->_lbFlying->setText(tr("Flying:     \tfalse"));
        this->_lbHover->setText(tr("Hovering:\ttrue"));
        this->_lblanding->setText(tr("Landing:    \tfalse"));
        break;
    case CTRL_LANDED:
        this->_lbtakeOff->setText(tr("TakeOff:    \tfalse"));
        this->_lbFlying->setText(tr("Flying:     \tfalse"));
        this->_lbHover->setText(tr("Hovering:\tfalse"));
        this->_lblanding->setText(tr("Landed:    \ttrue"));
        break;
    case CTRL_TRANS_LANDING:
        this->_lbtakeOff->setText(tr("TakeOff:    \tfalse"));
        this->_lbFlying->setText(tr("Flying:     \tfalse"));
        this->_lbHover->setText(tr("Hovering:\tfalse"));
        this->_lblanding->setText(tr("Landed:    \tfalse"));
        break;
    default:
        this->_lbtakeOff->setText(tr("TakeOff:    \tfalse"));
        this->_lbFlying->setText(tr("Flying:     \tfalse"));
        this->_lbHover->setText(tr("Hovering:\tfalse"));
        this->_lblanding->setText(tr("Landing:    \tfalse"));
        break;
    }

    /* the ardrone is emergency or not */
    if (this->_emergency)
        this->_lbEmergency->setText(tr("Emergency:\ttrue"));
    else
        this->_lbEmergency->setText(tr("Emergency:\tfalse"));

}

void MainWindow::updatePresure(const qint32 pression)
{
    this->_lbPressure->setText(QString::fromStdString("Pressure:\t") + ftos(pression / 100000.0) + "atm");
}

void MainWindow::updateTemperature(const qint32 temperature)
{
    this->_lbTemperature->setText(QString::fromStdString("Temperature:\t") + ftos(temperature / 10.0) + "C");
}

void MainWindow::updateCamera(QImage img)
{
	this->_video_frame->updateImage(img);
}


void MainWindow::startUp()
{
    if (!this->_connected)
    {
        emit this->startUpClicked();
		this->_control->go();
		this->_bEnableNuiCtrl->setEnabled(true);
    }
}

void MainWindow::shutDown()
{
    if (this->_connected)
    {
        emit this->shutDownClicked();
		this->_bStartup->setEnabled(true);
		this->_bShutdown->setEnabled(false);
		this->_connected = false;

		/* reset video and navdata  */
		this->_videoOK = false;
		this->_navDataOK = false;

		/* stop contorl thread */
		this->_nuiCtrl->stop();
		this->_control->stop();
    }
}

void MainWindow::changeCamera()
{
	if (_cameraType == -1)
		return;
	else if(_cameraType == 0)
	{
		/* change to bottom camera */
		_ardrone->sendf("AT*CONFIG_IDS=%d,\"%s\",\"%s\",\"%s\"\r", _ardrone->getSeqNum(), ARDRONE_SESSION_ID, ARDRONE_PROFILE_ID, ARDRONE_APPLOCATION_ID);
		_ardrone->sendf("AT*CONFIG=%d,\"video:video_channel\",\"1\"\r", _ardrone->getSeqNum());
		this->_cameraType = 1;
		this->_lbCamera->setText("Camera:	\tBottom");
		this->_lbCameraType->setText("Bottom Camera");
	}
	else
	{
		/* change to front camera */
		_ardrone->sendf("AT*CONFIG_IDS=%d,\"%s\",\"%s\",\"%s\"\r", _ardrone->getSeqNum(), ARDRONE_SESSION_ID, ARDRONE_PROFILE_ID, ARDRONE_APPLOCATION_ID);
		_ardrone->sendf("AT*CONFIG=%d,\"video:video_channel\",\"0\"\r", _ardrone->getSeqNum());
		this->_cameraType = 0;
		this->_lbCamera->setText("Camera:	\tFront");
		this->_lbCameraType->setText("Front Camera");
	}
}

void MainWindow::snapShot()
{
	_video_frame->snapShot();
}

void MainWindow::takeOff_Land()
{
    if (this->_connected)
    {
        if (this->_state == CTRL_LANDED)
        {
             this->_control->takeOff();
        }
        else
        {
             this->_control->landing();
        }
    }
}

void MainWindow::emergency()
{
    if (this->_connected)
    {
        this->_control->emergency();
    }
}

void MainWindow::goUp()
{
    if (this->_connected)
        this->_control->goUp(this->_speed);

}

void MainWindow::goDown()
{
    if (this->_connected)
        this->_control->goDown(this->_speed);
}

void MainWindow::goLeft()
{
    if (this->_connected)
        this->_control->goLeft(this->_speed);
}

void MainWindow::goRight()
{
    if (this->_connected)
        this->_control->goRight(this->_speed);
}

void MainWindow::rotateL()
{
    if (this->_connected)
        this->_control->rotateL(this->_speed);
}

void MainWindow::rotateR()
{
    if (this->_connected)
        this->_control->rotateR(this->_speed);
}

void MainWindow::forward()
{
    if (this->_connected)
        this->_control->forward(this->_speed);
}

void MainWindow::backward()
{
    if (this->_connected)
        this->_control->backward(this->_speed);
}

void MainWindow::hovering()
{
    if (this->_connected)
        this->_control->hovering();
}

void MainWindow::showSkeleton()
{
	this->_nuiCtrl->_skelForm->show();
}

void MainWindow::enableNuiCtrl()
{
	if (!this->_kinect)
	{
		this->_nuiCtrl->go();
		this->_bEnableNuiCtrl->setText(tr("&Disable Kinect Control"));
		this->_kinect = true;
	}
	else
	{
		this->_nuiCtrl->stop();
		this->_bEnableNuiCtrl->setText(tr("&Enable Kinect Control"));
		this->_kinect = false;
	}
}

void MainWindow::ardroneInitRes(int res)
{
	if (res == NAV_DATA_INIT_OK)
	{
		_navDataOK = true;
	}
	else if(res == NAV_DATA_INIT_FAILED)
	{
		_navDataOK = false;
	}
	else if(res == VIDEO_INIT_OK)
	{
		_videoOK = true;

		/* camera */
		this->_lbCamera->setText("Camera:	\tFront");
		this->_lbCameraType->setText("Front Camera");
		this->_cameraType = 0; // Front
	}
	else if(res == VIDEO_INIT_FAILED)
	{
		_videoOK = false;
	}
	else if(res == ARDRONE_INIT_FAILED)
	{
		_videoOK = false;
		_navDataOK = false;
	}

	/* ardrone connected */
	if (_videoOK && _navDataOK)
	{
		this->_tMesg->append("AR.Drone connected.");
		this->_bStartup->setEnabled(false);
		this->_bShutdown->setEnabled(true);
		this->_connected = true;
	}
	/* ardrone init failed*/
	else if((!_videoOK) && (!_navDataOK))
	{
		this->_tMesg->append("AR.Drone initialize FAILED!");
		this->_bStartup->setEnabled(true);
		this->_bShutdown->setEnabled(false);
		this->_connected = false;
	}
}

bool MainWindow::videoOK()
{
	return _videoOK;
}

bool MainWindow::navDataOK()
{
	return _navDataOK;
}

void MainWindow::keyPressEvent(QKeyEvent *evt)
{
    if (this->_connected && evt->isAutoRepeat() == false)
    {
        if (evt->key() == KEY_CTRL_ENABLE)
        {
            this->_keyboard = !this->_keyboard;
            this->_control->keyBoardCtrlEnable(this->_keyboard);
        }
        this->_control->keyPressed(evt);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *evt)
{
    if (this->_connected && evt->isAutoRepeat() == false)
    {
        this->_control->keyReleased(evt);
    }
}
