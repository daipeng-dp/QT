#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include<QFileDialog>
#include<QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class client; }
QT_END_NAMESPACE

class client : public QMainWindow
{
    Q_OBJECT

public:
    client(QWidget *parent = nullptr);
    ~client();
    void sendFile();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void socketReadData();

    void socketDisconnected();
private:
    Ui::client *ui;
    QTcpSocket *socket;
    QFile file;     //需要发送的文件对象
    QString fileName;
    int fileSize;
    QTimer timer;   //定时器
};
#endif // CLIENT_H
