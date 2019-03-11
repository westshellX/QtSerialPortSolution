#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QtWidgets/QComboBox>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>



int Bytes2QString(const  char* pSrc,int nSrcLength,QString& qDstString  )
{
    const char tab[]="0123456789ABCDEF";	// 0x0-0xf的字符查找表
    int i;
    for ( i = 0; i < nSrcLength; i++)
    {
        QChar buf[4];
        buf[0] = tab[((unsigned char)pSrc[i]) >> 4];// 输出高4位
        buf[1] = tab[((unsigned char)pSrc[i]) & 0x0f];// 输出低4位
        buf[2] = ' ';
        buf[3] = '\0';

        qDstString.append(buf[0]);
        qDstString.append(buf[1]);
        qDstString.append(buf[2]);

    }
    // 返回目标字符串长度
    return (nSrcLength * 3);
}


unsigned char  hex2int(unsigned char ch )
{
    if( ch >= '0' && ch <= '9' )
    {
        return ch - '0';
    }
    else if( (ch >= 'A' && ch <= 'F')  )
    {
        return ch - 'A' + 10;
    }
    else if( (ch >= 'a' && ch <= 'f'))
    {
        return ch - 'a' + 10;
    }
    else
    {
        return 0;
    }
}


int ishexchar(unsigned char ch )
{
    if( ch >= '0' && ch <= '9' )
    {
        return 1;
    }
    else if( (ch >= 'A' && ch <= 'F')  )
    {
        return 1;
    }
    else if( (ch >= 'a' && ch <= 'f'))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int ConvertToHex(unsigned char* pszSrc,int nSrcLen,unsigned char* pszDest,int nDestLen )
{
    int i=0,next=0;
    int destIndex=0;
    unsigned char ch=0;
    for( i = 0; i < nSrcLen; i++ )
    {
        if( (pszSrc[i]==' '|| pszSrc[i]=='\r' || pszSrc[i]=='\n')   )
        {
            if( next == 0 )
            {
                continue;
            }
            else if( next == 1)
            {
                return -1;
            }
        }
        if( !ishexchar(pszSrc[i]))
        {
            return  -1;
        }
        if( next == 0 )
        {
            ch = hex2int(pszSrc[i]);
            ch=ch*16;
            next = 1;
        }
        else if(next == 1 )
        {
            ch = ch + hex2int(pszSrc[i]);
            next = 0;

            if( destIndex < nDestLen )
            {
                pszDest[destIndex++] = ch;
            }
        }
    }
    if( next > 0 )
    {
        return -1;
    }
    return destIndex;
}





//排列判断
bool compareBaudRateData(const qint32 &baudRate1, const qint32 &baudRate2)
{
    // 从大到小排序
    if (baudRate1 > baudRate2)
    {
        return true;
    }
    return false;
}


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{


    ui->setupUi(this);

    mSerialOpened = false;

    QComboBox* serialComboBox = ui->comboSerial;
    serialComboBox->clear();
    // 查找可用串口
    foreach( const QSerialPortInfo& info, QSerialPortInfo::availablePorts() )
    {
        QSerialPort serialPort;
        serialPort.setPort(info);
        if( serialPort.open(QIODevice::ReadWrite) )
        {
            serialComboBox->addItem(info.portName());
            serialPort.close();
        }
    }

    // 标准波特率
    QComboBox* comboBaudRate = ui->comboBaudRate;

    QList<qint32> baudRates = QSerialPortInfo::standardBaudRates();

    qSort(baudRates.begin(),baudRates.end(),compareBaudRateData );
    qint32 selectedIndex = -1;
    qint32 nIndex = 0;
    foreach( qint32 baudRate ,baudRates )
    {
        comboBaudRate->addItem(QString::number(baudRate,10));
        if ( baudRate == QSerialPort::Baud9600 )
        {
            selectedIndex=nIndex;
        }
        nIndex++;
    }
    if ( selectedIndex != -1 )
    {
        comboBaudRate->setCurrentIndex(selectedIndex);
    }

    enableUI(true);

    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);

}




Dialog::~Dialog()
{
    delete ui;
}


void Dialog::serial_read()
{
    QByteArray buf;
    QString strHexString;
    int datalen = 0;
    if ( !mSerialOpened )
    {
        return;
    }

    datalen = mSerialPort.bytesAvailable();
    if ( datalen <= 0 )
    {
        return;
    }
    buf = mSerialPort.readAll();


    QString strText;
    if ( ui->checkBoxInputData->isChecked() )
    {
        // 字符串转化成十六进制字符串
        Bytes2QString(buf.data(),buf.length(),  strHexString);

        strText = ui->textLogEdit->toPlainText();
        strText += strHexString;
    }
    else
    {
       buf.append('\0');
       strText = ui->textLogEdit->toPlainText();
       strText += buf;
    }


    ui->textLogEdit->setText(strText);

}

void Dialog::enableUI(bool enable )
{
    ui->comboSerial->setEnabled(enable);
    ui->comboBaudRate->setEnabled(enable);
//    ui->checkBoxInputData->setEnabled(enable);
//    ui->checkBoxOutputData->setEnabled(enable);
    ui->pushButtonTxData->setEnabled(!enable);
}

void Dialog::on_btnOpenSerial_clicked()
{

    QString strPortName = ui->comboSerial->currentText();
    if ( strPortName.isEmpty() )
    {
        return;
    }

    if ( ui->btnOpenSerial->text() == tr("打开串口") )
    {
        QString qString = ui->comboBaudRate->currentText();
        if ( qString.isEmpty() )
        {
            return;
        }
        // 打开串口
        mSerialPort.setPortName(strPortName);
        mSerialPort.setBaudRate(qString.toInt());
        mSerialPort.setDataBits(QSerialPort::Data8);
        mSerialPort.setParity(QSerialPort::NoParity);
        mSerialPort.setStopBits(QSerialPort::OneStop);
        mSerialPort.setFlowControl(QSerialPort::NoFlowControl);

        //连接信号槽
        connect(&mSerialPort, SIGNAL(readyRead()), this, SLOT(serial_read()));
        mSerialOpened = mSerialPort.open(QIODevice::ReadWrite);
        if ( mSerialOpened )
        {
            mSerialOpened = true;


            enableUI(false);


            ui->btnOpenSerial->setText("关闭串口");
        }
    }
    else
    {
        // 关闭串口
        mSerialOpened = false;
        mSerialPort.clear();
        mSerialPort.close();

        enableUI(true);

        ui->btnOpenSerial->setText("打开串口");


        // 关闭串口后，可能电脑硬件已经改变，重新查找一次串口
        QComboBox* serialComboBox = ui->comboSerial;
        serialComboBox->clear();
        // 查找可用串口
        foreach( const QSerialPortInfo& info, QSerialPortInfo::availablePorts() )
        {
            QSerialPort serialPort;
            serialPort.setPort(info);
            if( serialPort.open(QIODevice::ReadWrite) )
            {
                serialComboBox->addItem(info.portName());
                serialPort.close();
            }
        }
    }



}


// 串口发送数据
void Dialog::on_pushButtonTxData_clicked()
{
    QString qstr = ui->textTxEdit->toPlainText();

    if ( ui->checkBoxOutputData->isChecked() )
    {
        // 十六进制发送
        qstr.trimmed();
        QByteArray qb =  qstr.toLatin1();
        if ( qb.length() > 0 )
        {
            char* p = new char[qb.length() + 1];
            memset(p,0,qb.length() + 1  );
            qint32 nHexLength = ConvertToHex((unsigned char*)qb.data(),qb.length(),(unsigned char*)p,qb.length() );
            if ( nHexLength > 0 )
            {
                mSerialPort.write(p,nHexLength);
            }
            else
            {
                QMessageBox msgBox(QMessageBox::Warning, tr("提示"), "十六进制数据格式错误!", 0, this);
                msgBox.exec();
            }
        }
    }
    else
    {
        // 字符串发送
        QByteArray qb =  qstr.toLatin1();
        mSerialPort.write(qb.data(),qb.length());
    }
}

void Dialog::on_pushButtonCleanRx_clicked()
{
    ui->textLogEdit->setText("");
}

void Dialog::on_pushButtonCleanTx_clicked()
{
    ui->textTxEdit->setText("");
}
