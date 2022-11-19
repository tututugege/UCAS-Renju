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

typedef struct hash_item {
	unsigned long long key;
	int point;    //分数值
	char depth;	  //节点深度
} item;

typedef struct hash_table {
	item *data;
} table;
