#ifndef ROLLMETER_H
#define ROLLMETER_H

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <math.h>

class RollMeter : public QWidget
{
    Q_OBJECT
public:
    RollMeter(QString title = "Roll", QWidget *parent = 0);
    ~RollMeter();

    void setSize(const int size);

    void updateValue(const float milliDegree);

    void setTitle(const QString title);

private:
    float       _milliDegree;
    float       _degree;
    int         _size;
    QString     _title;

    
signals:
    void draw();

protected:
    void paintEvent(QPaintEvent *);
    
};

#endif // ROLLMETER_H
