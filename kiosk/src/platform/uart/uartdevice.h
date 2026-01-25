#pragma once
#include <QObject>
#include <QSerialPort>

class UartDevice : public QObject
{
    Q_OBJECT
public:
    explicit UartDevice(QObject *parent = nullptr);
    bool open(const QString &port);
    void send(const QByteArray &data);

signals:
    void bytesReceived(QByteArray data);

private slots:
    void onReadyRead();

private:
    QSerialPort serial;
};

