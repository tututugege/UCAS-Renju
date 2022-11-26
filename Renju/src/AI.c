#include "../include/set.h"
#include "../include/AI.h"

void set_bit_board(int i, int j);

int AI_i = 7;
int AI_j = 7;
int node_num = 0;

clock_t evaluate_time, generate_time, sort_time;
clock_t time_b,time_e;
table* tt;

//PVS 剪枝算法 负极大值 soft_fail
int alpha_beta(tree pNode, int last_point, line* last_buf, tree last_move, int depth, int alpha, int beta) {
    tree p, head;
    line buf[5];
    int v, point;
    int found_PV = 0;
    int best = N_INFINITY;
    int i = pNode->position >> 4;
    int j = pNode->position & LENGTH;

    //得分够高的情况下判断禁手
    if (player == WHITE && last_point > 2000 && forbid(i, j))
        return P_INFINITY;

    //先判断有没有已经五连
    if (win(i, j)) {
        return N_INFINITY;
    } 
    //搜索层数耗尽
    if (depth <= 0) {
        if(player == BLACK) return last_point;   
        else return -last_point;   
    }
    //获取走法
    head = get_move(last_buf, last_move, pNode->position, j);

    for (p = head; p->position != NULLPOSITION; p++) {
        i = p->position >> 4;
        j = p->position & LENGTH;
        buf_bit_move(j, buf);
        time_b = clock();
        point = last_point + move_evaluate(i, j, depth); //走法评估
        time_e = clock();
        evaluate_time += time_e - time_b;

        //如果找到PV走法 用极小窗口
        if (found_PV) {
            v = -alpha_beta(p, point, buf, head, depth - 1, -alpha - 1, -alpha);
            if (v > alpha && v < beta)
                v = -alpha_beta(p, point, buf, head, depth - 1, -beta, -alpha);
        } else 
            v = -alpha_beta(p, point, buf, head, depth - 1, -beta, -alpha);
        reset_point(i, j);
        reset_bit_move(j, buf);
        if (v > best) {
            best = v;
            if (v > alpha)
                alpha = v;
            if (v >= beta)
                break;
        }
    }
    free(head);
    head = NULL;
    // 这里释放内存到第十步就会堆错误
    return best;
}

void AI_operation() {
    clock_t start_t, end_t;
    Node *head, *p;
    line buf[5];
    int alpha, beta, now_depth, point;
    int v, i, j;
    int found_PV = 0;

    evaluate_time = generate_time = sort_time = 0;
    tt = init_table();
    start_t = clock();
    head = get_move(g_last_buf, g_move, AI_i * (LENGTH + 1) + AI_j, AI_j);
    free(g_move);
    g_move = head;
    // for (p = head; p->position != NULLPOSITION; p++)
    //     printf("%c%d\n", ((p->position) & LENGTH) + 'a', ((p->position) >> 4) + 1);
    // system("pause");
    //迭代加深  
    for (now_depth = 1; now_depth <= MAXDEPTH; now_depth += 1) {
        //必须先初始化  否则当程序找不到合适走法就会覆盖上一轮对手走法
        AI_i = head->position >> 4;
        AI_j = head->position & LENGTH;
        beta = P_INFINITY;
        alpha = N_INFINITY;

        for (p = head; p->position != NULLPOSITION; p++) {
            i = p->position >> 4;
            j = p->position & LENGTH;
            buf_bit_move(j, buf);
            point = move_evaluate(i , j, now_depth);
            if (found_PV) {
                v = -alpha_beta(p, point, buf, g_move, now_depth - 1, -alpha - 1, -alpha);
                if (v > alpha)
                    v = -alpha_beta(p, point, buf, g_move, now_depth - 1, -beta, -alpha);
            } else 
                v = -alpha_beta(p, point, buf, g_move, now_depth - 1, -beta, -alpha);
            reset_point(i, j);
            reset_bit_move(j ,buf);
            // printf("%c%d %d ", j + 'a', i++, v);
            if (v > alpha) {
                found_PV = 1;
                AI_i = p->position >> 4;
                AI_j = p->position & LENGTH;
                alpha = v;
            }
        }
        // if (alpha >= 10000)
        //     break;
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
    free(tt->data);
    tt->data = NULL;
    free(tt);
    tt = NULL;
}

void set_point(int i, int j) {
    fir[i][j] = player;
    set_bit_board(i, j);
    now_key ^= zobrist[i][j][player - 1];
    change_player();
}

void reset_point(int i, int j) {
    change_player();
    now_key ^= zobrist[i][j][player - 1];
    fir[i][j] = 0;
    bit_board[j] |= ~bit_set[i];
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
