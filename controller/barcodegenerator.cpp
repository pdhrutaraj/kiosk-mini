#include "barcodegenerator.h"

#include <MultiFormatWriter.h>
#include <BitMatrix.h>
#include <BarcodeFormat.h>

BarcodeGenerator::BarcodeGenerator(QObject* parent)
    : QObject(parent)
{
}

QImage BarcodeGenerator::generate(const QString& text, int size)
{
    using namespace ZXing;

    MultiFormatWriter writer(BarcodeFormat::QRCode);
    writer.setMargin(2);

    BitMatrix matrix = writer.encode(text.toStdString(), size, size);

    QImage img(matrix.width(), matrix.height(), QImage::Format_RGB32);
    img.fill(Qt::white);

    for (int y = 0; y < matrix.height(); ++y) {
        for (int x = 0; x < matrix.width(); ++x) {
            if (matrix.get(x, y)) {
                img.setPixel(x, y, qRgb(0, 0, 0));
            }
        }
    }

    return img;
}
