#include "qmeter.h"

QMeter::QMeter(QString title) :
    _title(title)
{
    this->_lower = 0;
    this->_upper = 0;
    this->_value = 0;
    this->_size  = 100;
    this->_degree = 240;
    this->_degreePerValue = 0.0;
    QObject::connect(this, SIGNAL(draw()), this, SLOT(update()));
}

QMeter::~QMeter()
{
    QObject::disconnect(this, SIGNAL(draw()), this, SLOT(update()));
}

void QMeter::setBounds(const int lower, int upper)
{
    this->_lower = lower;
    this->_upper = upper;
    this->_degreePerValue = (this->_degree * 1.0) / (upper - lower);
}

void QMeter::updateValue(const quint32 value)
{
    this->_value = value;
    emit this->draw();
}

void QMeter::setSize(const int size)
{
    this->_size = size;
    this->resize(size, size);
    this->setMinimumSize(size, size);
}

void QMeter::setTitle(const QString title)
{
    this->_title = title;
}

void QMeter::setDegree(const int degree)
{
    this->_degree = degree;
}

void QMeter::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen thickPen(Qt::red, 1.5, Qt::SolidLine);
    QPen thinPen(Qt::black, 0.5, Qt::SolidLine);
    QPen textPen(Qt::black, 1, Qt::SolidLine);
    const float r = this->_size / 2.0;

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 4, Qt::SolidLine));
    int side = qMin(width(), height());
    painter.scale(side / (this->_size * 1.0),  side / (this->_size * 1.0) );


    // Draw the outside circle
    QPointF center(r, r);
    painter.drawEllipse(center, r - 2, r - 2);


    static const QPoint point[3] = {
        QPoint(r / 25, 4 * r / 25),
        QPoint(-r / 25, 4 * r / 25),
        QPoint(0, 30 - r)
    };

    // Draw the pointer
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::red);
    painter.drawEllipse(center, r / 16, r / 16);
    painter.translate(r, r);
    painter.save(); // Save the painter state twice
    painter.save();
    painter.rotate(this->_value * this->_degreePerValue - this->_degree / 2.0);
    painter.drawConvexPolygon(point, 3);


    // Draw the scale
    painter.restore();
    painter.rotate(-this->_degree / 2.0);
    for (int i = this->_lower; i <= this->_upper; i++)
    {
        if (0 == i % 10)
        {
            painter.setPen(thickPen);
            painter.drawLine(0, 14 - r, 0, 6 - r);
            painter.setPen(textPen);
            painter.drawText(-15, 14 - r, 30, 30, Qt::AlignHCenter|
                             Qt::AlignTop, QString::number(i));

        }
        else if (0 == i % 5 && 0 != i % 10)
        {
            painter.setPen(thickPen);
            painter.drawLine(0, 10 - r, 0, 6 - r);
        }
        else
        {
            painter.setPen(thinPen);
            painter.drawLine(0, 8 - r, 0, 6 - r);
        }
        painter.rotate(this->_degreePerValue);
    }


    painter.restore();
    painter.setPen(textPen);
    painter.drawText(-50, 25, 100, 100, Qt::AlignHCenter|
                     Qt::AlignTop, this->_title);
}

