#ifndef _AI_H_
#define _AI_H

#include "../include/common.h"
#include "../include/set.h"
#include "../include/bit_board.h"
#include "../include/zobrist.h"
#include "../include/history.h"

#define MAXDEPTH 5   //搜索深度
#define P_INFINITY 200000000
#define N_INFINITY -200000000

//fail-soft + PVS + 空着裁剪
int alpha_beta(Tree pNode, int last_point, unsigned short* last_buf, Tree last_move, int depth, int alpha, int beta); 
void AI_operation();

int move_evaluate(int i, int j, int board_score, int depth);	//着法评估

#endif