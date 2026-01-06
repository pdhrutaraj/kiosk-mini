#pragma once
#include <QObject>
#include <QString>


class BarcodeModel : public QObject {
Q_OBJECT
public:
explicit BarcodeModel(QObject* parent = nullptr);


QString text() const;
QString format() const;


public slots:
void setResult(const QString& text, const QString& format);
void clear();


signals:
void barcodeUpdated();


private:
QString m_text;
QString m_format;
};
