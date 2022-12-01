#ifndef _GAME_MODE_H_
#define _GAME_MODE_H_

#include "../include/common.h"
#include "../include/bit_board.h"
#include "../include/zobrist.h"
#include "../include/set.h"

//为了追求速度 牺牲空间预先缓存大量内容
void PvsP(); //人机对战
void PvsAI(); //人人对战
void AIvsAI(); //机机对战
void print_pos(); //报点

void init_point_table(); //初始化评分表

#endif