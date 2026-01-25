#include "uartdevice.h"

UartDevice::UartDevice(QObject *parent) : QObject(parent)
{
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    connect(&serial, &QSerialPort::readyRead,
            this, &UartDevice::onReadyRead);
}

bool UartDevice::open(const QString &port)
{
    serial.setPortName(port);
    return serial.open(QIODevice::ReadWrite);
}

void UartDevice::send(const QByteArray &data)
{
    serial.write(data);
}

void UartDevice::onReadyRead()
{
    emit bytesReceived(serial.readAll());
}

