#ifndef _GAME_MODE_H_
#define _GAME_MODE_H_

/*
 *三种游戏模式框架
 */

#include "../include/common.h"
#include "../include/bit_board.h"
#include "../include/set.h"

/* 函数声明 */
void PvsP();        //人机对战
void PvsAI();       //人人对战
void AIvsAI();      //机机对战
void print_pos();   //报点
void init_hash();   //初始化评分表
#endif