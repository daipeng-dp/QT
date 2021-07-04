#ifndef STRUCT_H
#define STRUCT_H

#include<QString>

//磁盘块
struct diskblock
{
    diskblock()
    {
    dflag=false;
    }
    int diskaddress;//外存地址
    QString dpage;//装入的页面即装入的内容
    bool dflag;//外存是否页面 装入标志位=1为占用 =0为未占用
};

//页表项
struct PageTableItem//进程页表页表项(请求式的页表项)
{
    int pagenumber;//页号
    int framenumber;//页框号
    int flag;//驻留标志位(中断位)
    int diskaddress;//外存地址
    int changeflag;//修改位=1为修改了
};
std::vector<diskblock> disk[50];//假设外存大小25M=25600KB
void init_disk();
#endif // STRUCT_H
