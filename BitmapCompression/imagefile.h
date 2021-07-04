#ifndef IMAGEFILE_H
#define IMAGEFILE_H
#include<QString>
#include<Windows.h>
#include<QFile>
#include<QMessageBox>
#include<vcruntime.h>
#include<QDebug>
#include<QDateTime>

class ImageFile
{
public:
    ImageFile();

    //读取图片数据
    void ReadImage(QString path);
    //写图片数据
    void WriteImage(QString path);
    //压缩图片
    void CompressImage(QString path);
    //解压缩
    void UnCompressImage(QString path);


    //压缩位图
    void Compress(QString path);
    //解压缩
    void UnCompress(QString path);
    //读取位图数据
    void ReadBitmap(QString path);
    //写位图数据
    void WriteBitmap(QString path);
    //得到文件大小
    QString getsize();
    //获取创建时间
    QString gettime();

private:
    void compress(int n,unsigned char p[],unsigned int s[],unsigned int l[],unsigned int b[]);
    int length(unsigned char i);//获取i存储所需位数
    void Traceback(int n,int&i,unsigned int s[],unsigned int l[]);
    int Output(unsigned char p[],unsigned int s[],unsigned int l[],unsigned int b[],int n);
    void store(unsigned char *a, int *j, int *left, int bit, int temp);//将temp存到数组里
    void load(unsigned char *a, int *i, int *left, int bit, unsigned char *temp);//将temp存到数组里

    //转换编码方式
    bool UTF8ToUnicode(const char* UTF8, wchar_t* strUnicode);


private:
    struct BitImage{
        BITMAPFILEHEADER bheader;//文件头
        BITMAPINFOHEADER binfo;//信息头
        RGBQUAD rgb[256]; // 定义彩色表
        unsigned char * data;//位图文件数据
        unsigned char *cdata;//压缩文件数据数据
        QDateTime CreateTime;//创建时间
    };
     BitImage bitImage;//位图信息
    QMessageBox mesg;//消息提示
    QByteArray arrary;//存储图片信息
};

#endif // IMAGEFILE_H
