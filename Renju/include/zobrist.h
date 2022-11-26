#ifndef _ZOBRIST_H_
#define _ZOBRIST_H_

#include "../include/common.h"

#define HASHSIZE 33554432 //置换表大小
#define NULLKEY 33554432  

typedef struct hash_item {
	unsigned long long key;
	int point;    //分数值
	char depth;	  //节点深度
} item;

typedef struct hash_table {
	item *data;
} table;

extern table* tt; //置换表

void init_rand();    //初始化随机数
table* init_table(); //初始化置换表
int TT_search(int depth); //查表
void TT_insert(int point, int depth); //插入数据
int zobrist_hash(unsigned long long key); //对当前key取index
void free_all(); //释放哈希表内存

#endif