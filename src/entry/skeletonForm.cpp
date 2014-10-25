#include "..//ENTRY/skeletonForm.h"

SkeletonForm::SkeletonForm(int width /* = 800 */, int height /* = 600 */, QWidget *parent /* = 0 */) :
	QWidget(parent), _width(width), _height(height)
{
	this->setGeometry(10, 100, _width, _height);
	this->setWindowTitle(tr("Skeleton View"));
	this->setWindowIcon(QIcon("../../src/resource/Kinect.ico"));
	QPalette palette;
	palette.setColor(QPalette::Background, Qt::black);
	this->setPalette(palette);

	QObject::connect(this, SIGNAL(draw()), this, SLOT(update()));
}

SkeletonForm::~SkeletonForm()
{

}

void SkeletonForm::drawBone(QPainter& painter, NUI_SKELETON_POSITION_INDEX joint0, NUI_SKELETON_POSITION_INDEX joint1)
{
	NUI_SKELETON_POSITION_TRACKING_STATE joint0State = this->_skel.eSkeletonPositionTrackingState[joint0];
	NUI_SKELETON_POSITION_TRACKING_STATE joint1State = this->_skel.eSkeletonPositionTrackingState[joint1];

	/* If we can't find either of these joints, exit */
	if (joint0State == NUI_SKELETON_POSITION_NOT_TRACKED || joint1State == NUI_SKELETON_POSITION_NOT_TRACKED)
	{
		return;
	}

	/* Don't draw if both points are inferred */
	if (joint0State == NUI_SKELETON_POSITION_INFERRED && joint1State == NUI_SKELETON_POSITION_INFERRED)
	{
		return;
	}

	/* We assume all drawn bones are inferred unless BOTH joints are tracked */
	if (joint0State == NUI_SKELETON_POSITION_TRACKED && joint1State == NUI_SKELETON_POSITION_TRACKED)
	{
		QPen trackedPen(Qt::green, 4, Qt::SolidLine);
		painter.setPen(trackedPen);
		painter.drawLine(this->_point[joint0], this->_point[joint1]);
	}
	else
	{
		QPen inferredPen(Qt::white, 1, Qt::SolidLine);
		painter.setPen(inferredPen);
		painter.drawLine(this->_point[joint0], this->_point[joint1]);
	}
}

/* Convert the point in Skeleton-space coordinate system to point in screen */
QPointF SkeletonForm::skeletonToScreen(Vector4 skeletonPoint)
{
	LONG x, y;
	USHORT depth;

	int width  = this->width();
	int height = this->height();

	NuiTransformSkeletonToDepthImage(skeletonPoint,&x, &y, &depth);

	float screenPointX = static_cast<float>(x * width) / WIDTH;
	float screenPointY = static_cast<float>(y * height) / HEIGHT;

	return QPointF(screenPointX, screenPointY);
}

/* emit a signal to draw the skeleton */
void SkeletonForm::drawSkeleton(const NUI_SKELETON_DATA & skel)
{
	for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; i++)
	{
		this->_point[i] = this->skeletonToScreen(skel.SkeletonPositions[i]);
	}

	this->_skel = skel;

	emit this->draw();
}

/* Paint the bone */
void SkeletonForm::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	
	// Render Torso
	this->drawBone(painter, NUI_SKELETON_POSITION_HEAD, NUI_SKELETON_POSITION_SHOULDER_CENTER);
	this->drawBone(painter, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_LEFT);
	this->drawBone(painter, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_RIGHT);
	this->drawBone(painter, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SPINE);
	this->drawBone(painter, NUI_SKELETON_POSITION_SPINE, NUI_SKELETON_POSITION_HIP_CENTER);
	this->drawBone(painter, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_LEFT);
	this->drawBone(painter, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_RIGHT);

	// Left Arm
	this->drawBone(painter, NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT);
	this->drawBone(painter, NUI_SKELETON_POSITION_ELBOW_LEFT, NUI_SKELETON_POSITION_WRIST_LEFT);
	this->drawBone(painter, NUI_SKELETON_POSITION_WRIST_LEFT, NUI_SKELETON_POSITION_HAND_LEFT);

	// Right Arm
	this->drawBone(painter, NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT);
	this->drawBone(painter, NUI_SKELETON_POSITION_ELBOW_RIGHT, NUI_SKELETON_POSITION_WRIST_RIGHT);
	this->drawBone(painter, NUI_SKELETON_POSITION_WRIST_RIGHT, NUI_SKELETON_POSITION_HAND_RIGHT);

	// Left Leg
	this->drawBone(painter, NUI_SKELETON_POSITION_HIP_LEFT, NUI_SKELETON_POSITION_KNEE_LEFT);
	this->drawBone(painter, NUI_SKELETON_POSITION_KNEE_LEFT, NUI_SKELETON_POSITION_ANKLE_LEFT);
	this->drawBone(painter, NUI_SKELETON_POSITION_ANKLE_LEFT, NUI_SKELETON_POSITION_FOOT_LEFT);

	// Right Leg
	this->drawBone(painter, NUI_SKELETON_POSITION_HIP_RIGHT, NUI_SKELETON_POSITION_KNEE_RIGHT);
	this->drawBone(painter, NUI_SKELETON_POSITION_KNEE_RIGHT, NUI_SKELETON_POSITION_ANKLE_RIGHT);
	this->drawBone(painter, NUI_SKELETON_POSITION_ANKLE_RIGHT, NUI_SKELETON_POSITION_FOOT_RIGHT);

}