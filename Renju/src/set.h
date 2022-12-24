#ifndef _SET_H_
#define _SET_H_

/*
 *主管下棋、棋盘显示、报点等基础性界面
 */

#include "./common.h"
#include "./bit_board.h"
#include "./evaluate.h"

#define LASTBLACK 3         //刚下的黑棋
#define LASTWHITE 4         //刚下的白棋

/* 函数声明 */
void set_board();                               //打印当前局势的棋盘
void print_piece(int i, int j);                 //打印棋子
void player_set();                              //每下一步棋后根据玩家下棋对相应全局参数修改
void AI_set();                                  //AI下棋
void set();                                     //玩家下棋 
int lineLength(int i, int j, int dx, int dy);   //返回对应行连子个数
void get_input();                               //获取玩家输入
void new_set(int i, int j);                     //置点
void print_pos();                               //报点
void judge_result();                            //判断当前结果
int win(int i, int j);                          //判断是否赢
void player_op();                               //玩家操作
void AI_op();                                   //从这开始进入AI
int forbid(int i ,int j);                       //判断禁手

/* 全局变量 */
extern int now_score[];   //当前局面评分
extern Line g_move_buf[]; //缓存走法数组
#endif