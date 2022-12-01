#include "../include/AI.h"
//目前AI主要考虑时间性能 采用PVS改进版AB剪枝算法+简单置换表+位棋盘生成着法+简单评估
//15s内大约能搜一千万个节点 但是每层走法过多 深度不是很理想
//bug1：当AI发现在搜索深度内无论怎么下都是死 就会不挣扎而瞎下 然而好还是挣扎一下

//主要考虑优化方向：位棋盘评估降低评估时间 采用更好的排序方式对历史表排序 优化生成着法降低每层节点数目
//                 VCT VCF杀手着法

int g_score = 80;
int node_num = 0;
int g_depth = 0;
int buf_score = 0;
Board_key now_key = 0;
Board_key zobrist[LENGTH][LENGTH][2] = {0};

clock_t evaluate_time, generate_time, sort_time;
clock_t time_b, time_e;

void player_op() {
    Tree head;
    head = get_move(g_i * (LENGTH + 1) + g_j, g_j, g_last_buf, g_move);
    free(g_move);
    g_move = head;
}

//PVS 剪枝算法 负极大值 soft_fail
int alpha_beta(Tree pNode, int last_score, Line* last_buf, Tree last_move, int depth, int alpha, int beta) {
    Tree p, head;
    Line buf[5];
    int v, score;
    int found_PV = 0;
    int best = N_INFINITY;
    int i = pNode->position >> 4;
    int j = pNode->position & LENGTH;

    // 得分够高的情况下判断禁手
    // if (player == WHITE && last_score > 2000) {
    //     test_time_b = clock();
    //     forbid(i, j);
    //     test_time_e = clock();
    //     time1  += test_time_e - test_time_b;

    // }

    //先判断有没有已经五连
    if (win(i, j)) {
        return N_INFINITY;
    } 
    //搜索层数耗尽
    // if (depth <= 0 && !is_killer(i, j)) {
        //如果是活三冲四，就冲棋延申/静态评估，直到无冲棋
    if (depth <= 0){
        if(player == BLACK) return last_score;   
        else return -last_score;   
    }
    //获取走法
    head = get_move(pNode->position, j, last_buf, last_move);

    g_depth++;
    for (p = head; p->position != NULLPOSITION; p++) {
        node_num++;
        i = p->position >> 4;
        j = p->position & LENGTH;

        //可优化，无需存储buf，只需要一个原数组和索引j即可缓存所有
        buf_bit_move(j, buf);
        time_b = clock();
        score = move_evaluate(i, j, last_score, depth); //走法评估
        time_e = clock();
        evaluate_time += time_e - time_b;

        //如果找到PV走法 用极小窗口
        if (found_PV) {
            v = -alpha_beta(p, score, buf, head, depth - 1, -alpha - 1, -alpha);
            if (v > alpha && v < beta)
                v = -alpha_beta(p, score, buf, head, depth - 1, -beta, -alpha);
        } else 
            v = -alpha_beta(p, score, buf, head, depth - 1, -beta, -alpha);
        reset_point(i, j);
        re_bit_move(j, buf);
        if (v > best) {
            best = v;
            if (v > alpha)
                INS_HIS(p->position);
                alpha = v;
            if (v >= beta)
                break;
        }
    }
    free(head);
    head = NULL;
    g_depth--;

    return best;
}

int pvs(Tree head, int depth) {
    Tree p;
    Line buf[5];
    int i, j, score, v;
    int alpha = N_INFINITY;
    int found_PV = N_INFINITY;

    g_depth++;
    for (p = head; p->position != NULLPOSITION; p++) {
        i = p->position >> 4;
        j = p->position & LENGTH;
        buf_bit_move(j, buf);
        score = move_evaluate(i , j, g_score, depth);
        if (found_PV) {
            v = -alpha_beta(p, score, buf, g_move, depth - 1, -alpha - 1, -alpha);
            if (v > alpha)
                v = -alpha_beta(p, score, buf, g_move, depth - 1, N_INFINITY, -alpha);
        } else 
            v = -alpha_beta(p, score, buf, g_move, depth - 1, N_INFINITY, -alpha);
        reset_point(i, j);
        re_bit_move(j ,buf);
        if (v > alpha) {
            found_PV = 1;
            g_i = i;
            g_j = j;
            buf_score = score;
            alpha = v;
        }
    }
    g_depth--;
    return alpha;
}

void AI_op() {
    clock_t start_t, end_t;
    Tree head, p;
    int now_depth, val;

    start_t = clock();
    evaluate_time = generate_time = sort_time = num_zobrist = 0;

    head = get_move(g_i * (LENGTH + 1) + g_j, g_j, g_last_buf, g_move);

    free(g_move);
    g_move = head;

    //迭代加深  
    for (now_depth = 1; now_depth <= MAXDEPTH; now_depth += 1) {
        //必须先初始化  否则当程序找不到合适走法就会覆盖上一轮对手走法
        g_i = head->position >> 4;
        g_j = head->position & LENGTH;
        val = pvs(head, now_depth);
        if (val >= 100000)
            break;
    }

    end_t = clock();
    g_score = buf_score;
    clear_history_table();

    printf("总用时%d\n", end_t - start_t);
    printf("生成走法时间%d\n", generate_time);
    printf("排序时间%d\n", sort_time);
    printf("评估时间%d\n", evaluate_time);
    printf("节点数目%d\n", node_num);
    printf("zobrist命中次数%d\n", num_zobrist);
    // printf("time:%d\n", time1);
    printf("当前局面评分%d\n", g_score);
    node_num = 0;
}

void set_point(int i, int j) {
    renju[i][j] = player;
    set_bit_board(i, j);
    now_key ^= zobrist[i][j][player - 1];
    CHANGE_PLAYER;
}

void reset_point(int i, int j) {
    CHANGE_PLAYER;
    now_key ^= zobrist[i][j][player - 1];
    renju[i][j] = 0;
    bit_board[j] |= ~bit_set[i];
}

