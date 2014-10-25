#ifndef NAVIGATER_H
#define NAVIGATER_H

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <math.h>

class Navigater : public QWidget
{
    Q_OBJECT
public:
    Navigater(QString title = "Navigater", QWidget *parent = 0);
    ~Navigater();

    void setSize(const int size);

    void updateValue(const float milliDegree);

    void setTitle(const QString title);

private:
    int     _size;

    // milli-degrees
    float   _milliDegree;
    float   _degree;
    QString _title;
    bool    _setup;
    
signals:
    void draw();
    
public slots:

protected:
    void paintEvent(QPaintEvent *);
    
};

#endif // NAVIGATER_H
