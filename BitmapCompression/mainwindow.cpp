#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_showimage->installEventFilter(this);
    ui->label_showimage->setAcceptDrops(true);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    ui->label_file_address->setText("文件路径：");
    ui->label_state->setText("耗时：");
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->label_showimage) {
        if (event->type() == QEvent::DragEnter) {
            //当拖放时鼠标进入label时, label接受拖放的动作
            QDragEnterEvent *dee = dynamic_cast<QDragEnterEvent *>(event);
            dee->acceptProposedAction();
            return true;
        }
        else if (event->type() == QEvent::Drop) {
            //当放操作发生后, 取得拖放的数据
            QDropEvent *de = dynamic_cast<QDropEvent *>(event);
            QList<QUrl> urls = de->mimeData()->urls();
            if (urls.isEmpty()) { return true; }
            filepath = urls.first().toLocalFile();
            ui->label_file_address->setText("文件路径："+filepath);
            ui->label_showimage->clear();//清理
            //显示图片
            const std::wstring path=filepath.toStdWString();
            if(IsImageByGDI(path)){
                //qDebug()<<"a";
                ShowImage(filepath);
            }
            return true;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::ShowImage(QString path)
{
    //在label上显示图片
    QImage image(path); // QImage对I/O优化过, QPixmap对显示优化
    if (!image.isNull()) {
        image = image.scaled(ui->label_showimage->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->label_showimage->setPixmap(QPixmap::fromImage(image));
    }
}

void MainWindow::ShowInfo(QString path)
{
    int i=path.lastIndexOf(".");
    int w=path.size()-i;
    QString postfix=path.right(w);
    bool falg=0;
    if(postfix==".bmp"){
        path.remove(i,4);
        path.append(".dp");
        falg=0;
    }
    else if(postfix==".dp"){
        path.remove(i,3);
        path.append("解压.bmp");
        falg=1;
    }
    if(!falg){
        ui->label_showinformation->clear();
        ui->label_state->clear();
        ui->label_showinformation->setText("位置："+path+"\n大小："+bimage.getsize()+"字节\n压缩时间："+bimage.gettime());
        QString rt=QString::number(runtime);//+"\nMD5："+postfix+"\nSHA1："+postfix+"\nCRC32："+postfix
        ui->label_state->setText("耗时"+rt+"s");
    }
    else{
        ui->label->clear();
        ui->label_state->clear();
        ui->label->setText("位置："+path+"\n大小："+bimage.getsize()+"字节\n解压时间："+bimage.gettime());
        QString rt=QString::number(runtime);
        ui->label_state->setText("耗时"+rt+"s");
    }
}

bool MainWindow::IsImageByGDI(const wstring &path)
{
    std::wstring file_exten;
    size_t pos = path.rfind(L'.');
    if (pos == std::wstring::npos)
        return false;
    file_exten = path.substr(pos, std::wstring::npos);
    //把file_exten转小写
    for (size_t u = 0; u < file_exten.length();u++)
    {
        if (file_exten[u] >= L'A' && file_exten[u]<='Z')
        {
            file_exten[u] += L'a' - L'A';
        }
    }
    if (file_exten == L".jpg" || file_exten == L".tif"|| file_exten == L".png" || file_exten == L".bmp"|| file_exten == L".gif" || file_exten == L".ico"){
        return true;
    }
    return false;
}

//解压
void MainWindow::on_pushButton_2_clicked()
{
    //判断读取的文件类型
    int i=filepath.lastIndexOf(".");
    int w=filepath.size()-i;
    QString postfix=filepath.right(w);
    if(postfix==".dp"){
    ui->progressBar->setValue(20);
    double time_Start = (double)clock();
    ui->progressBar->setValue(40);
    Sleep(1000);
    //解压
    bimage.UnCompress(filepath);
    ui->progressBar->setValue(50);
    double time_End = (double)clock();
    runtime=(time_End-time_Start)/1000.00;
    ui->progressBar->setValue(70);
    //显示信息
    ShowInfo(filepath);
     ui->progressBar->setValue(100);
    }
    else{
        m.warning(this,"警告","该文件暂不支持解压!");
    }
}

//压缩
void MainWindow::on_pushButton_clicked()
{
    //判断读取的文件类型
    int i=filepath.lastIndexOf(".");
    int w=filepath.size()-i;
    QString postfix=filepath.right(w);
    if(postfix==".bmp"){
    ui->progressBar->setValue(20);
    double time_Start = (double)clock();
    //压缩
    ui->progressBar->setValue(40);
    Sleep(1000);
    bimage.Compress(filepath);
    ui->progressBar->setValue(50);
    double time_End = (double)clock();
    runtime=(time_End-time_Start)/1000.00;
    ui->progressBar->setValue(70);
    //显示信息
    ShowInfo(filepath);
    ui->progressBar->setValue(100);
    }
    else{
        m.warning(this,"警告","该文件暂不支持压缩!");
    }
}
