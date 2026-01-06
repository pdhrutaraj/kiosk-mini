#include "barcodescanner.h"
#include "../model/barcodemodel.h"

#include <ReadBarcode.h>
#include <ImageView.h>

BarcodeScanner::BarcodeScanner(BarcodeModel* model, QObject* parent)
    : QObject(parent)
    , m_model(model)
{
}

void BarcodeScanner::scanImage(const QImage& image)
{
    ZXing::ImageView view(
        image.bits(),
        image.width(),
        image.height(),
        ZXing::ImageFormat::RGBX
    );

    auto result = ZXing::ReadBarcode(view);

    if (result.isValid() && m_model) {
        m_model->setResult(
            QString::fromStdString(result.text()),
            QString::fromStdString(ZXing::ToString(result.format()))
        );
    }
}
