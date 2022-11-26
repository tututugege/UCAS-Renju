#ifndef _MAIN_H_
#define _MAIN_H_

#include "../include/common.h"
extern int player_i;
extern int player_j;
//为了追求速度 牺牲空间预先缓存大量内容
void init_rand();                     //初始化随机数
void init_point_table();              //初始化评分表
void init_move_table();               //初始化走法表
void init_bit_board();                //初始化位棋盘，0表示已下棋，1表示未下棋
void set_bit_board(int i, int j);     //位棋盘下棋

void set_board();                     //打印棋盘
void set();                           //下棋 
void AI_set();                        //AI下棋

#endif