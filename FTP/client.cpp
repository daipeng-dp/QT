#include "client.h"

FtpCLient::FtpCLient()
{
    m_pManager = new QNetworkAccessManager();
    m_pUrl = new QUrl();
    m_pUrl->setScheme("ftp");
    connect(m_pManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(finished(QNetworkReply *)));
}




void FtpCLient::finished(QNetworkReply * reply)
{
    m_pFile->write(reply->readAll());
    m_pFile->flush();
    m_pFile->close();
    reply->deleteLater();
}


//设置FTP服务器用户名和密码
void FtpCLient::FtpSetUserInfor(QString user, QString pwd)
{
    m_pUrl->setUserName(user);
    m_pUrl->setPassword(pwd);
}
//设置地址和端口
void FtpCLient::FtpSetHostPort(QString str, int port )
{
    m_pUrl->setHost(str);
    m_pUrl->setPort(port);
}
//下载文件
void FtpCLient::FtpGet(QString sor, QString dev)
{
    QFileInfo info;
    info.setFile(dev);
    m_pFile = new QFile(info.filePath());
    m_pFile->open(QIODevice::Append|QIODevice::WriteOnly);
    m_pUrl->setPath(sor);

    m_pReply = m_pManager->get(QNetworkRequest(*m_pUrl));
}
//上传文件
void FtpCLient::FtpPut(QString source, QString dev)
{
    QFile file(source);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();

    m_pUrl->setPath(dev);
    m_pManager->put(QNetworkRequest(*m_pUrl), data);
}
