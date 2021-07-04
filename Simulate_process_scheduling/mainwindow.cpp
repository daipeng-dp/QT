#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QThread>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget_ready->setColumnCount(3);
    QStringList header;
    header<<"进程名称"<<"创建时间"<<"执行时间";
    ui->tableWidget_ready->setHorizontalHeaderLabels(header);
    ui->tableWidget->setColumnCount(6);
    QStringList header1;
    header1<<"进程名称"<<"进程状态"<<"开始时间"<<"完成时间"<<"周转时间"<<"带权周转时间";
    ui->tableWidget->setHorizontalHeaderLabels(header1);

    avg_turn_time=0.0;
    avg_wturn_time=0.0;
    QString s1="平均周转时间："+QString::number(avg_turn_time,10,2);
    QString s2="平均带权周转时间："+QString::number(avg_wturn_time,10,2);
    ui->label_4->setText(s1);
    ui->label_5->setText(s2);
    Group = new QButtonGroup(this);
    Group->addButton(ui->radioButton_1, 1);
    Group->addButton(ui->radioButton_2, 2);
    Group->addButton(ui->radioButton_3, 3);
    Group->addButton(ui->radioButton_4, 4);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateReadyTable()
{
    ui->tableWidget_ready->clear();
    int index=0;
    QStringList header;
    header<<"进程名称"<<"创建时间"<<"执行时间";
    ui->tableWidget_ready->clear();
    ui->tableWidget_ready->setHorizontalHeaderLabels(header);
    std::vector<process>::iterator it=ready.begin();
    while(it!=ready.end()){
        ui->tableWidget_ready->setRowCount(index+1);
        ui->tableWidget_ready->setColumnCount(3);
        ui->tableWidget_ready->setItem(index,0,new QTableWidgetItem((*it).p_name));
        ui->tableWidget_ready->setItem(index,1,new QTableWidgetItem(QString::number((*it).creation_time)));
        ui->tableWidget_ready->setItem(index,2,new QTableWidgetItem(QString::number((*it).running_time)));
        index++;
        it++;
    }
}

void MainWindow::updateTable(std::vector<process> sortVec)
{
    ui->tableWidget->clear();
    int index=0;
    QStringList header;
    header<<"进程名称"<<"进程状态"<<"开始时间"<<"完成时间"<<"周转时间"<<"带权周转时间";
    ui->tableWidget->clear();
    ui->tableWidget->setHorizontalHeaderLabels(header);
    std::vector<process>::iterator it=sortVec.begin();
    while(it!=sortVec.end()){
        ui->tableWidget->setRowCount(index+1);
        ui->tableWidget->setColumnCount(6);
        ui->tableWidget->setItem(index,0,new QTableWidgetItem((*it).p_name));
        //        ui->tableWidget->setItem(index,1,new QTableWidgetItem(QString::number((*it).creation_time)));
        //        ui->tableWidget->setItem(index,2,new QTableWidgetItem(QString::number((*it).running_time)));
        //        ui->tableWidget->setItem(index,2,new QTableWidgetItem(QString::number((*it).start_time)));
        //        ui->tableWidget->setItem(index,3,new QTableWidgetItem(QString::number((*it).finish_time)));
        //        ui->tableWidget->setItem(index,4,new QTableWidgetItem(QString::number((*it).turn_time)));
        //        ui->tableWidget->setItem(index,5,new QTableWidgetItem(QString::number((*it).Wturn_time)));
        if((*it).state==1){
            ui->tableWidget->setItem(index,1,new QTableWidgetItem("完成"));
            ui->tableWidget->setItem(index,2,new QTableWidgetItem(QString::number((*it).start_time)));
            ui->tableWidget->setItem(index,3,new QTableWidgetItem(QString::number((*it).finish_time)));
            ui->tableWidget->setItem(index,4,new QTableWidgetItem(QString::number((*it).turn_time)));
            ui->tableWidget->setItem(index,5,new QTableWidgetItem(QString::number((*it).Wturn_time)));
        }
        else{
            ui->tableWidget->setItem(index,1,new QTableWidgetItem("未完成"));
            ui->tableWidget->setItem(index,2,new QTableWidgetItem(QString::number((*it).start_time)));
            ui->tableWidget->setItem(index,3,new QTableWidgetItem(QString::number((*it).finish_time)));
            ui->tableWidget->setItem(index,4,new QTableWidgetItem(QString::number((*it).turn_time)));
            ui->tableWidget->setItem(index,5,new QTableWidgetItem(QString::number((*it).Wturn_time)));
        }
        index++;
        it++;
    }
}

std::vector<process> MainWindow::FCFS()
{
    avg_turn_time=0;//平均周转时间
    avg_wturn_time=0;//平均带权周转时间
    std::vector<process> temp(ready);
    temp=sortCtime(temp);//排序
    std::vector<process>::iterator it=temp.begin();
    double time=(*it).creation_time;
    while(it!=temp.end()){
        //确定进程的开始时间
        if((*it).creation_time<=time){
            (*it).start_time=time;
        }
        else{
            (*it).start_time=(*it).creation_time;
        }
        //更新进程状态
        (*it).state=(*it).start_time;
        // 确定进程的完成时间，周转时间，带权周转时间
        (*it).finish_time=(*it).start_time+(*it).running_time;
        (*it).turn_time=(*it).finish_time-(*it).creation_time;
        (*it).Wturn_time=(*it).turn_time/(*it).running_time;
        //更新进程状态
        (*it).state=1;
        time=(*it).finish_time;
        it++;
    }
    for (int j = 0; j < (int)temp.size(); j++)
    {
        //周转时间与带权周转时间之和
        avg_turn_time+=temp[j].turn_time;
        avg_wturn_time+=temp[j].Wturn_time;
    }
    //平均周转时间与平均带权周转时间
   avg_turn_time=avg_turn_time/(int)temp.size();
   avg_wturn_time=avg_wturn_time/(int)temp.size();
   return temp;
}

//（非抢占式）短进程优先算法
std::vector<process> MainWindow::SJF()
{
    avg_turn_time=0;//平均周转时间
    avg_wturn_time=0;//平均带权周转时间
    std::vector<process> temp(ready);
    //对就绪队列按创建时间升序排序，若到达时间相等，就按执行时间升序排序
     temp=sortCtime(temp);//排序
     //首先执行第一个创建的进程
    temp[0].start_time=temp[0].creation_time;
    temp[0].finish_time=temp[0].start_time+temp[0].running_time;
     temp[0].turn_time=temp[0].finish_time-temp[0].creation_time;
    temp[0].Wturn_time=temp[0].turn_time/temp[0].running_time;
    temp[0].state=1;
     double time=temp[0].finish_time;
     int sum=(int)temp.size();
     int n=1;
     while(n<sum){//还有进程未执行就继续
         for (int i = 1; i < (int)temp.size(); i++){
             //若当前时间小于下一个进程的创建时间，执行这个进程
             if(time<temp[i].creation_time&&temp[i].state==0){
                 temp[i].start_time=time;
                 temp[i].finish_time=temp[i].start_time+temp[i].running_time;
                 temp[i].turn_time=temp[i].finish_time-temp[i].creation_time;
                 temp[i].Wturn_time=temp[i].turn_time/temp[i].running_time;
                 temp[i].state=1;
                 n++;
                 time=temp[i].finish_time;
                 //qDebug()<<time;
             }
             else{//若当前时间大于下一个进程创建的时间，在到达的进程中找到执行时间最短的执行
                 int j=find(temp,time);//返回已创建进程中的最短进程的下标（未执行的）
                 //执行排序完成后的第一个进程
                 temp[j].start_time=time;
                 temp[j].finish_time=temp[j].start_time+temp[j].running_time;
                 temp[j].turn_time=temp[j].finish_time-temp[j].creation_time;
                 temp[j].Wturn_time=temp[j].turn_time/temp[j].running_time;
                 temp[j].state=1;
                 n++;
                 time=temp[j].finish_time;
                 //qDebug()<<time;
             }
         }
     }
     temp=sortStime(temp);//按开始时间排序，使其按执行顺序输出
    for (int j = 0; j < (int)temp.size(); j++)
    {
        //周转时间与带权周转时间之和
        avg_turn_time+=temp[j].turn_time;
        avg_wturn_time+=temp[j].Wturn_time;
    }
    //平均周转时间与平均带权周转时间
   avg_turn_time=avg_turn_time/(int)temp.size();
   avg_wturn_time=avg_wturn_time/(int)temp.size();
   return temp;
}

//（抢占式）短作业优先算法
std::vector<process> MainWindow::SPF()
{
    avg_turn_time=0;//平均周转时间
    avg_wturn_time=0;//平均带权周转时间
    double curtime=0;//记录当前时间
    std::vector<process> temp(ready);
    std::vector<process>t;
    int n=0,sum=(int)temp.size();//记录完成进程的数量
    while(n<sum){
        int index=0;  //数组下标记录
        int m=0;
        for(int i=0;i<(int)temp.size();i++)  //就绪队列加入进程
        {
            if(temp[i].creation_time<=curtime&&temp[i].state==0 )
            {
                t.push_back(temp[i]);
            }
        }
        if(!t.empty()){
            double mintime=INT_MAX;  //  最短剩余时间转换中间值
            for(int i=0;i<(int)t.size();i++){    //  从就绪队列找出下次运行进程的名称，并在原队列中进行调度
                //找出该已到达进程队列中剩余时间最短的进程
                if(t[i].rest_time<=mintime)
                {
                    mintime=t[i].rest_time;
                    m=i;
                }
            }
            /*对原进程队列进行进程调度模拟*/
            for(int i=0;i<(int)temp.size();i++){
                if(temp[i].p_name==t[m].p_name){
                    index=i;
                }
            }
            if(temp[index].rest_time==temp[index].running_time){
                temp[index].start_time=curtime;
            }
            temp[index].rest_time--;  //选中的进程剩余时间-1
            curtime++;  //系统时间加一
            if(temp[index].rest_time==0)
            {
                temp[index].finish_time=curtime;
                temp[index].turn_time=temp[index].finish_time-temp[index].creation_time;
                temp[index].Wturn_time=temp[index].turn_time/temp[index].running_time;
                temp[index].state=1;
                n++;
            }
        }
        else
        {
            curtime++;
        }
        t.clear();
    }

    for (int j = 0; j < (int)temp.size(); j++)
    {
        //周转时间与带权周转时间之和
        avg_turn_time+=temp[j].turn_time;
        avg_wturn_time+=temp[j].Wturn_time;
    }
    //平均周转时间与平均带权周转时间
    avg_turn_time=avg_turn_time/(int)temp.size();
    avg_wturn_time=avg_wturn_time/(int)temp.size();
    return temp;
}

std::vector<process> MainWindow::HRRN()
{
    avg_turn_time=0;//平均周转时间
    avg_wturn_time=0;//平均带权周转时间
    //进程队列按照创建时间排序，先让第一个创建的进程运行
    std::vector<process>temp(ready);
    temp=sortCtime(temp);
    int i=0;
    do{
        if(i==0){
            temp[i].start_time=temp[i].creation_time;//第一个进程的开始时间是它的创建时间
            temp[i].wait_time=0;
            temp[i].finish_time=temp[i].start_time+temp[i].running_time;//第一个进程的完成时间是它的开始时间+执行时间
            temp[i].turn_time=temp[i].finish_time-temp[i].creation_time;
            temp[i].Wturn_time=temp[i].turn_time/temp[i].running_time;
            temp[i].state=1;//已完成
        }
        else{//其他进程
            if(temp[i-1].finish_time>=temp[i].creation_time){//若该进程在上个进程完成之前被创建，那么它的开始时间为上个进程的完成时间
                temp[i].start_time=temp[i-1].finish_time;
            }
            else{//若该进程在上个进程完成之后被创建，那么它的开始时间为它的创建时间
                temp[i].start_time=temp[i].creation_time;

            }
            //确定进程的完成时间
            temp[i].wait_time=temp[i-1].finish_time-temp[i].creation_time;
            temp[i].finish_time=temp[i].start_time+temp[i].running_time;
            temp[i].turn_time=temp[i].finish_time-temp[i].creation_time;
            temp[i].Wturn_time=temp[i].turn_time/temp[i].running_time;
            temp[i].state=1;//已完成
        }


        i++;//到了第几个作业
        //计算响应比
        int n=i-1;
        //temp[i]完成后有多少进程到达
        int count=0;
        for (int j = i; j < (int)temp.size(); j++)
        {
         if(temp[j].creation_time<=temp[n].finish_time)   {
             count++;
         }
        }
        qDebug()<<count;//测试
        for (int j = i; j < (int)temp.size(); j++)
        {
            countRate(temp, j, n);    //计算响应比
            qDebug()<<temp[j].response_rate;//测试
        }
        temp=ratesort(temp, i);    //按响应比由大到小排序
        //qDebug()<<"顺序："<<temp[i].p_name;//测试(会越界)
    }while(i<(int)temp.size());

    for (int j = 0; j < (int)temp.size(); j++)
    {
        //周转时间与带权周转时间之和
        avg_turn_time+=temp[j].turn_time;
        avg_wturn_time+=temp[j].Wturn_time;
    }
    //平均周转时间与平均带权周转时间
    avg_turn_time=avg_turn_time/(int)temp.size();
    avg_wturn_time=avg_wturn_time/(int)temp.size();
    return temp;
}

void MainWindow::countRate(std::vector<process> &sortVec,int k, int m)
{
    double ratenum;
    ratenum = (sortVec[k].running_time + (sortVec[m].finish_time - sortVec[k].creation_time)) / (sortVec[k].running_time);
    sortVec[k].response_rate = ratenum;
}

std::vector<process> MainWindow::sortCtime(std::vector<process> sortVec)
{
    int swap;
    process temp;
    do{
        swap = 0;
        for (int count = 0; count < (int)sortVec.size() - 1; count ++){
            if (sortVec[count] .creation_time> sortVec[count + 1].creation_time){
                temp = sortVec[count];
                sortVec[count] = sortVec[count + 1];
                sortVec[count + 1] = temp;
                swap = 1;
            }
        }
    }while(swap!=0);
    return sortVec;
}

int MainWindow::find(std::vector<process> sortVec,double time)
{
    int n=0;
    double mintime=INT_MAX;
    for(int i=0;i<(int)sortVec.size();i++){
        if(sortVec[i].creation_time<time&&sortVec[i].state==0){
            if(sortVec[i].running_time<mintime){
                mintime=sortVec[i].running_time;
                 n=i;
            }
        }
    }

    return n;
}

std::vector<process> MainWindow::sortStime(std::vector<process> sortVec)
{
    int swap;
    process temp;
    do{
        swap = 0;
        for (int count = 0; count < (int)sortVec.size() - 1; count ++){
            if (sortVec[count] .start_time> sortVec[count + 1].start_time){
                temp = sortVec[count];
                sortVec[count] = sortVec[count + 1];
                sortVec[count + 1] = temp;
                swap = 1;
            }
        }
    }while(swap!=0);
    return sortVec;
}

 std::vector<process> MainWindow::ratesort(std::vector<process> sortVec,int k)
{
     process temp;
     for (int i = k; i <(int)sortVec.size(); i++)         //简单选择排序
     {
         for (int j = i + 1; j < (int)sortVec.size(); j++){
             if (sortVec[j].response_rate > sortVec[i].response_rate){
                 temp = sortVec[j];
                 sortVec[j] = sortVec[i];
                 sortVec[i] = temp;
             }
         }
     }
     return sortVec;
 }




//手动添加进程
void MainWindow::on_craete_pushButton_clicked()
{
    QString pname=ui->lineEdit_pname->text();
    QString ctime=ui->lineEdit_createtime->text();
    QString rtime=ui->lineEdit_runningtime->text();
    ui->lineEdit_pname->clear();
    ui->lineEdit_createtime->clear();
    ui->lineEdit_runningtime->clear();
    ready.push_back(process(pname,ctime.toDouble(),rtime.toDouble()));
    updateReadyTable();
}

void MainWindow::on_delete_pushButton_clicked()
{
    if(Group->checkedId()==1){
        std::vector<process> temp;
       temp=FCFS();//先来先服务算法
        QString s1="平均周转时间："+QString::number(avg_turn_time,10,2);
        QString s2="平均带权周转时间："+QString::number(avg_wturn_time,10,2);
        ui->label_4->setText(s1);
        ui->label_5->setText(s2);
        ui->tableWidget->clear();
        updateTable(temp);
        qDebug()<<1;
    }
    else if(Group->checkedId()==2){
        //短进程优先算法(非抢占式)
        std::vector<process> temp;
        temp=SJF();
        QString s1="平均周转时间："+QString::number(avg_turn_time,10,2);
        QString s2="平均带权周转时间："+QString::number(avg_wturn_time,10,2);
        ui->label_4->setText(s1);
        ui->label_5->setText(s2);
        ui->tableWidget->clear();
        updateTable(temp);
        qDebug()<<2;
    }
    else if(Group->checkedId()==3){
        //短进程优先算法（抢占式）
        std::vector<process> temp;
        temp=SPF();
        QString s1="平均周转时间："+QString::number(avg_turn_time,10,2);
        QString s2="平均带权周转时间："+QString::number(avg_wturn_time,10,2);
        ui->label_4->setText(s1);
        ui->label_5->setText(s2);
        ui->tableWidget->clear();
        updateTable(temp);
        qDebug()<<3;
    }
    else if(Group->checkedId()==4){
        //最高响应比优先调度算法
        std::vector<process> temp;
        temp=HRRN();
        QString s1="平均周转时间："+QString::number(avg_turn_time,10,2);
        QString s2="平均带权周转时间："+QString::number(avg_wturn_time,10,2);
        ui->label_4->setText(s1);
        ui->label_5->setText(s2);
        ui->tableWidget->clear();
        updateTable(temp);
        qDebug()<<4;
    }
}
