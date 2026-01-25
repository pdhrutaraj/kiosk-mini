#pragma once
#include <QObject>

class VitalsModel : public QObject
{
    Q_OBJECT
public:
    explicit VitalsModel(QObject *parent = nullptr);

signals:
    void spo2Changed(int);
    void pulseRateChanged(int);
    void temperatureChanged(float);
    void weightChanged(float);
    void heightChanged(int);

public slots:
    void setSpo2(int);
    void setPulseRate(int);
    void setTemperature(float);
    void setWeight(float);
    void setHeight(int);
};

