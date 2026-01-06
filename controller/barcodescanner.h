#pragma once

#include <QObject>
#include <QImage>

class BarcodeModel;

class BarcodeScanner : public QObject
{
    Q_OBJECT

public:
    explicit BarcodeScanner(BarcodeModel* model, QObject* parent = nullptr);

    void scanImage(const QImage& image);

private:
    BarcodeModel* m_model;
};
