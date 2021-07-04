#include "shudu.h"

shudu::shudu()
{
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            num[i][j]=0;
            CountCol[i][j]=0;
            CountRow[i][j]=0;
            CountGe[i][j]=0;
            jie[i][j]=0;
            Count=0;
        }
    }
}

void shudu::CreateShudu(int n)
{
    //clear();
    int x, y;
    /*中间格第4格随机值 */
    for (int i = 3; i <= 5; i++)
    {
        for (int j = 3; j <= 5; j++)
        {
            do
            {
                x = rand() % 9 + 1;
            } while (CountGe[4][x]);
            num[i][j] = x;
            CountRow[i][x] = CountCol[j][x] = CountGe[4][x] = 1;
        }
    }
    /*填充第3格 */
    for (int i = 3; i <= 5; i++)
    {
        for (int j = 0; j <= 2; j++)
        {
            num[i][j] = num[(i - 1 + 3) % 3 + 3][j + 3];

            x = num[i][j];
            CountRow[i][x] = CountCol[j][x] = CountGe[3][x] = 1;
        }
    }
    /*填充第5格 */
    for (int i = 3; i <= 5; i++)
    {
        for (int j = 6; j <= 8; j++)
        {
            num[i][j] = num[(i + 1 + 3) % 3 + 3][j - 3];

            x = num[i][j];
            CountRow[i][x] = CountCol[j][x] = CountGe[5][x] = 1;
        }
    }
    /*填充第1格 */
    for (int i = 0; i <= 2; i++)
    {
        for (int j = 3; j <= 5; j++)
        {
            num[i][j] = num[i + 3][(j - 1 + 3) % 3 + 3];

            x = num[i][j];
            CountRow[i][x] = CountCol[j][x] = CountGe[1][x] = 1;
        }
    }
    /*填充第7格 */
    for (int i = 6; i <= 8; i++)
    {
        for (int j = 3; j <= 5; j++)
        {
            num[i][j] = num[i - 3][(j + 1 + 3) % 3 + 3];

            x = num[i][j];
            CountRow[i][x] = CountCol[j][x] = CountGe[7][x] = 1;
        }
    }
    /*填充第0格 */
    for (int i = 0; i <= 2; i++)
    {
        for (int j = 0; j <= 2; j++)
        {
            num[i][j] = num[i + 3][(j - 1 + 3) % 3];

            x = num[i][j];
            CountRow[i][x] = CountCol[j][x] = CountGe[0][x] = 1;
        }
    }
    /*填充第6格 */
    for (int i = 6; i <= 8; i++)
    {
        for (int j = 0; j <= 2; j++)
        {
            num[i][j] = num[i - 3][(j + 1 + 3) % 3];

            x = num[i][j];
            CountRow[i][x] = CountCol[j][x] = CountGe[6][x] = 1;
        }
    }

    /*填充第2格 */
    for (int i = 0; i <= 2; i++)
    {
        for (int j = 6; j <= 8; j++)
        {
            num[i][j] = num[i + 3][(j - 1 + 3) % 3 + 6];

            x = num[i][j];
            CountRow[i][x] = CountCol[j][x] = CountGe[2][x] = 1;
        }
    }
    /*填充第8格 */
    for (int i = 6; i <= 8; i++)
    {
        for (int j = 6; j <= 8; j++)
        {
            num[i][j] = num[i - 3][(j + 1 + 3) % 3 + 6];

            x = num[i][j];
            CountRow[i][x] = CountCol[j][x] = CountGe[8][x] = 1;
        }
    }
    Count = 81;
    while (Count > n)//保留n个数字
    {
        do
        {
            x = rand() % 9;
            y = rand() % 9;
        } while (num[x][y] == 0);
        int t = num[x][y];
        jie[x][y]=t;//得到数独的解
        num[x][y] = 0;
        Count--;
        CountRow[x][t] = CountCol[y][t] = CountGe[x / 3 * 3 + y / 3][t] = 0;
    }
}

void shudu::clear()
{
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            num[i][j]=0;
            jie[i][j]=0;
            CountCol[i][j]=0;
            CountRow[i][j]=0;
            CountGe[i][j]=0;
            Count=0;
        }
    }
}
