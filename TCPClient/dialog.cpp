#include "dialog.h"
#include "ui_dialog.h"

#include <QDebug>
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    tcpSocket(new QTcpSocket(this)),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    // find out name of this machine
    QString name = QHostInfo::localHostName();
    if (!name.isEmpty()) {
        ui->comboBoxIp->addItem(name);
        QString domain = QHostInfo::localDomainName();
        if (!domain.isEmpty())
             ui->comboBoxIp->addItem(name + QChar('.') + domain);
    }
    if (name != QLatin1String("localhost"))
        ui->comboBoxIp->addItem(QString("localhost"));
    // find out IP addresses of this machine
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // add non-localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (!ipAddressesList.at(i).isLoopback())
             ui->comboBoxIp->addItem(ipAddressesList.at(i).toString());
    }
    // add localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i).isLoopback())
            ui->comboBoxIp->addItem(ipAddressesList.at(i).toString());
    }

    ui->lineEditPort->setValidator(new QIntValidator(1, 65535, this));

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readStrGet()));
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
//! [3]
            this, &Dialog::displayError);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_connectButton_clicked()
{
    if(ui->lineEditPort->text().isEmpty() || ui->comboBoxIp->currentText().isEmpty())
    {
        return;
    }
    tcpSocket->abort();
    tcpSocket->connectToHost(ui->comboBoxIp->currentText(),
                             ui->lineEditPort->text().toInt());
}
void Dialog::readStrGet()
{
//    in.startTransaction();

//    QString nextFortune;
//    in >> nextFortune;
//    qDebug()<<in<<endl;
//    if (!in.commitTransaction())
//        return;

    QByteArray buffer=tcpSocket->readAll();
    if(!buffer.isEmpty())
    {
        qDebug()<<buffer<<endl;
        ui->textEdit_receive->append(buffer);
    }
}
void Dialog::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }
}