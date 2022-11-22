#include "../include/set.h"
#include "../include/AI.h"

int AI_i = 7;
int AI_j = 7;
int node_num = 0;

clock_t evaluate_time, generate_time, sort_time;
clock_t time_b,time_e;
move_head_node *list = NULL;      //当前申请的链表表头
move_head_node *list_head = NULL; //申请链表的表头
table* tt;

//申请内存
tree get_memory() {
    tree head, p; 

    //一次性申请大内存比一次一次申请快
    head = p = list->move_head = (tree)malloc(sizeof(Node) * 80);
    for (int i = 0; i < 79; i++, p++)
        p->bro = p + 1;
    p->bro = NULL;

    list->next = (move_head_node*)malloc(sizeof(move_head_node));
    list = list->next;
    list->next = NULL;
    list->move_head = NULL;
    return head;
}

tree get_move(int parent_point, int depth) {            
    time_b = clock();
    tree move_set, p, r;
    int i, j, now_i, now_j, search_i, search_j;
    int value, index;
    int num = 0;

    p = move_set = get_memory(); 
    for (i = 0; i < LENGTH; i++) {
        for (j = 0; j < LENGTH; j++) {
            value = 0;
            if (!fir[i][j]) {
                for (now_i = i - 2, now_j = j - 2; now_j < j + 3; now_j++) {
                    for (search_i = now_i, index = 0; index < 5; search_i++, index++){
                        if (within_range(search_i) && within_range(now_j))
                            value = value || fir[search_i][now_j];
                        if (value) break;
                    }
                    if (value) break;
                }
                if (value && p) {
                    p->position = (unsigned char)(i * (LENGTH + 1) + j);
                    p->point = parent_point;
                    p->first_child = NULL;
                    r = p;
                    p = p->bro;
                    node_num++;
                }
            }
        }
    } 
    // 截断
    if(r) 
        r->bro = NULL;
    
    time_e = clock();
    generate_time += time_e - time_b;

    return move_set;
}

//PVS 剪枝算法 负极大值 soft_fail
int alpha_beta(tree pNode, int depth, int alpha, int beta) {
    Node *p;
    int v;
    int found_PV = 0;
    int best = N_INFINITY;
    int i = pNode->position >> 4;
    int j = pNode->position & LENGTH;
    //先判断有没有已经五连
    if (win(i, j)) {
        return N_INFINITY;
    } 
    //搜索层数耗尽
    if (depth <= 0) {
        if(player == BLACK) return pNode->point;   
        else return -pNode->point;   
    }
    //获取走法
    if (pNode->first_child == NULL) 
        pNode->first_child = get_move(pNode->point, depth - 1);

    for (p = pNode->first_child; p; p = p->bro) {
        i = p->position >> 4;
        j = p->position & LENGTH;

        time_b = clock();
        if (p->point == pNode->point) 
            p->point = move_evaluate(i, j, depth); //走法评估
        else 
            set_point(i, j);
        time_e = clock();
        evaluate_time += time_e - time_b;

        //如果找到PV走法 用极小窗口
        if (found_PV) {
            v = -alpha_beta(p, depth - 1, -alpha - 1, -alpha);
            if (v > alpha && v < beta)
                v = -alpha_beta(p, depth - 1, -beta, -alpha);
        } else 
            v = -alpha_beta(p, depth - 1, -beta, -alpha);
        reset_point(i, j);
        p->result_point = v;
        if (v > best) {
            best = v;
            if (v > alpha)
                alpha = v;
            if (v >= beta)
                break;
        }
    }
    resort(p, &(pNode->first_child));

    return best;
}

void AI_operation() {
    clock_t start_t, end_t;
    Node *head, *p, *r;
    int alpha, beta, now_depth;
    int v, i, j;
    int found_PV = 0;

    evaluate_time = generate_time = sort_time = 0;
    list = list_head = (move_head_node*)malloc(sizeof(move_head_node));
    tt = init_table();
    start_t = clock();
    head = get_move(0, 0);

    for(p = head; p; p = p->bro) {
        time_b = clock();
        i = p->position >> 4;
        j = p->position & LENGTH;
        p->result_point = p->point = move_evaluate(i, j ,0);
        reset_point(i, j);
        time_e = clock();
        evaluate_time += time_e - time_b;
    }

    //迭代加深  
    for (now_depth = 2; now_depth <= MAXDEPTH; now_depth += 1) {
        resort(p, &head);

        //必须先初始化  否则当程序找不到合适走法就会覆盖上一轮对手走法
        AI_i = head->position >> 4;
        AI_j = head->position & LENGTH;
        r = NULL;
        beta = P_INFINITY;
        alpha = N_INFINITY;

        for (p = head; p; r = p, p = p->bro) {
            i = p->position >> 4;
            j = p->position & LENGTH;
            set_point(i, j);
            if (found_PV) {
                v = -alpha_beta(p, now_depth - 1, -alpha - 1, -alpha);
                if (v > alpha)
                    v = -alpha_beta(p, now_depth - 1, -beta, -alpha);
            } else 
                v = -alpha_beta(p, now_depth - 1, -beta, -alpha);
            reset_point(i, j);

            printf("%c%d %d ", j + 'a', i++, v);
            if (v > alpha) {
                found_PV = 1;
                AI_i = p->position >> 4;
                AI_j = p->position & LENGTH;
                alpha = v;
                if (r) {
                    r->bro = p->bro;
                    p->bro = head;
                    head = p;
                    p = r;
                }
            }
        }
        // system("pause");
    }
    free_all();

    end_t = clock();
    printf("总用时%d\n", end_t - start_t);
    printf("生成走法时间%d\n", generate_time);
    printf("排序时间%d\n", sort_time);
    printf("评估时间%d\n", evaluate_time);
    printf("节点数目%d\n", node_num);
    node_num = 0;
}

void free_all() {
    for(list = list_head; list_head; list = list_head) {
        list_head = list_head->next;
        free(list->move_head);
        free(list);
    }
    free(tt->data);
    free(tt);
    tt = NULL;
}

void set_point(int i, int j) {
    fir[i][j] = player;
    now_key ^= zobrist[i][j][player - 1];
    change_player();
}

void reset_point(int i, int j) {
    change_player();
    now_key ^= zobrist[i][j][player - 1];
    fir[i][j] = 0;
}

tree sort(tree p_head, tree p_end) {
	if( p_head == p_end ) return p_head;

	tree fast, slow;
	fast = slow = p_head;
	while (fast != p_end && fast->bro != p_end) {
		fast = fast->bro->bro;
		slow = slow->bro;
	}

	tree right = sort(slow->bro, p_end);
	slow->bro = NULL;
	tree left = sort(p_head, slow);

	return merge(left, right);
}

tree merge(tree left, tree right) {
	if (left == NULL || right == NULL) 
        return (left == NULL ? right : left);

    if (left->result_point > right->result_point) {
        left->bro = merge(left->bro, right);
        return left;
    } else {
        right->bro = merge(left, right->bro);
        return right;
    }
}

void resort(tree p, tree* p_first_child) {
    clock_t time_b, time_e;
    tree head, r;
    time_b = clock();

    if ( p ) {
        head = p->bro;
        p->bro = NULL;
        *p_first_child = sort(*p_first_child, NULL);
        //有待改进 这个遍历似乎很浪费
        for(p = *p_first_child; p; r = p, p = p->bro)
            ;
        r->bro = head;
    } else {
        *p_first_child = sort(*p_first_child, NULL);
    }
    time_e = clock();
    sort_time += time_e - time_b;
}

//对置换表长度取余的对应的下标
int zobrist_hash(unsigned long long key) {
    return (int)(key & (HASHSIZE - 1));
}

//初始化一个置换表
table* init_table() {
    int i;
    table* p_table = (table*)malloc(sizeof(table));
    p_table->data = (item*)malloc(sizeof(item) * HASHSIZE);
    for (i = 0; i < HASHSIZE; i++) {
        p_table->data[i].key = NULLKEY;
    }
    return p_table;
}

void TT_insert(int point, int depth) {
    int index = zobrist_hash(now_key);
    (tt->data)[index].depth = (char)depth; 
    (tt->data)[index].key = now_key; 
    (tt->data)[index].point = point; 
}

int TT_search(int depth) {
    int index = zobrist_hash(now_key);
    if (((tt->data)[index]).key != NULLKEY && ((tt->data)[index]).depth <= depth && (tt->data[index].key == now_key))
        return tt->data[index].point;
    else return NULLKEY;
} 