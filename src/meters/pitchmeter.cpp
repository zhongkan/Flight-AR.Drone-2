#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QDebug>
#include "pitchmeter.h"
#include <iostream>

PitchMeter::PitchMeter(QString title, QWidget *parent) :
    QWidget(parent), _title(title)
{
    this->_size = 100;
    this->_milliDegree = 0.0;
    this->_degree = 0.0;
    this->_maxDegree = 20;
    this->_distancePerValue = (this->_size / 2.0 * sqrt(3.0)) / (2.0 * this->_maxDegree);

    QObject::connect(this, SIGNAL(draw()), this, SLOT(update()));
}

PitchMeter::~PitchMeter()
{
    QObject::disconnect(this, SIGNAL(draw()), this, SLOT(update()));
}

void PitchMeter::setSize(const int size)
{
    this->_size = size;
    this->resize(size, size);
    this->setMinimumSize(size, size);
}

void PitchMeter::setTitle(const QString title)
{
    this->_title = title;
}

void PitchMeter::updateValue(const float milliDegree)
{
    this->_milliDegree = milliDegree;
    this->_degree = milliDegree / 1000.0;
    emit draw();
}

void PitchMeter::setMaxDegree(const int degree)
{
    this->_maxDegree = degree;
    this->_distancePerValue = (this->_size / 2.0 * sqrt(3.0)) / (2.0 * this->_maxDegree);
}

void PitchMeter::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen textPen(Qt::black, 1, Qt::SolidLine);
    QPen rlinePen(Qt::red, 1, Qt::SolidLine);
    QPen blinePen(Qt::black, 1, Qt::SolidLine);
    const float r = this->_size / 2.0;
    const float upper = r / 2.0 * sqrt(2.0);
    const float c = 2.0f / 3.0f;

    painter.setRenderHint(QPainter::Antialiasing, true);
    int side = qMin(width(), height());
    painter.scale(side / (this->_size * 1.0),  side / (this->_size * 1.0) );

    // Draw the outside circle
    painter.setPen(QPen(Qt::black, 4, Qt::SolidLine));
    QPointF center(r, r);
    painter.drawEllipse(center, r - 2, r - 2);

    painter.translate(r, r);

    // Draw two triangle
    const QPointF point1[3] = {
        QPoint(-r * c , 0),
        QPoint(-r * c - 10 * sqrt(3.0), -10),
        QPoint(-r * c - 10 * sqrt(3.0),  10)

    };

    const QPointF point2[3] = {
        QPoint(r * c, 0),
        QPoint(r * c + 10 * sqrt(3.0), -10),
        QPoint(r * c + 10 * sqrt(3.0),  10)
    };

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawConvexPolygon(point1, 3);
    painter.drawConvexPolygon(point2, 3);

    // Draw scale
    const float offset = this->_degree * this->_distancePerValue;
    float distance  = 0.0;
    painter.translate(0.0, offset);
    painter.save();


    for(int i = 0; ; i++)
    {
        if (distance - offset > upper)
            break;

        if (0 == (i % 10))
        {
            painter.setPen(textPen);
            painter.drawText(-r * c, -15, 30, 30, Qt::AlignVCenter | Qt::AlignLeft, itos(i, 2));

            painter.drawText(r * c - 30 , -15, 30, 30, Qt::AlignVCenter | Qt::AlignRight, itos(i, 2));

            painter.setPen(rlinePen);
            painter.drawLine(-r * c + 24, 0.0, r * c - 24, 0.0);
        }
        else if (0 == (i % 5) && 0 != (i % 10))
        {
            painter.setPen(blinePen);
            painter.drawLine(-r * c + 30, 0.0, r * c - 30, 0.0);
        }

        painter.translate(0.0, -this->_distancePerValue);
        distance += this->_distancePerValue;
    }

    painter.restore();
    distance = 0.0;


    for (int i = 0; ;i++)
    {
        if (distance + offset > upper)
            break;

        if (0 == (i % 10))
        {
            painter.setPen(textPen);
            painter.drawText(-r * c, -15, 30, 30, Qt::AlignVCenter | Qt::AlignLeft, itos(-i, 2));

            painter.drawText(r * c - 30 , -15, 30, 30, Qt::AlignVCenter | Qt::AlignRight, itos(-i, 2));

            painter.setPen(rlinePen);
            painter.drawLine(-r * c + 24, 0.0, r * c - 24, 0.0);
        }
        else if (0 == (i % 5) && 0 != (i % 10))
        {
            painter.setPen(blinePen);
            painter.drawLine(-r * c + 30, 0.0, r * c - 30, 0.0);
        }

        painter.translate(0.0, this->_distancePerValue);
        distance += this->_distancePerValue;
    }
}
