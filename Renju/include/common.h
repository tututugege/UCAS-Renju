//通用头文件
#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LENGTH 15   //棋盘长度
#define EMPTY 0		//空位
#define BLACK 1 	//黑棋	
#define WHITE 2		//白棋
#define NULLPOSITION 255

typedef struct iNode {
	unsigned char position;	  //节点对应下法
	short history_point;
} Node;

typedef unsigned short line;
typedef Node *tree; //树

extern int g_i, g_j;
extern int player;    //记录当前棋局的下棋人
extern int fir[LENGTH][LENGTH];   //棋盘作为全局变量
extern unsigned long long now_key; //当前键值
extern clock_t generate_time ,time_e, time_b;
extern int node_num;
extern int base[9];   //哈希匹配权值  三进制
extern unsigned long long zobrist[LENGTH][LENGTH][2];   //置换表(transposition table, TT)

extern line g_last_buf[5];
extern tree g_move;

int within_range(int i); //在0-14闭区间内
int win(int i, int j); //判断输赢
void change_player();  //换人
int is_full();
void set_point(int i, int j);
void reset_point(int i, int j);

#endif