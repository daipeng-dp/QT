#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->pTimer = new QTimer;
    //绑定定时器的信号
    connect(this->pTimer,SIGNAL(timeout()),this,SLOT(updateTime()));
    x=0,y=0;
    isSubmit=false;
    isStart=false;
    init();//初始化游戏盘
    for(int i=80;i>=17;i--){
        ui->comboBox->setToolTip("难度");
        ui->comboBox->addItem(QString::number(i));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTime()
{
    /*
         * 1.点击开始后获取到当前的时间并且赋值给baseTime
         * 2.启动定时器,间隔1s
         * 3.槽函数中再次获取当前的时间currTime
         * 4.计算两个时间的差值t
         * 5.声明一个showTime对象给他加上t的差值
         * 6.格式化后设置显示
         */
    QTime currTime = QTime::currentTime();
    int t = this->baseTime.msecsTo(currTime);
    QTime showTime(0,0,0,0);
    showTime = showTime.addMSecs(t);
    QString timeStr = showTime.toString("           时间hh:mm:ss");
    ui->label->setText(timeStr);
}

void MainWindow::on_pushButton_2_clicked()
{
    isSubmit=false;
    this->pTimer->stop();
    ui->label->setText("           时间00:00:00");
    SudokuClear();//清除关卡
    QMessageBox::about(this,"新游戏","请重新选择难度！");
}

void MainWindow::on_pushButton_clicked()
{
    isSubmit=false;
    isStart=true;
    //获取当前时间
    this->pTimer->stop();
    ui->pushButton_3->setText("暂停");
    this->baseTime = this->baseTime.currentTime();
    this->pTimer->start(1);

    SudokuClear();//清除关卡
    Q.clear();
    SetLevel();//设置难度
    //生成数独数组并复制给shudu
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            shudu[i][j]=Q.num[i][j];
            if(shudu[i][j]==0){
                isempty[i][j]=true;
            }
            //qDebug()<<shudu[i][j];
        }
    }
    SudokuShow(shudu);//载入关卡
}

//清除填入的某个框内的数字
void MainWindow::on_pushButton_4_clicked()
{
    if(!isSubmit&&isStart){//未提交且游戏开始才可以清除
        shudu[x][y]=0;
        sodku[x][y].setText("");
        solve[x][y]=Q.jie[x][y];
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    if(isStart){//游戏开始才可重新开始
        isSubmit=false;
        this->pTimer->stop();
        ui->pushButton_3->setText("暂停");
        ui->label->setText("           时间00:00:00");
        this->baseTime = this->baseTime.currentTime();
        this->pTimer->start(1);
        memcpy(solve,Q.jie,9*9*sizeof(int) );//还原解数组
        //还原数独数组
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                if(isempty[i][j]){
                    shudu[i][j]=0;
                    sodku[i][j].setText("");
                    sodku[i][j].setStyleSheet("QPushButton:checked{background-color: rgb(255, 255, 0)}");//按钮按下变为黄色
                    sodku[i][j].show();
                }
            }
        }
    }

}

void MainWindow::on_pushButton_3_clicked()
{
    if(!isSubmit&&isStart){//未提交且游戏开始才可以暂停/继续
        static QTime pauseTime;
        if(this->ui->pushButton_3->text() == "暂停")
        {
            pauseTime = QTime::currentTime();
            this->ui->pushButton_3->setText("继续");
            this->pTimer->stop();
            for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                    if(isempty[i][j]){
                        sodku[i][j].setCheckable(false);
                        sodku[i][j].setAutoExclusive(false);
                        sodku[i][j].close();
                        //sodku[i][j].installEventFilter(nullptr);
                        //qDebug()<<i<<"  "<<j;
                    }
                }
            }
        }
        else if(this->ui->pushButton_3->text() == "继续")
        {
            //进行差值计算
            QTime cut = QTime::currentTime();
            int t = pauseTime.msecsTo(cut);
            this->baseTime = this->baseTime.addMSecs(t);
            this->ui->pushButton_3->setText("暂停");
            this->pTimer->start();
            for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                    if(isempty[i][j]){
                        sodku[i][j].setCheckable(true);
                        sodku[i][j].setAutoExclusive(true);
                        sodku[i][j].show();
                        //sodku[i][j].installEventFilter(this);
                        qDebug()<<i<<"  "<<j;
                    }
                }
            }
        }
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    if(!isSubmit&&isStart){//未提交且游戏开始才可以提示
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                int temp=solve[i][j];
                if(temp){
                    shudu[i][j]=temp;
                    QString str=QString::number(temp);
                    sodku[i][j].setText(str);
                    solve[i][j]=0;
                    i=9;
                    j=9;
                    //qDebug()<<temp;
                }
            }
        }
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    if(isStart){//游戏开始后才可以提交
        isSubmit=true;
        this->pTimer->stop();
        int solve1[9][9],wring=0;
        memcpy(solve1,Q.jie,9*9*sizeof(int) );
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                if(isempty[i][j]){
                    if(shudu[i][j]==solve1[i][j]){
                        sodku[i][j].setStyleSheet("background-color: rgb(0, 255, 0);");//正确的用绿色标出
                    }
                    else{
                        QString str=QString::number(shudu[i][j])+"/"+QString::number(solve1[i][j]);
                        sodku[i][j].setStyleSheet("background-color: rgb(255, 0, 0);");//错误的用红色标出，并给出解
                        sodku[i][j].setText(str);
                        wring++;
                    }
                }
            }
        }
        double v=ui->comboBox->currentText().toDouble();
        v=(v-wring)*100/v;
        QTime cut = QTime::currentTime();
        int t = this->baseTime.msecsTo(cut);
        QTime sumtime(0,0,0,0);
        sumtime = sumtime.addMSecs(t);
        QMessageBox::about(this,"结束","你的正确率为："+QString::number(v)+"%\n花费时间为："+sumtime.toString("hh:mm:ss"));
//        if(wring){
//            QMessageBox::about(this,"菜狗","(*･´ω`･)っ[ 你好菜 ]");
//        }
//        else{
//            QMessageBox::about(this,"呵呵","到底是我的提示功能强大？还是遇到了高手？(ಡωಡ)hiahiahia ");
//        }
    }
}

void MainWindow::init()
{
    int c=0,l=0;
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            sodku[i][j].setParent(this);
            sodku[i][j].setGeometry(c,l,50,50);
            sodku[i][j].move(c,l);
            sodku[i][j].show();
            sodku[i][j].installEventFilter(this);//注册
            shudu[i][j]=0;
            isempty[i][j]=false;
            c+=50;
        }
        c=0;
        l+=50;
    }
}

void MainWindow::SudokuShow(int num[][9])
{
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(num[i][j]>=1&&num[i][j]<=9){//该位置有1~9的数字，显示在按钮上
                QString n=QString::number(num[i][j]);
                sodku[i][j].setText(n);
                sodku[i][j].setStyleSheet("background-color: rgb(0, 0, 255);");//按钮显示蓝色
                //sodku[i][j].show();
            }
            else if(num[i][j]==0){//该位置数字为0，即空白
                sodku[i][j].setStyleSheet("QPushButton:checked{background-color: rgb(255, 255, 0)}");//按钮按下变为黄色
                sodku[i][j].setCheckable(true);
                sodku[i][j].setAutoExclusive(true);//按钮点击高亮
                //sodku[i][j].installEventFilter(this);//注册
                //sodku[i][j].show();
            }
        }
    }
}

void MainWindow::SudokuClear()
{//将按钮的状态返回到初始状态
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            sodku[i][j].setText("");
            sodku[i][j].setStyleSheet("");
            sodku[i][j].setCheckable(false);
            sodku[i][j].setAutoExclusive(false);
            sodku[i][j].show();
            //sodku[i][j].installEventFilter(nullptr);
            solve[i][j]=0;
            shudu[i][j]=0;
            isempty[i][j]=false;
        }
    }
}

void MainWindow::SetLevel()
{
    int index=ui->comboBox->currentText().toInt();
    Q.CreateShudu(index);
    memcpy(solve,Q.jie,9*9*sizeof(int) );
}

bool MainWindow::eventFilter(QObject *target, QEvent *e)
{
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(target == &sodku[i][j]&&isempty[i][j]&&!isSubmit){//未提交才可以编辑，得到提示，得到信息；提交后功能冻结
                if(e->type() == QEvent::MouseButtonPress&&isempty[i][j]){ //单击消息
                    x=i,y=j;
                    qDebug()<<x<<"  "<<y;
                }
                else if(e->type()==QEvent::Enter){//鼠标进入消息
                    if(shudu[i][j]){
                        //sodku[i][j].setStyleSheet("QPushButton:hover{background-color: rgb(85, 0, 127)}");//鼠标悬停在按钮为紫色
                        qDebug()<<"使与其相同的数字高亮显示";
                    }

                }
                else if(e->type()==QEvent::Leave){//鼠标离开消息
                    if(shudu[i][j]){
                        //sodku[i][j].setStyleSheet("QPushButton:hover{background-color: rgb(85, 0, 127)}");//鼠标悬停在按钮为紫色
                        qDebug()<<"让高亮显示恢复";
                    }

                }
                else if(e->type()==QEvent::MouseButtonDblClick&&isempty[i][j]){//双击消息
                    x=i,y=j;
                    qDebug()<<x<<"  "<<y;
                    //QMessageBox::about(this,"填入数字","1 2 3 4 5 6 7 8 9");//后期改为双击提示
                    int temp=solve[i][j];
                    if(temp){
                        shudu[i][j]=temp;
                        QString str=QString::number(temp);
                        sodku[i][j].setText(str);
                        solve[i][j]=0;
                    }
                }
            }
        }
    }
    return QMainWindow::eventFilter(target, e);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(!isSubmit){//未提交，可以输入
        switch (event->key()) {
        //数字键0
        case Qt::Key_0:
            if(isempty[x][y]){
                shudu[x][y]=0;
                sodku[x][y].setText("");
            }
            break;
            //数字键1
        case Qt::Key_1:
            if(isempty[x][y]){
                shudu[x][y]=1;
                sodku[x][y].setText("1");
            }
            break;
            //数字键2
        case Qt::Key_2:
            if(isempty[x][y]){
                shudu[x][y]=2;
                sodku[x][y].setText("2");
            }
            break;
            //数字键3
        case Qt::Key_3:
            if(isempty[x][y]){
                shudu[x][y]=3;
                sodku[x][y].setText("3");
            }
            break;
            //数字键4
        case Qt::Key_4:
            if(isempty[x][y]){
                shudu[x][y]=4;
                sodku[x][y].setText("4");
            }
            break;
            //数字键5
        case Qt::Key_5:
            if(isempty[x][y]){
                shudu[x][y]=5;
                sodku[x][y].setText("5");
            }
            break;
            //数字键6
        case Qt::Key_6:
            if(isempty[x][y]){
                shudu[x][y]=6;
                sodku[x][y].setText("6");
            }
            break;
            //数字键7
        case Qt::Key_7:
            if(isempty[x][y]){
                shudu[x][y]=7;
                sodku[x][y].setText("7");
            }
            break;
            //数字键8
        case Qt::Key_8:
            if(isempty[x][y]){
                shudu[x][y]=8;
                sodku[x][y].setText("8");
            }
            break;
            //数字键9
        case Qt::Key_9:
            if(isempty[x][y]){
                shudu[x][y]=9;
                sodku[x][y].setText("9");
            }
            break;
        }
    }
}
