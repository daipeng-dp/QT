#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include<QFileDialog>
#include<QMessageBox>
QT_BEGIN_NAMESPACE
namespace Ui { class server; }
QT_END_NAMESPACE

class server : public QMainWindow
{
    Q_OBJECT

public:
    server(QWidget *parent = nullptr);
    ~server();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void server_New_Connect();

    void socket_Read_Data();

    void socket_Disconnected();

private:
    Ui::server *ui;
    QTcpServer* Server;
    QTcpSocket* socket;
    QFile file;         //需要接收的文件对象
    QString fileName;
    int fileSize;
    int currentSize;    //当前已接收的文件大小
    bool isHead;        //标志位，是否是文件头
};
#endif // SERVER_H
