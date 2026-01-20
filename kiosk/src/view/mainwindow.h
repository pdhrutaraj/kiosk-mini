#pragma once
#include <QMainWindow>
#include <QImage>
#include <QString>
#include "controller/cameracontroller.h"
#include "controller/barcodescanner.h"
#include "model/barcodemodel.h"
#include "storage/sqliterecorder.h"


class QLabel;
class QLineEdit;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

public slots:
    void onGenerateQR();
    void onPrintQR();
    void updateFrame(const QImage& img);
    void updateBarcode(const QString& text, const QString& format);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    void resetUI();
    QImage  m_frame;
    QString m_text;
    QString m_format;
    QImage currentQR;
    QLineEdit* inputEdit;
    QLabel* qrLabel;
    QPushButton* generateBtn;
    QPushButton* printBtn;
    CameraController* camera;
    BarcodeScanner*  scanner;
    BarcodeModel*    model;
    SQLiteRecorder*  db;
    QPushButton* scanBtn;
    bool scannerRunning = false;
    
};



