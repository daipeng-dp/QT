#include "imagefile.h"

ImageFile::ImageFile()
{

}

void ImageFile::ReadImage(QString path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)){
        mesg.warning(NULL,"警告",file.errorString());
    }
    QImage image;
    arrary = file.readAll();
    qDebug() << image.fromData(arrary);
    file.close();
}

void ImageFile::WriteImage(QString path)
{
    int i=path.lastIndexOf(".");
    int w=path.size()-i;
    QString postfix=path.right(w);
    if(postfix==".dp"){
        path.remove(i,3);
        path.append(".");//传进存放的文件后缀
    }
    else{
        path.remove(i,w);
        path.append(".dp");
    }
    // 以只写方式打开，如果文件不存在，那么会创建该文件
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        mesg.warning(NULL,"警告",file.errorString());
    }
    file.write(arrary);
    file.close();
}

void ImageFile::Compress(QString path)
{
    ReadBitmap(path);
    //qDebug()<<"r";
    int n=bitImage.binfo.biWidth*bitImage.binfo.biHeight;
    unsigned int *s=new unsigned int[n+1];
    unsigned int *l=new unsigned int[n+1];
    unsigned int *b=new unsigned int[n+1];
    bitImage.cdata=new unsigned char[n];
    memset(bitImage.cdata, 0, n);
    compress(n,bitImage.data,s,l,b);
    int m=Output(bitImage.data,s,l,b,n);
    int left = 8,j = 0,t,k=1;
    int sum=1;
    for (int i = 1; i <= m; i++) {
        //qDebug()<<bitImage.cdata[i];
        //qDebug()<<sum;
        sum+=l[i];
        store(bitImage.cdata,&j,&left,8,l[i] - 1);
        store(bitImage.cdata,&j,&left,3,b[i] - 1);
        t = k + l[i];
        for (; k < t; k++) {
            store(bitImage.cdata,&j,&left,b[i],bitImage.data[k]);
        }
    }
    //qDebug()<<sum;
    bitImage.binfo.biSizeImage = j+1;//修改文件大小
    //qDebug()<<bitImage.binfo.biSizeImage;
    //qDebug()<<"s1";
    WriteBitmap(path);
    //qDebug()<<"s2";
    delete []s;
    delete []l;
    delete []b;
    delete []bitImage.cdata;
    delete []bitImage.data;
}

void ImageFile::UnCompress(QString path)
{
    ReadBitmap(path);
    //qDebug()<<"x";
    int n=bitImage.binfo.biWidth*bitImage.binfo.biHeight;
    //int  lineByte = (bitImage.binfo.biWidth * bitImage.binfo.biBitCount/ 8 + 3) / 4 * 4;//定义变量，计算图像每行像素所占的字节数（必须是4的倍数）
    bitImage.data=new unsigned char[n];
    memset(bitImage.data,0,n);
    int left = 8, k=0;
    unsigned char length,bit;
    int i = 0;//解压前数组index
    int j = 0;//解压后数组index
    //    qDebug()<<bitImage.binfo.biSizeImage;
    //    qDebug()<<lineByte*bitImage.binfo.biHeight;
    //    qDebug()<<n;
    while (i <(int)bitImage.binfo.biSizeImage) {
        length = 0;
        bit = 0;
        load(bitImage.cdata, &i, &left, 8, &length);
        load(bitImage.cdata, &i, &left, 3, &bit);
        length++;//因为压缩的时候有减1
        bit++;
        k = j;
        //qDebug()<<length<<"   "<<bit;
        //qDebug()<<k+length;
        for (; j <k+length; j++) {
            if(j<n){
                load(bitImage.cdata, &i, &left, bit, &bitImage.data[j]);
            }
            //qDebug()<<j;
        }
    }
    bitImage.binfo.biSizeImage = n; //恢复像素所占的空间
    //qDebug()<<"w";
    WriteBitmap(path);
    //qDebug()<<"wq";
    delete []bitImage.cdata;
    delete []bitImage.data;
}

void ImageFile::ReadBitmap(QString path)
{
    //判断读取的文件类型
    int i=path.lastIndexOf(".");
    int w=path.size()-i;
    QString postfix=path.right(w);

    //解决无法读取带有中文路径的文件的问题
    wchar_t strUnicode[260];
    UTF8ToUnicode(path.toStdString().c_str(), strUnicode);

    // 以二进制的方式打开文件
    FILE *file = _wfopen(strUnicode,L"rb");
    if (file == NULL) {
        mesg.warning(NULL,"警告","打开文件失败R!");
    }

    // 读取文件信息头
    fread(&bitImage.bheader, sizeof(BITMAPFILEHEADER), 1, file);
    // 读取位图信息头
    fread(&bitImage.binfo, sizeof(BITMAPINFOHEADER), 1, file);

    // 读取彩色表
    if(bitImage.binfo.biBitCount==1){
        //qDebug()<<1;
        fread(&bitImage.rgb, sizeof (RGBQUAD), 2, file);
    }
    else if(bitImage.binfo.biBitCount==4){
        //qDebug()<<4;
        fread(&bitImage.rgb, sizeof (RGBQUAD), 16, file);
    }
    else if(bitImage.binfo.biBitCount==8){
        //qDebug()<<8;
        fread(&bitImage.rgb, sizeof (RGBQUAD), 256, file);
    }
    else if(bitImage.binfo.biBitCount==24){
        //qDebug()<<24;
        fread(&bitImage.rgb, sizeof (RGBQUAD), 0, file);
    }

    int n=bitImage.binfo.biWidth*bitImage.binfo.biHeight;
    //int  lineByte = (bitImage.binfo.biWidth * bitImage.binfo.biBitCount/ 8 + 3) / 4 * 4;//定义变量，计算图像每行像素所占的字节数（必须是4的倍数）

    if(postfix==".bmp"){
        // 定义位图数据内存大小
        bitImage.data = new unsigned char[n+1]; //申请位图数据所需要的空间，读位图数据进内存
        //memset(bitImage.data,0,n+1);
        // 读取元素的位图数据
        fread(bitImage.data+1, sizeof(unsigned char),n, file);
    }
    else if(postfix==".dp"){
        //定义压缩文件数据内存大小
        bitImage.cdata=new unsigned char[bitImage.binfo.biSizeImage];
        memset(bitImage.cdata,0,bitImage.binfo.biSizeImage);
        //qDebug()<<"s1";
        //读取压缩文件的数据
        fread(bitImage.cdata,sizeof (unsigned char),bitImage.binfo.biSizeImage,file);
        //qDebug()<<"s2";
    }
    // 关闭位图
    fclose(file);
}

void ImageFile::WriteBitmap(QString path)
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

    //解决无法读取带有中文路径的文件的问题
    wchar_t strUnicode[260];
    UTF8ToUnicode(path.toStdString().c_str(), strUnicode);
    // 以二进制的方式打开图片
    FILE *file = _wfopen(strUnicode,L"wb");
    if (file == NULL) {
        mesg.warning(NULL,"警告","打开文件失败W!");
    }

    // 写入文件
    fwrite(&bitImage.bheader, sizeof(BITMAPFILEHEADER), 1, file);
    fwrite(&bitImage.binfo,  sizeof(BITMAPINFOHEADER), 1, file);
    // 写入彩色表
    if(bitImage.binfo.biBitCount==1){
        //qDebug()<<1;
        fwrite(&bitImage.rgb, sizeof (RGBQUAD), 2, file);
    }
    else if(bitImage.binfo.biBitCount==4){
        //qDebug()<<4;
        fwrite(&bitImage.rgb, sizeof (RGBQUAD), 16, file);
    }
    else if(bitImage.binfo.biBitCount==8){
        //qDebug()<<8;
        fwrite(&bitImage.rgb, sizeof (RGBQUAD), 256, file);
    }
    else if(bitImage.binfo.biBitCount==24){
        //qDebug()<<24;
        fwrite(&bitImage.rgb, sizeof (RGBQUAD), 0, file);
    }

    //fseek(file,bitImage.bheader.bfOffBits,0);

    int n=bitImage.binfo.biWidth*bitImage.binfo.biHeight;
    //qDebug()<<"k";
    if(falg){
        //qDebug()<<"w1";
        // 写入解压后的文件数据
        fwrite(bitImage.data, sizeof(unsigned char), n, file);
        //qDebug()<<"w2";
    }
    else if(!falg){
        //qDebug()<<"k1";
        //写入压缩后的文件数据
        fwrite(bitImage.cdata,sizeof(unsigned char),bitImage.binfo.biSizeImage,file);
        //qDebug()<<"k2";
    }
    // 关闭位图
    fclose(file);
    bitImage.CreateTime=QDateTime::currentDateTime();
}

QString ImageFile::getsize()
{
    QString size=QString::number(bitImage.binfo.biSizeImage);
    return size;
}

QString ImageFile::gettime()
{
    QString ctime=bitImage.CreateTime.toString("yyyy-MM-dd hh:mm:ss");
    return ctime;

}

void ImageFile::compress(int n, unsigned char p[], unsigned int s[], unsigned int l[], unsigned int b[])
{
    int Lmax = 256, header = 11;
    unsigned int bmax;
    s[0] = 0;
    for (int i = 1; i <= n; i++)
    {
        b[i] = length(p[i]);//计算像素点p需要的存储位数
        bmax = b[i];
        s[i] = s[i - 1] + bmax;
        l[i] = 1;
        for (int j = 2; j <= i && j <= Lmax; j++)
        {
            if (bmax<b[i - j + 1])
                bmax = b[i - j + 1];
            if (s[i]>s[i - j] + j * bmax)
            {
                s[i] = s[i - j] + j * bmax;
                l[i] = j;
            }
        }
        s[i] += header;
    }
}

int ImageFile::length(unsigned char i)
{
    int k=1;
    i=i/2;
    while(i>0){
        k++;
        i=i/2;
    }
    return k;
}

void ImageFile::Traceback(int n, int &i, unsigned int s[], unsigned int l[])
{
    if (n == 0){
        return;
    }
    Traceback(n - l[n], i, s, l);
    s[i++] = n - l[n]; //重新为s[]数组赋值，用来存储分段位置
}

int ImageFile::Output(unsigned char p[],unsigned int s[], unsigned int l[], unsigned int b[], int n)
{
    //qDebug()<<"The optimal value is "<<s[n];
    //int N=bitImage.binfo.biWidth*bitImage.binfo.biHeight;
    int max;
    int m = 0;
    Traceback(n, m, s, l);
    s[m] = n;
    //qDebug()<<"Decompose into "<<m<<" segments ";
    for (int j = 1; j <= m; j++)
    {
        l[j] = l[s[j]];
        max = -1;
        for (unsigned int i = s[j - 1] + 1; i <= s[j]; i++) {//b[j]应该取第段中占用位数最大的那个
            if (length(p[i]) > max)
                max = length(p[i]);
        }
        b[j] = max;
    }
    return m;
}

void ImageFile::store(unsigned char *a, int *j, int *left, int bit, int temp)
{
    if (*left <=bit) {
        a[(*j)++] += temp >> (bit - (*left));
        a[(*j)] +=(temp << (8 + (*left) - bit)) & 0xff;
        *left = 8 - (bit - (*left));
    }
    else {
        a[(*j)] += (temp << (*left - bit)) & 0xff;
        *left -= bit;
    }
}


void ImageFile::load(unsigned char *a, int *i, int *left, int bit, unsigned char *temp)
{
    if (*left <= bit) {
        *temp += ((a[(*i)++] << (8 - (*left))) & 0xff) >> (8 - bit);
        *temp += a[(*i)] >> (8 - (bit - (*left)));
        *left = 8 - (bit - (*left));
    }
    else {
        *temp += ((a[(*i)] >> ((*left) - bit) << (8 - bit)) & 0xff)>> (8 - bit);
        *left -= bit;
    }
}

bool ImageFile::UTF8ToUnicode(const char *UTF8, wchar_t *strUnicode)
{
    DWORD dwUnicodeLen;    //转换后Unicode的长度
    TCHAR *pwText;      //保存Unicode的指针
    // wchar_t* strUnicode;    //返回值
    //获得转换后的长度，并分配内存
    dwUnicodeLen = MultiByteToWideChar(CP_UTF8,0,UTF8,-1,NULL,0);
    pwText = new TCHAR[dwUnicodeLen];
    if(!pwText)
    {
        return false;
    }
    //转为Unicode
    MultiByteToWideChar(CP_UTF8,0,UTF8,-1,pwText,dwUnicodeLen);
    //转为CString
    wcscpy(strUnicode, pwText);
    //清除内存
    delete[] pwText;
    return true;
}
