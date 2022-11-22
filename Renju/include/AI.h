#ifndef _AI_H_
#define _AI_H

#define MAXDEPTH 4   //搜索深度
#define P_INFINITY 200000000
#define N_INFINITY -200000000
#define HASHSIZE 33554432 //置换表大小
#define NULLKEY 33554432  
#define NULLPOSITION 225

typedef struct hash_item {
	unsigned long long key;
	int point;    //分数值
	char depth;	  //节点深度
} item;

typedef struct hash_table {
	item *data;
} table;

typedef struct move_head {
	tree move_head;
	struct move_head* next;
} move_head_node;
//链表记录所有申请的节点地址 用于搜索结束后free 

extern table* tt; //置换表
extern int AI_i;
extern int AI_j;

tree get_memory(); //申请内存
void free_all(); //释放内存
tree get_move(int parent_pointd, int depth); 			//获取着法                   
int move_evaluate(int i, int j, int depth);	 			//着法评估
int alpha_beta(tree pNode, int depth, int alpha, int beta); //fail-soft + PVS + 空着裁剪
tree sort(tree p_head, tree p_end);						//链表排序(归并排序)
tree merge(tree left, tree right);
void resort(tree p, tree* p_first_child);
										
table* init_table(); //初始化置换表
int TT_search(int depth); //查表
void TT_insert(int point, int depth); //插入数据
int zobrist_hash(unsigned long long key); //对当前key取index

#endif