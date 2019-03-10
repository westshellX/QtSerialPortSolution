#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:

    void on_btnOpenSerial_clicked();
    void serial_read();
    void on_pushButtonTxData_clicked();

    void on_pushButtonCleanRx_clicked();

    void on_pushButtonCleanTx_clicked();

private:
    void enableUI(bool enable );
    Ui::Dialog *ui;
    QSerialPort  mSerialPort;
    bool mSerialOpened;
};

#endif // DIALOG_H
