#include "rollmeter.h"

RollMeter::RollMeter(QString title, QWidget *parent) :
    QWidget(parent), _title(title)
{
    this->_size = 100;
    this->_degree = 0.0;
    this->_milliDegree  = 0.0;

    QObject::connect(this, SIGNAL(draw()), this, SLOT(update()));
}

RollMeter::~RollMeter()
{
    QObject::disconnect(this, SIGNAL(draw()), this, SLOT(update()));
}

void RollMeter::setSize(const int size)
{
    this->_size = size;
    this->setMinimumSize(size, size);
    this->resize(size, size);
}

void RollMeter::updateValue(const float milliDegree)
{
    this->_milliDegree = milliDegree;
    this->_degree = milliDegree / 1000.0;
    emit this->draw();
}

void RollMeter::setTitle(const QString title)
{
    this->_title = title;
}

void RollMeter::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen textPen(Qt::black, 1, Qt::SolidLine);
    QPen hPen(Qt::red, 1, Qt::DotLine);
    QPen thickPen(Qt::red, 1.5, Qt::SolidLine);
    QPen thinPen(Qt::black, 0.5, Qt::SolidLine);
    const float r = this->_size / 2.0;

    painter.setRenderHint(QPainter::Antialiasing, true);
    int side = qMin(width(), height());
    painter.scale(side / (this->_size * 1.0),  side / (this->_size * 1.0) );

    // Draw the outside circle
    painter.setPen(QPen(Qt::black, 4, Qt::SolidLine));
    QPointF center(r, r);
    painter.drawEllipse(center, r - 2, r - 2);

    painter.translate(r, r);

    // Draw the scale
    painter.save();
    painter.rotate(-90);
    painter.setPen(hPen);
    painter.drawLine(0, 35 - r, 0, r - 35);
    for (int i = 0; i <= 90; i++)
    {
        if (0 == i || 90 == i || 30 == i || 60 == i)
        {
            painter.setPen(thickPen);
            painter.drawLine(0, 14 - r, 0, 6 - r);
            painter.setPen(textPen);
            painter.drawText(-15, 14 - r, 30, 30, Qt::AlignHCenter|
                             Qt::AlignTop, QString::number(i));
        }
        else if(0 == (i % 10))
        {
            painter.setPen(thickPen);
            painter.drawLine(0, 10 - r, 0, 6 - r);
        }
        else
        {
            painter.setPen(thinPen);
            painter.drawLine(0, 8 - r, 0, 6 - r);
        }
        painter.rotate(1);
    }

    for (int i = 89; i >= 0; i--)
    {
        if (0 == i || 90 == i || 30 == i || 60 == i)
        {
            painter.setPen(thickPen);
            painter.drawLine(0, 14 - r, 0, 6 - r);
            painter.setPen(textPen);
            painter.drawText(-15, 14 - r, 30, 30, Qt::AlignHCenter|
                             Qt::AlignTop , QString::number(i));
        }
        //else if()
        else if(0 == (i % 10))
        {
            painter.setPen(thickPen);
            painter.drawLine(0, 10 - r, 0, 6 - r);
        }
        else
        {
            painter.setPen(thinPen);
            painter.drawLine(0, 8 - r, 0, 6 - r);
        }
        painter.rotate(1);
    }


    for (int i = 1; i <= 90; i++)
    {
        if (0 == i || 90 == i || 30 == i || 60 == i)
        {
            painter.setPen(thickPen);
            painter.drawLine(0, 14 - r, 0, 6 - r);
            painter.setPen(textPen);
            painter.drawText(-15, 14 - r, 30, 30, Qt::AlignHCenter|
                             Qt::AlignTop, QString::number(-i));
        }
        else if(0 == (i % 10))
        {
            painter.setPen(thickPen);
            painter.drawLine(0, 10 - r, 0, 6 - r);
        }
        else
        {
            painter.setPen(thinPen);
            painter.drawLine(0, 8 - r, 0, 6 - r);
        }
        painter.rotate(1);
    }


    for (int i = 89; i > 0; i--)
    {
        if (0 == i || 90 == i || 30 == i || 60 == i)
        {
            painter.setPen(thickPen);
            painter.drawLine(0, 14 - r, 0, 6 - r);
            painter.setPen(textPen);
            painter.drawText(-15, 14 - r, 30, 30, Qt::AlignHCenter|
                             Qt::AlignTop, QString::number(-i));
        }
        else if(0 == (i % 10))
        {
            painter.setPen(thickPen);
            painter.drawLine(0, 10 - r, 0, 6 - r);
        }
        else
        {
            painter.setPen(thinPen);
            painter.drawLine(0, 8 - r, 0, 6 - r);
        }
        painter.rotate(1);
    }


    painter.restore();

    // Draw the plane image
    painter.rotate(this->_degree);
    float size = r - 22;
    QRect rect(-4.0 * size / sqrt(17.0), -size / sqrt(17.0), 8.0 * size / sqrt(17.0), 2.0 * size / sqrt(17.0));
    QPixmap map("../../src/resource/roll.png");
    painter.drawPixmap(rect, map);
}
