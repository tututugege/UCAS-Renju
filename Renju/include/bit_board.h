#ifndef _BIT_BOARD_H_
#define _BIT_BOARD_H_

#include "../include/common.h"
#include "../include/set.h"
#define MOVE_NUM 32768

void init_move_table();
void init_bit_board();                //初始化位棋盘，0表示已下棋，1表示未下棋
Tree get_move(int position, int j, Line* last_buf, Tree last_move);           
int get_new_move(Tree move_set, Line* last_buf, int j);
void set_bit_board(int i, int j);
void buf_bit_move(int j, Line* buf);
void re_bit_move(int j, Line* buf);
void get_range(int* start_j, int *end_j, int j);

extern Line bit_board[];
extern Line bit_set[];

#endif