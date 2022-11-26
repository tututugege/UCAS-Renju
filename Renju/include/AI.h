#ifndef _AI_H_
#define _AI_H

#include "../include/common.h"
#include "../include/set.h"
#include "../include/bit_board.h"
#include "../include/zobrist.h"

#define MAXDEPTH 4   //搜索深度
#define P_INFINITY 200000000
#define N_INFINITY -200000000

//fail-soft + PVS + 空着裁剪
int alpha_beta(tree pNode, int last_point, unsigned short* last_buf, tree last_move, int depth, int alpha, int beta); 
void AI_operation();

int move_evaluate(int i, int j, int depth);	//着法评估

#endif