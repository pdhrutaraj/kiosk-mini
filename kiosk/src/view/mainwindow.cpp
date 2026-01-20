#include "mainwindow.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>

#include <QHBoxLayout>
#include <QFrame>
#include <QSpacerItem>


#include "model/qrgenerator.h"
#include "controller/printcontroller.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // Setup UI
    setStyleSheet(R"(
QMainWindow {
    background-color: #121212;
    color: #ffffff;
}
QLineEdit {
    border-radius: 6px;
    padding: 6px;
    font-size: 16px;
}
QPushButton {
    background-color: #2d89ef;
    border-radius: 6px;
    font-size: 15px;
    color: white;
}
QPushButton:hover {
    background-color: #1b5fbf;
}
QPushButton:pressed {
    background-color: #144a96;
}
)");

    /* ---------- CENTRAL WIDGET ---------- */
QWidget* central = new QWidget(this);
setCentralWidget(central);

QVBoxLayout* mainLayout = new QVBoxLayout(central);
mainLayout->setContentsMargins(10, 10, 10, 10);
mainLayout->setSpacing(8);

/* ---------- CAMERA PREVIEW ---------- */
QFrame* previewFrame = new QFrame;
previewFrame->setMinimumHeight(360);
previewFrame->setFrameShape(QFrame::StyledPanel);
//previewFrame->setStyleSheet(
//    "QFrame { background-color: #000; border-radius: 8px; }"
//);

mainLayout->addWidget(previewFrame, 1);  // expands

/* ---------- CONTROL BAR ---------- */
QFrame* controlBar = new QFrame;
//controlBar->setFixedHeight(120);
controlBar->setFixedHeight(160);
controlBar->setStyleSheet(
    "QFrame { background-color: #1e1e1e; border-radius: 10px; }"
);

mainLayout->addWidget(controlBar);

/* ---------- CONTROL BAR LAYOUT ---------- */
QHBoxLayout* barLayout = new QHBoxLayout(controlBar);
barLayout->setContentsMargins(12, 12, 12, 12);
barLayout->setSpacing(10);

/* ---------- INPUT FIELD ---------- */
inputEdit = new QLineEdit;
inputEdit->setPlaceholderText("Scanned / Enter text");
inputEdit->setMinimumHeight(40);

/* ---------- BUTTONS ---------- */
generateBtn = new QPushButton("Generate QR");
printBtn    = new QPushButton("Print QR");
scanBtn     = new QPushButton("Start Scanner");

for (auto btn : {generateBtn, printBtn, scanBtn}) {
    btn->setMinimumHeight(40);
    btn->setMinimumWidth(140);
}

/* ---------- QR PREVIEW ---------- */
qrLabel = new QLabel;
qrLabel->setFixedSize(96, 96);
qrLabel->setStyleSheet(
    "QLabel { background:#fff; border-radius:6px; }"
);
qrLabel->setAlignment(Qt::AlignCenter);

/* ---------- ASSEMBLE BAR ---------- */
barLayout->addWidget(qrLabel);
barLayout->addWidget(inputEdit, 1);
barLayout->addWidget(generateBtn);
barLayout->addWidget(printBtn);
barLayout->addWidget(scanBtn);

    connect(generateBtn, &QPushButton::clicked,
            this, &MainWindow::onGenerateQR);

    connect(printBtn, &QPushButton::clicked,
            this, &MainWindow::onPrintQR);
    //add scanner
    /* ---- create backend objects ---- */
camera  = new CameraController("/dev/video0", 640, 480, this);
scanner = new BarcodeScanner(this);
model   = new BarcodeModel(this);
db      = new SQLiteRecorder("barcodes.db", this);

/* ---- UI ---- */
//scanBtn = new QPushButton("Start Scanner");
//layout->addWidget(scanBtn);

/* ---- connections (SAME AS YOUR WORKING CODE) ---- */

// Preview
connect(camera, &CameraController::frameColorReady,
        this, &MainWindow::updateFrame);

// Grayscale → scanner
connect(camera, &CameraController::frameGrayReady,
        scanner, &BarcodeScanner::onFrame);

// Scanner → model
connect(scanner, &BarcodeScanner::barcodeDetected,
        model, &BarcodeModel::setBarcode);

// Model → UI + DB
connect(model, &BarcodeModel::barcodeChanged, this, [=]() {
    updateBarcode(model->text(), model->format());
    db->store(model->text(), model->format());
});
//scanner button toggle logic
//scannerRunning = false;
connect(scanBtn, &QPushButton::clicked, this, [this]() {

    if (!scannerRunning) {

        /* ---- CREATE CAMERA FRESH ---- */
        
        camera = new CameraController("/dev/video0", 640, 480, this);

        connect(camera, &CameraController::frameColorReady,
                this, &MainWindow::updateFrame);

        connect(camera, &CameraController::frameGrayReady,
                scanner, &BarcodeScanner::onFrame);

        camera->start();

        scanBtn->setText("Stop Scanner");
        //scanBtn->setText("Done...");
        scannerRunning = true;
        //scannerRunning = false;

    } else {
    //if (scannerRunning) {

        /* ---- STOP & DESTROY CAMERA ---- */
        scannerRunning = false;
        //scannerRunning = true;
        camera->stop();
        camera->wait();     // VERY IMPORTANT
        camera->deleteLater();
        camera = nullptr;
        update(); 
        scanBtn->setText("Start Scanner");
        resetUI();

    }
});

}

/* ---------- QR GENERATION ---------- */
void MainWindow::onGenerateQR()
{
    QString text = inputEdit->text();
    if (text.isEmpty())
        return;

    currentQR = QRGenerator::generate(text, 256);
    update();   
}

/*
void MainWindow::onGenerateQR()
{
    m_text = inputEdit->text();
    if (m_text.isEmpty())
        return;

    currentQR = QRGenerator::generate(m_text, 256);
    qrLabel->setPixmap(QPixmap::fromImage(currentQR));
}
*/
//update baercode into ui
void MainWindow::updateBarcode(const QString& text,
                               const QString& format)
{
    if (!scannerRunning)
        return;

    inputEdit->setText(text);
    currentQR = QRGenerator::generate(text, 256);
    qrLabel->setPixmap(QPixmap::fromImage(currentQR));
}

/* ---------- PRINT QR ---------- */
void MainWindow::onPrintQR()
{
    if (currentQR.isNull())
        return;

    PrinterController::printQR(currentQR, m_text);
}

/* ---------- CAMERA FRAME UPDATE ---------- */
void MainWindow::updateFrame(const QImage& img)
{
    if (!scannerRunning || img.isNull())
        return;

    m_frame = img;
    update();
}

/* ---------- PAINT EVENT (CAMERA PREVIEW) ---------- */

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);

    // Preview area = window minus control bar
    QRect previewRect = centralWidget()->geometry();
    previewRect.setBottom(previewRect.bottom() - 140);

    p.fillRect(previewRect, Qt::black);

    /* ---------- SCANNER ON → CAMERA PREVIEW ---------- */
    if (scannerRunning && !m_frame.isNull()) {

        QImage scaled = m_frame.scaled(
            previewRect.size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        );

        QPoint center = previewRect.center() -
                        QPoint(scaled.width()/2, scaled.height()/2);

        p.drawImage(center, scaled);
        return;
    }

    /* ---------- SCANNER OFF → SHOW QR ---------- */
    if (!scannerRunning && !currentQR.isNull()) {

        int size = qMin(previewRect.width(),
                        previewRect.height()) * 0.7;

        QImage scaledQR = currentQR.scaled(
            size, size,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        );

        QPoint center = previewRect.center() -
                        QPoint(scaledQR.width()/2, scaledQR.height()/2);

        p.drawImage(center, scaledQR);
        return;
    }

    /* ---------- IDLE STATE ---------- */
    p.setPen(Qt::gray);
    p.setFont(QFont("Sans", 18));
    p.drawText(previewRect,
               Qt::AlignCenter,
               "Ready");
}


/*
void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::SmoothPixmapTransform);

    // Preview area = top part of window
    QRect previewRect = centralWidget()->geometry();
    previewRect.setBottom(previewRect.bottom() - 140); // control bar height

    ///---------------- CAMERA MODE ---------------- 
    if (scannerRunning && !m_frame.isNull()) {

        QImage scaled = m_frame.scaled(
            previewRect.size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        );

        QPoint center = previewRect.center() -
                        QPoint(scaled.width() / 2,
                               scaled.height() / 2);


        
        p.drawImage(center, scaled);
        return;
    }

    // ---------------- QR DISPLAY MODE ---------------- 
    if (!scannerRunning && !currentQR.isNull()) {

        int qrSize = qMin(previewRect.width(),
                          previewRect.height()) * 0.6;

        QImage scaledQR = currentQR.scaled(
            qrSize, qrSize,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        );

        QPoint center = previewRect.center() -
                        QPoint(scaledQR.width() / 2,
                               scaledQR.height() / 2);

        p.fillRect(previewRect, Qt::black);
        p.drawImage(center, scaledQR);
        return;
    }

    //---------------- IDLE SCREEN ---------------- 
    p.fillRect(previewRect, QColor("#000000"));
    p.setPen(Qt::gray);
    p.setFont(QFont("Sans", 18, QFont::Medium));
    p.drawText(previewRect, Qt::AlignCenter,
               "Ready\nWelcome to QR generator & QR Scanner");
}
*/
/*
void MainWindow::paintEvent(QPaintEvent*)
{
    if (m_frame.isNull())
        return;

    QPainter p(this);

    QRect target = centralWidget()->geometry();
    target.setBottom(target.bottom() - 140); // leave space for controls

    p.drawImage(target, m_frame.scaled(
        target.size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    ));
}
*/
/*
void MainWindow::paintEvent(QPaintEvent* event)
{
    QMainWindow::paintEvent(event);

    if (m_frame.isNull())
        return;

    QPainter painter(this);
    QRect r = rect().adjusted(10, 10, -10, -10);

    painter.drawImage(r, m_frame.scaled(
        r.size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation));
}
*/
void MainWindow::resetUI()
{
    // Clear preview frame
    m_frame = QImage();
    update();   // repaint window

    // Clear barcode text
    inputEdit->clear();

    // Clear QR image
    currentQR = QImage();
    qrLabel->clear();

    // Optional: reset status labels if any
    // statusLabel->setText("Ready");

    m_text.clear();
    m_format.clear();
}
