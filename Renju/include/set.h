#ifndef _SET_H_
#define _SET_H_

#include "../include/common.h"
#include "../include/bit_board.h"
#define LASTBLACK 3 //刚下的黑棋
#define LASTWHITE 4 //刚下的白棋
//位棋盘

extern int g_score;

void set_board();   //打印当前局势的棋盘
void print_piece(int i, int j);
void player_set();
void AI_set();       //AI下棋
void set();          //下棋 
int lineLengTH(int i, int j, int dx, int dy);
void get_input();
void new_set(int i, int j);
void print_pos();
void judge_result();
int win(int i, int j);

void player_op();
void AI_op();//从这开始进入AI

int forbid(int i ,int j);//判断禁手

int move_evaluate(int i, int j, int g_score, int depth);

#endif