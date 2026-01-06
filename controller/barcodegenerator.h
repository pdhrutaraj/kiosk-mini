#pragma once
#include <QObject>
#include <QImage>
#include <QString>

class BarcodeGenerator : public QObject {
    Q_OBJECT
public:
    explicit BarcodeGenerator(QObject* parent = nullptr);

    QImage generate(const QString& text, int size = 256);
};
