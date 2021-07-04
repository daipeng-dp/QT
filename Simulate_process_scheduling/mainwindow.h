#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<iostream>
#include<queue>
#include<vector>
#include<QTime>
#include<QButtonGroup>
#include"process.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::vector<process> ready;
    QButtonGroup *Group;
    //显示创建的进程的信息
    void updateReadyTable();
    //显示调度结束后进程的信息
    void updateTable(std::vector<process> sortVec);
    //先来先服务调度算法
   std::vector<process> FCFS();
    //短进程优先算法（非抢占式）
    std::vector<process> SJF();
    //短进程优先调度算法（抢占式）
    std::vector<process> SPF();
    //最高响应比优先调度算法
    std::vector<process> HRRN();
    //计算作业的响应比
    void countRate(std::vector<process> &sortVec, int k,int m);
    //按照创建时间对进程排序
    std::vector<process> sortCtime(std::vector<process> sortVec);
    //找出已创建进程中执行时间最短的进程（忽略已执行的进程）
    int find(std::vector<process> sortVec,double time);
    //排序：按照开始时间排序
    std::vector<process> sortStime(std::vector<process> sortVec);
    //按响应比大小对作业进行排序（降序排序）
     std::vector<process> ratesort(std::vector<process> sortVec,int k);
private slots:

    void on_craete_pushButton_clicked();

    void on_delete_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    double avg_turn_time;//平均周转时间
    double avg_wturn_time;//平均带权周转时间
};
#endif // MAINWINDOW_H
