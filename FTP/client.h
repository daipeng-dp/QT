#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>

#include <QFile>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QByteArray>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>

class FtpCLient:public QObject
{
    Q_OBJECT
protected slots:
    void finished(QNetworkReply * reply);
public:
    FtpCLient();
    void FtpGet(QString sor, QString dev);
    void FtpPut(QString source, QString dev);
    void FtpSetUserInfor(QString user, QString pwd);
    void FtpSetHostPort(QString str, int port =21);
private:
    QFile * m_pFile;
    QNetworkReply *m_pReply;
    QNetworkAccessManager * m_pManager;
    QUrl * m_pUrl;


};
#endif // CLIENT_H
