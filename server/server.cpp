#include "server.h"
#include "ui_server.h"

server::server(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::server)
{
    ui->setupUi(this);
    ui->lineEdit->setText("8765");
    ui->pushButton_2->setEnabled(false);

    Server = new QTcpServer();

    //连接信号槽
    connect(Server,&QTcpServer::newConnection,this,&server::server_New_Connect);
    //初始化
    isHead = true;
    fileSize = 0;
    currentSize = 0;
}

server::~server()
{
    Server->close();
    Server->deleteLater();
    delete ui;
}


void server::on_pushButton_clicked()
{
    if(ui->pushButton->text() == tr("侦听"))
    {
        //从输入框获取端口号
        int port = ui->lineEdit->text().toInt();

        //监听指定的端口
        if(!Server->listen(QHostAddress::Any, port))
        {
            //若出错，则输出错误信息
            qDebug()<<Server->errorString();
            return;
        }
        //修改按键文字
        ui->pushButton->setText("取消侦听");
        qDebug()<< "Listen succeessfully!";
    }
    else
    {
        /*
            //如果正在连接（点击侦听后立即取消侦听，若socket没有指定对象会有异常，应修正——2017.9.30）
            if(socket->state() == QAbstractSocket::ConnectedState)
            {
                //关闭连接
                socket->disconnectFromHost();
            }
    */
        socket->abort();
        //取消侦听
        Server->close();
        //修改按键文字
        ui->pushButton->setText("侦听");
        //发送按键失能
        ui->pushButton_2->setEnabled(false);
    }
}

void server::on_pushButton_2_clicked()
{
    qDebug() << "Send: " << ui->textEdit_2->toPlainText();
    //获取文本框内容并以ASCII码形式发送
    socket->write(ui->textEdit_2->toPlainText().toLatin1());
    socket->flush();
}

void server::server_New_Connect()
{
    //获取客户端连接
    socket = Server->nextPendingConnection();
    //连接QTcpSocket的信号槽，以读取新数据
    QObject::connect(socket, &QTcpSocket::readyRead, this, &server::socket_Read_Data);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &server::socket_Disconnected);
    //发送按键使能
    ui->pushButton_2->setEnabled(true);

    qDebug() << "A Client connect!";
}

void server::socket_Read_Data()
{
    //取出接收的内容
    QByteArray buffer;
    //读取缓冲区数据
    buffer = socket->readAll();
    if(!buffer.isEmpty())
    {
        if(true == isHead)          //文件头处理
        {
            isHead = false;
            fileName = QString(buffer).section("###", 0, 0);
            fileSize = QString(buffer).section("###", 1, 1).toInt();
            //重置已接收大小
            currentSize = 0;
            file.setFileName(fileName);
            //以只写方式打开文件
            if(!file.open(QIODevice::WriteOnly))
            {
                qDebug()<<"以只写方式打开文件失败";
                socket->disconnectFromHost();
                socket->close();
                return ;
            }
            QString str = QString("接收的文件: [%1: %2 KB]").arg(fileName).arg(fileSize/1024);
            ui->textEdit->append(str);

        }
        else                        //文件内容处理
        {
            qint64 len = file.write(buffer);
            //每次写入的大小累加
            if(len > 0)
            {
                currentSize += len;
            }

            if(currentSize == fileSize)   //文件传输完成的条件
            {
                ui->textEdit_2->append("文件接收完成");
                socket->write("file done");
                QMessageBox::information(this, "完成", "文件传输完成");

                //传输完成,关闭文件且关闭连接
                file.close();
                socket->disconnectFromHost();
                socket->close();
            }
        }
//        QString str = ui->textEdit_2->toPlainText();
//        str+=tr(buffer);
//        //刷新显示
//        ui->textEdit_2->setText(str);
    }
}

void server::socket_Disconnected()
{
    //发送按键失能
    ui->pushButton_2->setEnabled(false);
    qDebug() << "Disconnected!";
}
