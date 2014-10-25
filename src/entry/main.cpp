#include <QtGui/QtGui>
#include <QtCore/QObject>
#include <QtGui/QApplication>
#include "..//entry/mainWindow.h"
#include "..//entry/skeletonForm.h"
#include "..//common/ardrone.h"
#include "..//navdata/navData.h"
#include "..//navdata/navdata_update.h"
#include "..//ctrl/control.h"
#include "..//meters/navigater.h"
#include "..//meters/qmeter.h"
#include "..//meters/altimeter.h"
#include "..//meters/rollmeter.h"
#include "..//meters/pitchmeter.h"
#include "..//video/video.h"
#include "..//video/video_update.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	/* Initialize a ardrone */
    ARDrone * ardrone       = new ARDrone();

	/* Processing navigation data */
    navData * navdata       = new navData(ardrone); 

	/* Processing video data */
	Video * video			= new Video(ardrone);

	/* Control the ardrone use keyboard or joystick */
    Control * control       = new Control(ardrone);

	/* Show skeleton data */
	SkeletonForm * skelForm = new SkeletonForm();

	/* Control the ardrone with Kinect */
	NuiControl * nuiCtrl	= new NuiControl(ardrone, control, skelForm);

	/* User interface */
    MainWindow * wd         = new MainWindow(ardrone, control, nuiCtrl, video, navdata);

	/* video/navdata update thread */
	videoUpdate *video_update = new videoUpdate(wd, video);
	navUpdate   *nav_update	  = new navUpdate(wd, navdata);

	/* Passing message */
    QObject::connect(ardrone, SIGNAL(mesgShow(QString, RESULT)), wd, SLOT(updateMesg(QString, RESULT)));
    QObject::connect(ardrone, SIGNAL(mesgShow(QString)), wd, SLOT(updateMesg(QString)));

    QObject::connect(wd, SIGNAL(startUpClicked()), ardrone, SLOT(startUp()));
    QObject::connect(wd, SIGNAL(shutDownClicked()), ardrone, SLOT(shutDown()));

	/* Ardrone init failed */
    QObject::connect(ardrone, SIGNAL(ardroneInitRes(int)), wd, SLOT(ardroneInitRes(int)));

	/* Start/stop the video/navdata thread */
    QObject::connect(ardrone, SIGNAL(navStart()), navdata, SLOT(navStart()));
    QObject::connect(ardrone, SIGNAL(navStop()), navdata, SLOT(navStop()));
	QObject::connect(ardrone, SIGNAL(videoStart()), video, SLOT(videoStart()));
	QObject::connect(ardrone, SIGNAL(videoStop()), video, SLOT(videoStop()));

	/* The result of video/navdata initialize */
	QObject::connect(navdata, SIGNAL(initNavStreamRes(int)), wd, SLOT(ardroneInitRes(int)));
	QObject::connect(video, SIGNAL(initVideoStreamRes(int)), wd, SLOT(ardroneInitRes(int)));

	QObject::connect(navdata, SIGNAL(startNavUpdate()), nav_update, SLOT(startNavUpdate()));
	QObject::connect(navdata, SIGNAL(stopNavUpdate()), nav_update, SLOT(stopNavUpdate()));

	QObject::connect(video, SIGNAL(startVideoUpdate()), video_update, SLOT(startVideoUpdate()));
	QObject::connect(video, SIGNAL(stopVideoUpdate()), video_update, SLOT(stopVideoUpdate()));

	QObject::connect(video_update, SIGNAL(videoUpdateStoped()), video, SLOT(videoUpdateStoped()));

	/* update the control state of control and nuiCtrl */
	QObject::connect(nav_update, SIGNAL(ctrlStateUpdate(const quint32)), control, SLOT(ctrlStateUpdate(const quint32)));
	QObject::connect(nav_update, SIGNAL(ctrlStateUpdate(const quint32)), nuiCtrl, SLOT(nuiCtrlStateUpdate(const quint32)));

	/* Show user interface */
    wd->show();

	/* Enter event loop */
    return app.exec();
}
