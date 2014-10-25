#ifndef PITCHMETER_H
#define PITCHMETER_H

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <math.h>
#include "..//common/functional.h"

class PitchMeter : public QWidget
{
    Q_OBJECT
public:
    PitchMeter(QString title = "Pitch", QWidget *parent = 0);
    ~PitchMeter();

    void setSize(const int size);

    void setTitle(const QString title);

    void updateValue(const float milliDegree);

    void setMaxDegree(const int degree);

private:
    QString     _title;
    float       _degree;
    float       _milliDegree;
    int         _maxDegree;
    float       _distancePerValue;
    int         _size;

signals:
    void draw();

protected:
    void paintEvent(QPaintEvent *);
    
};

#endif // PITCHMETER_H
