#include "altimeter.h"

AltiMeter::AltiMeter(QWidget *parent) :
    QWidget(parent)
{
    this->_meter = new QMeter("Altitude\n(m)");
    this->_meter->setBounds(0, 100);
    this->_size = 100;
    this->_altidute = 0;
    this->_layout = new QHBoxLayout();
    this->_layout->addWidget(this->_meter);
    this->setLayout(this->_layout);
}

AltiMeter::~AltiMeter()
{
    QObject::disconnect(this, SIGNAL(draw()), this, SLOT(showAltitude()));
}

void AltiMeter::setSize(const int size)
{
    this->_size = size;
    this->resize(size, size);
    this->setMinimumSize(size, size);
    this->_meter->setSize(size);
}

void AltiMeter::updateValue(const quint32 centiMeter)
{
    this->_centiMeter = centiMeter;
    this->_altidute = centiMeter / 100.0;
    this->_meter->updateValue(this->_altidute);
}
