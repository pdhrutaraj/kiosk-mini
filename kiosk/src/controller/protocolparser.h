#pragma once
#include <QObject>
#include <QByteArray>

class ProtocolParser : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolParser(QObject *parent = nullptr);
    void feed(const QByteArray &data);

signals:
    void pulseRate(int);
    void spo2(int);
    void temperature(float);
    void weight(float);
    void height(int);

private:
    enum class RxState {
        WaitControl,
        WaitLength,
        WaitPayload
    };

    RxState state = RxState::WaitControl;
    quint8 control = 0;
    quint8 length = 0;
    QByteArray payload;

    void decode(quint8 ctrl, const QByteArray &data);
};

