#ifndef _SET_H_
#define _SET_H_

#include "../include/common.h"
#define LASTBLACK 3 //刚下的黑棋
#define LASTWHITE 4 //刚下的白棋
//位棋盘
extern int player_i;
extern int player_j;

void cache_move_board();
void set_bit_board(int i, int j);
void get_range(int*, int*, int);

void get_input();
void print_piece(int i, int j);
void set_board();   //打印当前局势的棋盘
void set();          //下棋 
void AI_set();       //AI下棋
void new_set(int i, int j);

int forbid(int i ,int j);//判断禁手

void AI_operation();//从这开始进入AI

#endif