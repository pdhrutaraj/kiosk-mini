#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include "model/barcodemodel.h"
#include "controller/barcodegenerator.h"
#include "controller/barcodescanner.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void generateQR();
    void onBarcodeUpdated();

private:
    BarcodeModel* m_model;
    BarcodeGenerator* m_generator;
    BarcodeScanner* m_scanner;

    QLabel* m_imageLabel;
    QLabel* m_textLabel;
     QLineEdit* m_inputEdit;   // ✅ NEW

};

#endif // MAINWINDOW_H
