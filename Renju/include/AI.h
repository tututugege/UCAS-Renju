#define MAXDEPTH 4   //搜索深度
#define P_INFINITY 200000000
#define N_INFINITY -200000000
#define HASHSIZE 33554432
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
//记录所有申请的节点地址 方便释放
typedef struct head {
	tree head;
	struct head* next;
} head_node;

extern table* tt;

tree get_move(int parent_pointd, int depth);                   //返回每个位置的位置分，更有可能赢或者输的点在前 决定搜索树每层的顺序 方便剪枝

int init_evaluate(int* board);
int point_evaluate(int i, int j, int depth);						//总评估的封装函数
int move_evaluate(int i, int j, int depth);						//总评估的封装函数
int init_point_evaluate(int i, int j, int dx, int dy);

int alpha_beta(tree pNode, int depth, int alpha, int beta); //MINMAX算法+alpha—beta剪枝

tree get_memory();
void free_all();

tree sort(tree p_head, tree p_end);						//对链表排序(归并排序)
tree merge(tree left, tree right);
void resort(tree p, tree first_child);

table* init_table();
int TT_search(int depth);
void TT_insert(int point, int depth);
int zobrist_hash(unsigned long long key);