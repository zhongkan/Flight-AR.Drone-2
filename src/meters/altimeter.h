#ifndef ALTIMETER_H
#define ALTIMETER_H

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include "qmeter.h"

class AltiMeter : public QWidget
{
    Q_OBJECT
public:
    AltiMeter(QWidget *parent = 0);
    ~AltiMeter();

    void setSize(const int size);

    void updateValue(const quint32 altitude);

private:
    QHBoxLayout *   _layout;
    quint32         _centiMeter;
    float           _altidute;
    int             _size;
    QMeter *        _meter;
};

#endif // ALTIMETER_H
