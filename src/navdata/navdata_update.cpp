#include "..//navdata/navdata_update.h"

navUpdate::navUpdate(MainWindow * wd, navData * navdata, QObject * parent /* = 0 */)
    :QThread(parent)
{
    this->_navdata = navdata;
    this->_wd      = wd;
    this->_stop = false;
	
	
    this->_stateTimer = new QTimer();
    this->_batTimer   = new QTimer();
    this->_altiTimer  = new QTimer();
    this->_magTimer   = new QTimer();

    QObject::connect(this->_stateTimer, SIGNAL(timeout()), this, SLOT(stateUpdate()));
    QObject::connect(this->_magTimer,   SIGNAL(timeout()), this, SLOT(magUpdate()));
    QObject::connect(this->_magTimer,   SIGNAL(timeout()), this, SLOT(batteryUpdate()));
    QObject::connect(this->_altiTimer,  SIGNAL(timeout()), this, SLOT(altitudeUpdate()));

    QObject::connect(this, SIGNAL(timerStart()), this, SLOT(startTimer()));
    QObject::connect(this, SIGNAL(timerStop()), this, SLOT(stopTimer()));
	
	
}

void navUpdate::go()
{
	this->_stop = false;
	this->start();
}

void navUpdate::stop()
{
    this->_stop = true;
}

void navUpdate::run()
{

    /* start the timer */
    emit this->timerStart();

    while(!this->_stop)
    {
        /* notify the control and nuiControl to update the ctrl_state */
        emit this->ctrlStateUpdate(this->_navdata->get_ctrl_state());
        msleep(10);
    }

    /* stop the timer */
    emit this->timerStop();
}

void navUpdate::stateUpdate()
{
    float vx            = this->_navdata->get_vx();
    float vy            = this->_navdata->get_vy();
    float vz            = this->_navdata->get_vz();
    quint32 ctrl_state  = this->_navdata->get_ctrl_state();
    bool emergency      = this->_navdata->isEmergency();
    this->_wd->updateStatus(vx, vy, vz, ctrl_state, emergency);
}

void navUpdate::magUpdate()
{

    this->_wd->updatePitch(this->_navdata->get_pitch());

    this->_wd->updateRoll(this->_navdata->get_roll());

    this->_wd->updateCompass(this->_navdata->get_yaw());
}

void navUpdate::batteryUpdate()
{
    this->_wd->updateBattery(this->_navdata->get_battery());
}

void navUpdate::altitudeUpdate()
{
    this->_wd->updateAltitude(this->_navdata->get_altitude());
}

void navUpdate::startTimer()
{
    this->_stateTimer->start(500);
    this->_magTimer->start(40);
    this->_batTimer->start(5000);
    this->_altiTimer->start(1000);
}

void navUpdate::stopTimer()
{
    this->_stateTimer->stop();
    this->_magTimer->stop();
    this->_batTimer->stop();
    this->_altiTimer->stop();
}

void navUpdate::startNavUpdate()
{
	this->go();
}

void navUpdate::stopNavUpdate()
{
	this->stop();
}
