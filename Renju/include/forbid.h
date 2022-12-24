#ifndef _FORBID_H_
#define _FORBID_H_
/*
 *禁手判断，核心思想是三进制哈希匹配，将一种棋形映射为一个三进制数作为索引
 */
#include "../include/common.h"

/* 棋形模板 */
#define huo_three1  117  //011100
#define huo_three2  39   //001110
#define huo_three3  111  //011010
#define huo_three4  93   //010110
#define chong_four_1 112 //11011
#define chong_four_2 94  //10111
#define chong_four_3 118 //11101

/* 函数声明 */
int lineLength(int i, int j, int dx, int dy);  //返回最长连子
int isHuoFour(int i, int j, int dx, int dy);   //判断活四和半活四
int isChongFour(int i, int j, int dx, int dy); //跳四的冲四
int isHuoThree(int i, int j, int dx, int dy);  //判断活三

#endif