#ifndef PROCESS_H
#define PROCESS_H
#include<QString>
#include<vector>


//定义进程结构体
struct process
{
    process(){

    }
    process(QString name,double ctime,double rtime){
        this->p_name=name;
        this->creation_time=ctime;
        this->running_time=rtime;
        this->start_time=0;
        this->finish_time=0;
        this->wait_time=0;
        this->turn_time=0;
        this->Wturn_time=0;
        this->state=0;
        this->response_rate=0;
        this->rest_time=rtime;//刚创建时剩余时间为执行时间
    }
    QString p_name;//名称（无重复）（输入）
    double creation_time;//创建时间（输入）
    double running_time;//执行时间（输入）
    double start_time;//开始时间
    double finish_time;//完成时间
    double wait_time;//等待时间
    double turn_time;//周转时间=完成时间-创建时间
    double Wturn_time;//带权周转时间=周转时间/执行时间
    bool state;//进程状态
    double response_rate;//响应比
    double rest_time;//剩余时间
};
#endif // PROCESS_H
