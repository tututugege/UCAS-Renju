//通用头文件 几乎每部分都会用到的函数和宏
#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define LENGTH 15   //棋盘长度
#define EMPTY 0		//空位
#define BLACK 1 	//黑棋	
#define WHITE 2		//白棋
#define NULLPOSITION 255 //表示非可下棋位置

//切换玩家 1变2 2变1
#define CHANGE_PLAYER player ^= 0b11

typedef unsigned char Chess_point;
typedef unsigned short Line;
typedef unsigned long long Board_key;

typedef struct iNode {
	Chess_point position;	  //节点对应下法
	short history_score;      //历史表得分，用于排序
} Node;

typedef Node *Tree; //树

extern int g_i, g_j;	//记录当前下棋位置
extern int player;    //记录当前棋局的下棋人
extern int renju[LENGTH][LENGTH];   //棋盘作为全局变量
extern clock_t sort_time, generate_time ,time_e, time_b; //调试用时间管理
extern int node_num, num_zobrist;
extern int base[9];   //哈希匹配权值  三进制 改进空间为四进制结合位操作
extern Board_key zobrist[LENGTH][LENGTH][2];   //置换表(transposition table, TT)
extern Board_key now_key; //当前键值

extern Line g_last_buf[5];  //缓存变化前的未改变的着法部分
extern Tree g_move;   //缓存上一步所得的走法

int within_range(int i); //在0-14闭区间内
int in_range(int i, int j); //i, j都在0到14内
int win(int i, int j); //判断输赢
int is_full(); //判断棋盘有没有下满
void set_point(int i, int j);
void reset_point(int i, int j);

#endif