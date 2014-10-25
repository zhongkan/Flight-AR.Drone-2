/***********************************************************************************
 * @file    control.h
 * @author  kzhong1991<kzhong1991@gmail.com>
 * @date    2012-12-11
 * @brief   The Control class, control the ardrone with keyboard or joystick
 ***********************************************************************************/

#ifndef CONTROL_H
#define CONTROL_H

#include <QtCore/QThread>
#include <QtGui/QKeyEvent>
#include <QtCore/QSemaphore>
#include "..//common/ardrone.h"
#include "..//common/functional.h"
#include "..//common/config.h"
#include "..//ctrl/control_states.h"

const float eps = 1e-6f;

#define KEY_TAKEOFF_LAND     Qt::Key_Space

#define KEY_GOUP             Qt::Key_I		// Make the ardrone go up
#define KEY_GODOWN           Qt::Key_K		// Make the ardrone go down
#define KEY_ROTATELEFT       Qt::Key_L		// Make the ardrone rotate left
#define KEY_ROTATERIGHT      Qt::Key_J		// Make the ardrone rotate right

#define KEY_GOLEFT           Qt::Key_A		// Make the ardrone go left
#define KEY_GORIGHT          Qt::Key_D		// Make the ardrone go right
#define KEY_FORWARD          Qt::Key_W		// Make the ardrone go forward
#define KEY_BACKWARD         Qt::Key_S		// Make the ardrone go backward

#define KEY_EMERGENCY        Qt::Key_F11    // Set the ardrone to emergency mode, it will cut off the engine immediately

#define KEY_CTRL_ENABLE		 Qt::Key_F12	// Enable or Disable the keyboard or joystick control

class Control : public QThread
{
    Q_OBJECT
public:
    Control(ARDrone * ardrone, QObject *parent = 0);
    ~Control();

    /* ardrone movement control */
    void takeOff();
    void landing();
    void emergency();
    void goUp(const float speed);     /* gaz+ */
    void goDown(const float speed);   /* gaz- */
    void goLeft(const float speed);   /* roll- */
    void goRight(const float speed);  /* roll+ */
    void rotateL(const float speed);   /* yaw- */
    void rotateR(const float speed);   /* yaw+ */
    void forward(const float speed);  /* pitch+ */
    void backward(const float speed); /* pitch- */
    void hovering();

    /* enable/disable the keyboard control */
    void keyBoardCtrlEnable(const bool enable);

    /* keyboard control */
    void keyPressed(QKeyEvent * evt);
    void keyReleased(QKeyEvent * evt);

    /* set the flying speed : [-1, 1] */
    void setSpeed(const float speed);

	/* start the control thread */
	void go();

	/* stop the control thread */
    void stop();

private:
    ARDrone *       _ardrone;
    bool            _keyboard;
    QKeyEvent *     _evt;
    float           _speed;
    quint32         _ctrl_state;
    quint32         _state;

    bool            _stop;

    QSemaphore *    _semaphore;

    /* roll, pitch, gaz, yaw */
    float           _last_movement[4];

    /* roll, pitch, gaz, yaw */
    int             _movement[4];

	bool			_pcmd;

protected:
    void run();

signals:
    
public slots:
    void ctrlStateUpdate(const quint32 ctrl_state);
    
};
#endif // CONTROL_H
