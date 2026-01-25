#include "protocolparser.h"

ProtocolParser::ProtocolParser(QObject *parent) : QObject(parent) {}

void ProtocolParser::feed(const QByteArray &data)
{
    for (uint8_t b : data)
    {
        switch (state)
        {
        case RxState::WaitControl:
            control = b;
            state = RxState::WaitLength;
            break;

        case RxState::WaitLength:
            length = b;
            payload.clear();
            state = RxState::WaitPayload;
            break;

        case RxState::WaitPayload:
            payload.append(b);
            if (payload.size() == length) {
                decode(control, payload);
                state = RxState::WaitControl;
            }
            break;
        }
    }
}

//frame decode
void ProtocolParser::decode(quint8 ctrl, const QByteArray &d)
{
    switch (ctrl)
    {
    case 0xF2: emit pulseRate(uint8_t(d[0])); break;

    case 0xF4: emit spo2(uint8_t(d[0])); break;

    case 0xFA: {
        float temp = float(d[0]) + float(d[1]) / 10.0f;
        emit temperature(temp);
        break;
    }

    case 0xF8: {
        float w = float(d[0]) + float(d[1]) / 10.0f;
        emit weight(w);
        break;
    }

    case 0xF7: emit height(uint8_t(d[0])); break;

    default:
        break;
    }
}

