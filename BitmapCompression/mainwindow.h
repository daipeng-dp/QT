#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMessageBox>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QList>
#include <QMimeData>
#include<Windows.h>
#include<QFile>
#include<QDebug>
#include<vcruntime.h>
#include<QTimer>
#include"imagefile.h"

using namespace std;
#define _CRT_SECURE_NO_WARNINGS

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();//压缩

    void on_pushButton_clicked();//解压

private:
    //将图片拖拽到label
    bool eventFilter(QObject *watched, QEvent *event);
    //显示图片
    void ShowImage(QString path);
    //展示压缩/解压信息
    void ShowInfo(QString path);
    //判断是否为图片
    bool IsImageByGDI(const std::wstring &path);

private:
    Ui::MainWindow *ui;
    QMessageBox m;//消息提示
    QString filepath;//文件路径
    ImageFile bimage;
    QTime time;
    double runtime;//耗时
};
#endif // MAINWINDOW_H
