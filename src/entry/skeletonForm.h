/***********************************************************************************
* @file    skeletonForm.h
* @author  kzhong1991<kzhong1991@gmail.com>
* @date    2013-1-25
* @brief   Show the skeleton
 ***********************************************************************************/
#ifndef SKELETON_FORM_H
#define SKELETON_FORM_H

#include <QtGui/QWidget>
#include <QtGui/QIcon>
#include <QtGui/QPainter>
#include <windows.h>
#include "NuiApi.h"

/************************************************************************/
/* NuiTransformSkeletonToDepthImage returns coordinates in 
/* NUI_IMAGE_RESOLUTION_320x240 space                                                                     */
/************************************************************************/
#define  WIDTH		320
#define  HEIGHT		240

class SkeletonForm : public QWidget
{
	Q_OBJECT
public:
	SkeletonForm(int width = 500, int height = 500, QWidget *parent = 0);
	~SkeletonForm();

	/// <summary>
	/// Draws a skeleton
	/// </summary>
	/// <param name="skel">skeleton to draw</param>
	void		drawSkeleton(const NUI_SKELETON_DATA & skel);

	/// <summary>
	/// Draws a bone line between two joints
	/// </summary>
	/// <param name="painter">painter to draw bones</param>
	/// <param name="joint0">joint to start drawing from</param>
	/// <param name="joint1">joint to end drawing at</param>
	void		drawBone(QPainter  & painter, NUI_SKELETON_POSITION_INDEX joint0, NUI_SKELETON_POSITION_INDEX joint1);

	/// <summary>
	/// Converts a skeleton point to screen space
	/// </summary>
	/// <param name="skeletonPoint">skeleton point to tranform</param>
	/// <returns>point in screen-space</returns>
	QPointF		skeletonToScreen(Vector4 skeletonPoint);

	/* The skeleton position in screen */
	QPointF				_point[NUI_SKELETON_POSITION_COUNT];

private:
	int					_width;
	int					_height;

	NUI_SKELETON_DATA	_skel;

protected:
	void paintEvent(QPaintEvent *);

signals:
	void draw();

};

#endif //SKELETON_FORM_H