#ifndef SHUDU_H
#define SHUDU_H
#include<stdlib.h>

class shudu
{
public:
    shudu();//初始化数据
    int num[9][9];//数独数组
    int jie[9][9];//数独的解
    //创建数独
    void CreateShudu(int n);
    //清除数据
    void clear();
private:
    bool CountRow[9][9];//第i行的值为j的时候是否可行（9行）
    bool CountCol[9][9];//第i列的值为j的时候是否可行（9列）
    bool CountGe[9][9];//第i个九宫格的值为j的时候是否可行（9个九宫格）
    int Count;//非零元素个数
};

#endif // SHUDU_H
