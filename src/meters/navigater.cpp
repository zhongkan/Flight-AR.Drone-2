#include "navigater.h"

Navigater::Navigater(QString title, QWidget *parent) :
     QWidget(parent),_title(title)
{
    this->_size = 100;
    this->_degree = 0.0;
    this->_milliDegree = 0.0;
    QObject::connect(this, SIGNAL(draw()), this, SLOT(update()));
}

Navigater::~Navigater()
{
    QObject::disconnect(this, SIGNAL(draw()), this, SLOT(update()));
}
void Navigater::setSize(const int size)
{
    this->_size = size;
    this->resize(size, size);
    this->setMinimumSize(size, size);
}

void Navigater::setTitle(const QString title)
{
    this->_title = title;
}

void Navigater::updateValue(const float milliDegree)
{
    this->_milliDegree = -milliDegree;
    this->_degree = milliDegree / 1000.0;
    emit this->draw();
}

void Navigater::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen thickPen(Qt::red, 1.5, Qt::SolidLine);
    QPen textPen(Qt::black, 4, Qt::SolidLine);
    QString dir[] = {"N", "E", "S", "W"};
    QFont font = painter.font();
    font.setPointSize(12);
    const float r = this->_size / 2.0;

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(textPen);
    int side = qMin(width(), height());
    painter.scale(side / (this->_size * 1.0),  side / (this->_size * 1.0) );

    // Draw the outside circle
    QPointF center(r, r);
    painter.drawEllipse(center, r - 2, r - 2);
    painter.translate(r, r);
    painter.save();

    // Draw scale
    for (int i = 0; i < 4; i++)
    {
        painter.setPen(thickPen);
        painter.drawLine(0, 10 - r, 0, 6 - r);
        painter.setPen(textPen);
        painter.setFont(font);
        painter.drawText(-15, 10 - r, 30, 30, Qt::AlignHCenter |
                         Qt::AlignTop,  dir[i]);
        painter.rotate(90);
    }

    painter.restore();

    // Draw the air plane image
    painter.rotate(this->_degree);
    float size = r - 16;
    QRect rect(-size / sqrt(2.0), -size / sqrt(2.0), 2.0 * size / sqrt(2.0), 2.0 * size / sqrt(2.0));
    QPixmap map("../../src/resource/yaw.png");
    painter.drawPixmap(rect, map);
}
