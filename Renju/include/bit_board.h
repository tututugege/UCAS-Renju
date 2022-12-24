#ifndef _BIT_BOARD_H_
#define _BIT_BOARD_H_

/*
 *位棋盘部分,主要用来生成走法并排序
 */

#include "../include/common.h"
#include "../include/set.h"
#include "../include/evaluate.h"

/* 宏定义 */
#define MOVE_NUM 32768          //2^15,表示一行15个子下棋位点的所有情况
#define NULL_SCORE 33554432     //着法裁剪的标志

/* 函数声明 */
void init_move_table();                                                 //初始化走法哈希表
void init_bit_board();                                                  //初始化位棋盘，0表示已下棋，1表示未下棋
Tree get_move(int i, int j, int depth, Line* last_buf, Tree last_move); //获取全部走法           
int get_new_move(Tree move_set, Line* last_buf, int j, int depth);      //每下一步棋得到的新走法
void set_bit_board(int i, int j);                                       //在bit_board上下棋
void buf_move_board(Line* buf);                                         //缓存上一步的bit_move_board
void re_move_board(int j, Line* buf);                                   //恢复上一步的bit_move_board
void get_range(int* start_j, int *end_j, int j);                        
int is_change(int, int, Tree);
void quick_sort(Tree move_set, int left, int right);                    //快排

//Tree get_move_kill(int i, int j, int depth, Line* last_buf, Tree last_move); //获取冲棋走法 配合算杀使用，有亿点点bug，提交版暂时不用   

/* 全局变量 */
/* bit_board由15个16bit的Line构成，每一个bit为1表示为空，为0表示有子*/
extern Line bit_board[];
/* bit_set是bit_board每一个下棋对应的模板，每次下棋都需要对应进行或操作*/
extern const Line bit_set[];
/* g_move_buf是上一步的缓存bit_move_board*/
extern Line g_move_buf[LENGTH];

#endif