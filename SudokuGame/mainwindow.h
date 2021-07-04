#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPushButton>
#include<QMessageBox>
#include<QKeyEvent>
#include<QTime>
#include<QTimer>
#include"shudu.h"
using namespace std;

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
    //更新时间
    void updateTime();
    //新游戏
    void on_pushButton_2_clicked();
    //开始游戏
    void on_pushButton_clicked();
    //清除填入的某个框内的数字
    void on_pushButton_4_clicked();
    //重玩当前关卡
    void on_pushButton_7_clicked();
    //暂停
    void on_pushButton_3_clicked();
    //提示
    void on_pushButton_5_clicked();
    //提交
    void on_pushButton_6_clicked();

private:
    //初始化游戏盘和数独数组
    void init();
    //载入关卡
    void SudokuShow(int num[][9]);
    //清理关卡
    void SudokuClear();
    //设置难度
    void SetLevel();

protected:
    bool eventFilter(QObject *target, QEvent *e);//响应鼠标点击
    void keyPressEvent( QKeyEvent *event );//响应键盘输入

private:
    Ui::MainWindow *ui;
    QTimer *pTimer;
    QTime baseTime;
    QPushButton sodku[9][9];
    int x,y;//获得鼠标点击的按钮下标
    shudu Q;//创建数独和解数独的类对象
    int shudu[9][9];//数独的二维数组
    bool isempty[9][9];//存储数独的空位置填入的值，未填的设置为10
    int solve[9][9];//存放数独的解
    bool isSubmit;//是否提交
    bool isStart;//是否开始
};
#endif // MAINWINDOW_H
