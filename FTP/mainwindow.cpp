#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_3_clicked()
{
    QString name=ui->lineEdit->text();
    QString pass=ui->lineEdit_2->text();
    QString host=ui->lineEdit_3->text();
    QString port=ui->lineEdit_4->text();
    client.FtpSetUserInfor(name,pass);
    client.FtpSetHostPort(host,port.toInt());
}

void MainWindow::on_pushButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                        tr("选择文件"),
                                                        "C:/Users");
    QFileInfo info(filePath);
    QString fileName=info.fileName();
    client.FtpPut(filePath,fileName);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString dirpath = QFileDialog::getExistingDirectory(this,
                                                        "选择文件夹","./",QFileDialog::ShowDirsOnly);
    QString fileName;
    client.FtpGet(fileName,dirpath);
}
