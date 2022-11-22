#ifndef _FORBID_H_
#define _FORBID_H_

#define huo_three1  117 //三进制的011100
#define huo_three2  39 //001110
#define huo_three3  111 //011010
#define huo_three4  93 //010110
#define chong_four_1  112 //11011
#define chong_four_2  94 //10111
#define chong_four_3  118 //11101

int lineLength(int i, int j, int dx, int dy); //返回最长连子
int isHuoFour(int i, int j, int dx, int dy); //判断活四和半活四
int isChongFour(int i, int j, int dx, int dy); //跳四的冲四
int isHuoThree(int i, int j, int dx, int dy);  //判断活三

#endif