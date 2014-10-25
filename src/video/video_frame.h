#ifndef VIDEO_FRAME_H
#define VIDEO_FRAME_H

#include <QtGui/QWidget>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QtCore/QSemaphore>
#include <QtCore/QDateTime>
#include <QtCore/QFile>
#include <QtCore/QByteArray>
#include <QtCore/QBuffer>

class videoFrame : public QWidget
{
	 Q_OBJECT
public:

	videoFrame(int width = 640, int height = 360, QWidget  *parent = 0);
	~videoFrame();

	/* update the video image */
	void updateImage(QImage img);

	/* return the width of the video frame*/
	int getWidth();

	/* return the height of the frame */
	int getHeight();

	/* take a snapshot, write the image to disk */
	void snapShot();

private:
	int				_height;
	int				_width;
	QImage			_img;
	QSemaphore		*_sem;


protected:
	void paintEvent(QPaintEvent *);

signals:
	void draw();

};

#endif // VIDEO_FRAME_h