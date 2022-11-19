#ifndef _SET_H_
#define _SET_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//棋盘相关宏
#define BLACK 1 	//黑棋	
#define WHITE 2		//白棋
#define LASTBLACK 3 //刚下的黑棋
#define LASTWHITE 4 //刚下的白棋
#define LENGTH 15   //棋盘长度
//AI算法相关
#define MAXDEPTH 5   //搜索深度
#define P_INFINITY 200000000
#define N_INFINITY -200000000
#define HASHSIZE 33554432
#define NULLKEY 33554432
#define NULLPOSITION 225
#define BIT_0 0b111000000000000
#define BIT_1 0b111100000000000
#define BIT_2 0b111110000000000
#define BIT_3 0b011111000000000
#define BIT_4 0b001111100000000
#define BIT_5 0b000111110000000
#define BIT_6 0b000011111000000
#define BIT_7 0b000001111100000
#define BIT_8 0b000000111110000
#define BIT_9 0b000000011111000
#define BIT_10 0b000000001111100
#define BIT_11 0b000000000111110
#define BIT_12 0b000000000011111
#define BIT_13 0b000000000001111
#define BIT_14 0b000000000000111

#ifndef _TREE_
#define _TREE_
typedef struct Node {
	unsigned char position;	  //节点对应下法
	int point;
	int result_point;
	struct Node *bro; 
	struct Node *first_child;
} Node; //二叉树节点 儿子兄弟表示法

typedef Node *tree; //树
#endif

//记录所有申请的节点地址 方便释放
typedef struct head {
	tree head;
	struct head* next;
} head_node;

extern table* tt;
extern int player;    //记录当前棋局的下棋人
extern int fir[LENGTH][LENGTH];   //棋盘作为全局变量
extern unsigned long long now_key; //当前键值
extern int base[9];   //哈希匹配权值  三进制
extern int AI_i;   
extern int AI_j;      //ai决定的下棋坐标
extern unsigned long long zobrist[LENGTH][LENGTH][2];   //置换表(transposition table, TT)
extern int node_num;
extern short bit_board[LENGTH];

int is_full();
void set_board(); //打印当前局势的棋盘
int* set(int* position);      //下棋 
int* AI_set(int* position);    //AI下棋
int win(int i, int j); //判断输赢
int within_range(int i); //在0-14闭区间内
void change_player();  //换人

tree get_move(int parent_pointd, int depth);                   //返回每个位置的位置分，更有可能赢或者输的点在前 决定搜索树每层的顺序 方便剪枝
void init_point_table();
int init_evaluate(int* board);
int point_evaluate(int i, int j, int depth);						//总评估的封装函数
int move_evaluate(int i, int j, int depth);						//总评估的封装函数
int init_point_evaluate(int i, int j, int dx, int dy);
int alpha_beta(tree pNode, int depth, int alpha, int beta); //MINMAX算法+alpha—beta剪枝
void AI_operation();                            //ai下棋的封装
void set_point(int i, int j);
void reset_point(int i, int j);
tree get_memory();
void free_all();
tree sort(tree p_head, tree p_end);						//对链表排序(归并排序)
tree merge(tree left, tree right);


void get_rand();
table* init_table();
int TT_search(int depth);
void TT_insert(int point, int depth);
int zobrist_hash(unsigned long long key);
void resort(tree p, tree first_child);

#endif