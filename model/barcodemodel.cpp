#include "barcodemodel.h"


BarcodeModel::BarcodeModel(QObject* parent)
: QObject(parent) {}


QString BarcodeModel::text() const { return m_text; }
QString BarcodeModel::format() const { return m_format; }


void BarcodeModel::setResult(const QString& text, const QString& format)
{
m_text = text;
m_format = format;
emit barcodeUpdated();
}


void BarcodeModel::clear()
{
m_text.clear();
m_format.clear();
emit barcodeUpdated();
}
