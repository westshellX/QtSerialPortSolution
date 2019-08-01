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

    void tryConnect();

private slots:
    void on_connectButton_clicked();
    void readStrGet();
    void displayError(QAbstractSocket::SocketError socketError);

    void on_checkBox_stateChanged(int arg1);

    void connectToServer();

private:

    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    bool isAutoConnect;     //是否自动连接
    Ui::Dialog *ui;
};

#endif // DIALOG_H
