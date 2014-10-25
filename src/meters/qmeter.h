#ifndef QMETER_H
#define QMETER_H

#include <QtGui/QWidget>
#include <QtGui/QPainter>

class QMeter : public QWidget
{
    Q_OBJECT
public:
    QMeter(QString title = "Meter");
    ~QMeter();

    void setBounds(const int lower, const int upper);

    void setSize(const int size);

    void updateValue(const quint32 value);

    void setTitle(const QString title);

    void setDegree(const int degree);

private:
    quint32   _value;
    int       _lower;
    int       _upper;
    int       _size;
    int       _degree;
    float     _degreePerValue;
    QString   _title;

protected:
    void paintEvent(QPaintEvent *);

signals:
    void draw();
    
};

#endif // QMETER_H
