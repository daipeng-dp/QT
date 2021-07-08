#include "client.h"
#include "ui_client.h"

client::client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::client)
{
    ui->setupUi(this);
    socket = new QTcpSocket();

    //连接信号槽
    QObject::connect(socket, &QTcpSocket::readyRead, this, &client::socketReadData);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &client::socketDisconnected);

    ui->pushButton_2->setEnabled(false);
    ui->lineEdit_ip->setText("127.0.0.1");
    ui->lineEdit->setText("8765");
    //定时器处理
    connect(&timer, &QTimer::timeout, [=](){
        timer.stop();
        //发送文件
        sendFile();
    });
    connect(socket, &QTcpSocket::readyRead, [=](){
                //取客户端的信息
                QByteArray buf = socket->readAll();
                if(QString(buf) == "file done")
                {
                    //文件发送完毕
                    ui->textEdit_2->append("文件发送完毕");

                    //关闭文件且断开连接
                    file.close();
                    socket->disconnectFromHost();
                    socket->close();
                }
            });
}

client::~client()
{
    delete this->socket;
    delete ui;
}

void client::sendFile()
{
    //ui->textBrowser->append("正在发送文件...");
    qint64 len = 0;
    do{
        //每次发送4KB
        char buf[4*1024] = {0};
        len = 0;

        len = file.read(buf, sizeof(buf));
        len = socket->write(buf, len);
    }while (len > 0);
}


void client::on_pushButton_clicked()
{
    if(ui->pushButton->text() == tr("连接"))
    {
        QString IP;
        int port;

        //获取IP地址
        IP = ui->lineEdit_ip->text();
        //获取端口号
        port = ui->lineEdit->text().toInt();

        //取消已有的连接
        socket->abort();
        //连接服务器
        socket->connectToHost(IP, port);

        //等待连接成功
        if(!socket->waitForConnected(30000))
        {
            qDebug() << "Connection failed!";
            return;
        }
        qDebug() << "Connect successfully!";

        //发送按键使能
        ui->pushButton_2->setEnabled(true);
        //修改按键文字
        ui->pushButton->setText("断开连接");
    }
    else
    {
        //断开连接
        socket->disconnectFromHost();
        //修改按键文字
        ui->pushButton->setText("连接");
        ui->pushButton->setEnabled(false);
    }
}

void client::on_pushButton_2_clicked()
{
    //初始化文件属性
    QString filePath = QFileDialog::getOpenFileName(this, "Open", "../");
    if(!filePath.isEmpty())
    {
        QFileInfo info(filePath);
        fileName = info.fileName();
        fileSize = info.size();

        file.setFileName(filePath);

        if(file.open(QIODevice::ReadOnly))
        {
            ui->textEdit->append(QString("FileName:%1 \n\nFileSize:%2 KB").arg(fileName).arg(fileSize/1024));
            //连接时设置“选择文件”为不可用，“发送文件”可用
        }
        else {
            qDebug()<<"打开文件失败";
        }
    }
    else
    {
        qDebug()<<"文件路径出错";
    }
    //1.发送文件头  格式：文件名###文件大小
    QString fileHead = QString("%1###%2").arg(fileName).arg(fileSize);
    qint16 len = socket->write(fileHead.toUtf8());
    if(len > 0)
    {
        //让文件头与文件内容之间发送间隔相隔10ms
        timer.start(10);
    }
    else {
        //发送头部文件失败
        qDebug()<<"发送头部文件失败";
        file.close();
    }
    socket->flush();
    file.close();
}

void client::socketReadData()
{
    QByteArray buffer;
    //读取缓冲区数据
    buffer = socket->readAll();
    if(!buffer.isEmpty())
    {
        QString str = ui->textEdit->toPlainText();
        str+=tr(buffer);
        //刷新显示
        ui->textEdit->setText(str);
    }
}

void client::socketDisconnected()
{
    //发送按键失能
    ui->pushButton_2->setEnabled(false);
    //修改按键文字
    ui->pushButton->setText("连接");
    qDebug() << "Disconnected!";
}
