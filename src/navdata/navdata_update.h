/***********************************************************************************
 * @file    navdata_update.h
 * @author  kzhong1991<kzhong1991@gmail.com>
 * @date    2013-1-18
 * @brief   Update the the navdata show in main window 
 ***********************************************************************************/

#ifndef NAVDATA_UPDATE_H
#define NAVDATA_UPDATE_H
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include "..//navdata/navData.h"
#include "..//entry/mainWindow.h"

class navUpdate : public QThread
{
    Q_OBJECT

public:
    navUpdate(MainWindow * wd, navData * navdata, QObject * parent = 0);

	/* start / stop thread */
	void go();
    void stop();

private:
    navData *       _navdata;
    MainWindow *    _wd;
    bool            _stop;

    QTimer *        _stateTimer;
    QTimer *        _magTimer;
    QTimer *        _batTimer;
    QTimer *        _altiTimer;

protected:
    void run();
signals:
    /* update the state of class control and nuiControl */
    void ctrlStateUpdate(const quint32 ctrl_state);

	/* start / stop the timer */
    void timerStart();
    void timerStop();

private slots:
    /************** update the navdata of mainwindow ***************/
    void stateUpdate();
    void magUpdate();
    void batteryUpdate();
    void altitudeUpdate();
    /***************************************************************/

	/* start / stop the timer */
    void startTimer();
    void stopTimer();

	/* receive signal from thread navdata, start / stop the nav_update thread */
	void startNavUpdate();
	void stopNavUpdate();

};

#endif // NAVDATA_UPDATE_H
