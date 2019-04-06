#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QDataStream>
#include <QtNetwork>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_connectButton_clicked();
    void readStrGet();
    void displayError(QAbstractSocket::SocketError socketError);

private:

    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;

    Ui::Dialog *ui;
};

#endif // DIALOG_H
