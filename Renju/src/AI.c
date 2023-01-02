#include "./AI.h"

int node_num = 0;               //搜索节点数目
int now_score[2] = {4, 0};      //当前黑棋0白棋1的得分
int next_score[2] = {0, 0};     //搜索完毕下完一步棋后的得分
int board_shape[4][30] = {0};   //四个方向所有棋形
int maxdepth = MAXDEPTH;
Line g_move_buf[LENGTH] = {0};  //缓存上一走法的比特棋盘
Tree PV_p;                      //用于指向找到的最佳走法
Node kill_node;
Node last_node;
int chaoshi = 0;

clock_t generate_time, sort_time; //生成走法时间和排序时间
clock_t time_b, time_e;           //开始b 结束e
clock_t start_t, end_t;

int pvs_kill(Tree, int);

void player_op() {
    Tree head;
    head = get_move(g_i, g_j, 0, g_move_buf, g_move);
    free(g_move);
    g_move = head;
}

/* PVS 剪枝算法 负极大值 soft_fail */
int alpha_beta(Tree pNode, Line* last_buf, Tree last_move, int depth, int alpha, int beta) {
    Tree p, head;
    Line buf[LENGTH];
    int v, found_PV = 0, best = N_INFINITY;
    int shape_buf[4];
    int i = pNode->i, j = pNode->j;

    /* 1.得分够高的情况下判断禁手,若有禁手返回正无穷，因为pNode指向上一层的走法 */
    if (player == WHITE && pNode->point_score > 3600) { 
        if (forbid(i, j))
            return P_INFINITY;
        else
            renju[i][j] = BLACK;
    }

    /* 2.判断有没有已经五连,五连返回负无穷*/
    if (win(i, j)) 
        return N_INFINITY;

    /* 3.搜索层数耗尽,返回当前评分 */
    if (depth <= 0){
        if(player == BLACK) 
            return now_score[0] - now_score[1];   
        else 
            return now_score[1] - now_score[0];   
    }

    /* 4.不满足以上三种退出递归条件的话
     *   获取走法，并排序裁剪*/
    head = get_move(i, j, depth, last_buf, last_move);
    if (now_score[bool_player^1] > 2000) 
        kill_cut(head);
    else 
        common_cut(head);
    buf_move_board(buf);

    /* 5.遍历所有走法
     *   ①下棋②进入下一层③悔棋*/
    for (p = head; p->point_score != NULL_SCORE; p++) {
        node_num++;
        i = p->i;
        j = p->j;
        set_bit_board(i ,j);
        set_point(p, shape_buf);
        //如果找到PV走法 用极小窗口
        if (found_PV) {
            v = -alpha_beta(p, buf, head, depth - 1, -alpha - 1, -alpha);
            if (v > alpha && v < beta)
                v = -alpha_beta(p, buf, head, depth - 1, -beta, -alpha);
        } else 
            v = -alpha_beta(p, buf, head, depth - 1, -beta, -alpha);
        reset_point(p, shape_buf);
        re_move_board(j, buf);
        if (v > best) {
            best = v;
            if (v > alpha) {
                alpha = v;
                found_PV = 1;
            }
            if (v >= beta)
                break;
        }
    }
    /* 6.释放get_move中malloc申请的head
     *   返回搜索到的best*/
    free(head);
    head = NULL;

    return best;
}

/* 剪枝算法的入口，也是第一层，大体与alpha-beta一致
 * 第一层需要决定下一步走法、剔除必败点，同时第一层不需要beta限制上界，越大越好 */
int pvs(Tree head, int depth) {
    Tree p;
    Line buf[LENGTH];
    int i, j, v;
    int alpha = N_INFINITY;
    int found_PV = 0;
    int shape_buf[4];

    buf_move_board(buf);
    for (p = head; p->point_score != NULL_SCORE; p++) {
        if (clock() - start_t > 14000) {
            chaoshi = 1;
            break;
        }
        /* 跳过必败点 */
        if (p->point_score == LOSE_SCORE)
            continue;
        i = p->i;
        j = p->j;
        set_bit_board(i, j);
        set_point(p, shape_buf);
        if (found_PV) {
            v = -alpha_beta(p, buf, g_move, depth - 1, -alpha - 1, -alpha);
            if (v > alpha)
                v = -alpha_beta(p, buf, g_move, depth - 1, N_INFINITY, -alpha);
        } else 
            v = -alpha_beta(p, buf, g_move, depth - 1, N_INFINITY, -alpha);

        if (v > alpha) {
            PV_p = p;
            found_PV = 1;
            alpha = v;
            next_score[0] = now_score[0];
            next_score[1] = now_score[1];
        }
        /* 剔除必败点 */
        if (v <= -100000)
            p->point_score = LOSE_SCORE;

        reset_point(p, shape_buf);
        re_move_board(j ,buf);
    }

    return alpha;
}

void AI_op() {
    Tree head, p, buf_head;
    int val, now_depth;

    start_t = clock();
    generate_time = sort_time = 0;
    chaoshi = 0;

    head = get_move(g_i, g_j, 0, g_move_buf, g_move);
    free(g_move);
    g_move = head;

    /*
     *先进行大概15层算杀搜索，失败就进行9层PVS搜索
    */

    int kill_out = 0;

    if (head->point_score >= 3000 && now_score[bool_player ^ 1] < 2000) {
        kill_cut(head);
        for (int kill = 1; kill <= KILL_DEPTH; kill += 2) {
            if ((kill_out = pvs_kill(head, kill)) >= 100000)
                break;
            end_t = clock();
        }
    }
    end_t = clock();
    printf("算杀用时：%.2fs\n", (end_t - start_t) / 1000.0);

    for (p = head; p->j != NULLPOSITION; p++) {
        if (p->point_score == NULL_SCORE || p->point_score == LOSE_SCORE)
            get_point_score(p);
    }


    printf("kill:%d\n", kill_out);
    if (kill_out < 100000) {
        printf("失败\n");
        /* 迭代加深 
        * 迭代加深可以在浅层就剔除必败点，提高深层搜索速度，
        * 同时找到最短赢法，还可以通过浅层信息对节点排序，本程序曾经采用了迭代加深的历史表排序，后来弃用了*/  
        if (now_score[bool_player^1] > 2000)
            kill_cut(head);
        else 
            common_cut(head);

        for (now_depth = 1; now_depth <= maxdepth; now_depth += 1) {
            val = pvs(head, now_depth);
            last_node.i = PV_p->i;
            last_node.j = PV_p->j;
            last_node.left = PV_p->left;
            last_node.right = PV_p->right;
            for (int index = 0; index < 4; index++)
                last_node.shape[index] = PV_p->shape[index];
            if (val >= 100000)
                break;
        }
        if (chaoshi) {
            PV_p = &last_node;
        }
    } else  
        printf("成功\n");

    end_t = clock();
    now_score[0] = next_score[0];
    now_score[1] = next_score[1];
    board_shape[0][PV_p->j] = PV_p->shape[0]; 
    board_shape[1][PV_p->i] = PV_p->shape[1]; 
    board_shape[2][PV_p->left] = PV_p->shape[2]; 
    board_shape[3][PV_p->right] = PV_p->shape[3]; 
    g_i = PV_p->i;
    g_j = PV_p->j;
    // system("cls");
    printf("当前局面分 黑：%d 白：%d\n", now_score[0], now_score[1]);
    printf("总用时%.2fs\n", (end_t - start_t)/1000.0);
    printf("生成走法时间%d\n", generate_time);
    printf("排序时间%d\n", sort_time);
    printf("节点数目%d\n", node_num);
    node_num = 0;
}

/* 下棋包括
 * ①棋盘置点②缓存3进制棋形信息③修改评分*/  
void set_point(Tree p, int *buf) {
    int i = p->i;
    int j = p->j;
    int num, index;
    buf[0] = board_shape[0][j];
    buf[1] = board_shape[1][i];
    buf[2] = board_shape[2][p->left];
    buf[3] = board_shape[3][p->right];
    for (index = 0; index < 4; index++) {
        /* 长度小于5没必要考虑 */
        if((num = get_line_num(i, j, index)) < 5) 
            continue;
        now_score[0] -= score_table[0][num][buf[index]];
        now_score[0] += score_table[0][num][p->shape[index]];
        now_score[1] -= score_table[1][num][buf[index]];
        now_score[1] += score_table[1][num][p->shape[index]];
    }
    board_shape[0][j] = p->shape[0]; 
    board_shape[1][i] = p->shape[1]; 
    board_shape[2][p->left] = p->shape[2]; 
    board_shape[3][p->right] = p->shape[3]; 
    renju[i][j] = player;
    bit_board[j] &= bit_set[i];
    CHANGE_PLAYER;
}

/* 悔棋包括
 * ①棋盘归零②恢复3进制棋形信息③恢复评分*/  
void reset_point(Tree p, int *buf) {
    int i = p->i;
    int j = p->j;
    int num, index;
    CHANGE_PLAYER;
    renju[i][j] = 0;
    bit_board[j] |= ~bit_set[i];
    board_shape[0][j] = buf[0]; 
    board_shape[1][i] = buf[1]; 
    board_shape[2][p->left] = buf[2]; 
    board_shape[3][p->right] = buf[3]; 
    for (index = 0; index < 4; index++) {
        if((num = get_line_num(i, j, index)) < 5) 
            continue;
        now_score[0] += score_table[0][num][buf[index]];
        now_score[0] -= score_table[0][num][p->shape[index]];
        now_score[1] += score_table[1][num][buf[index]];
        now_score[1] -= score_table[1][num][p->shape[index]];
    }
}

/* 
 * 当场面上出现活三、冲四、活四时候需要裁剪一些必然不可能的走法 
 * 事实上当对面活三时只有封堵和成四成五，对方冲四活四时只有封堵或者成五，
 * 但是本函数会将对方能成活三的点也包括，不是那么完美
 */  
int kill_cut(Tree head) {
    Tree p;
    for (p = head; p->j != NULLPOSITION && (p->point_score >= 3000); p++);
    p->point_score = NULL_SCORE;
    if (p - head > 0)
        return 1;
    else
        return 0;
}

/*  当场面平静时，选前二十分大的走法*/  
void common_cut(Tree head) {
    for (int index = 0; head->j != NULLPOSITION && index != 20; head++, index++);
    head->point_score = NULL_SCORE;
}

/*简单的快排*/
void quick_sort(Tree move_set, int left, int right) {
    int i, j, std;

     if (left > right)
        return;
   
    i = left;
    j = right;
    std = move_set[left].point_score;

    while (i != j) {
        while (move_set[j].point_score <= std && j > i)
            j--;
        while (move_set[i].point_score >= std && j > i)
            i++;
        
        if (j > i) 
            exchange(move_set, i, j);
    }
    exchange(move_set, i, left);

    quick_sort(move_set, left, i - 1);
    quick_sort(move_set, i + 1, right);
}

int quick_select(Tree move_set, int left, int right) {
    int i, j, std;

    i = left;
    j = right;
    std = move_set[left].point_score;

	while (i != j){
		while (move_set[j].point_score <= std && j > i)//找到右侧小于基准值的元素
			j--;
		while (move_set[i].point_score >= std && j > i)//找到左侧大于基准值的元素
			i++;
        if (j > i)
            exchange(move_set, i, j);
	}

    exchange(move_set, i, left);

	if (left < right){
		if (i < 15)//因为k表示第k个数，而s为下标，故k-1，第k小的数排完序后下标为k-1
			i = quick_select(move_set, i + 1, right);
		else if (i > 20)
			i = quick_select(move_set, left, i - 1);
		else
			return i;
	}
}

/*交换两个Node的元素*/
void exchange(Tree move_set, int i, int j) {
    Node temp;
    temp = move_set[i];
    move_set[i] = move_set[j];
    move_set[j] = temp;
}

#define FAIL 100000
#define SUCCESS -100000
/* 只搜索冲棋走法的ab搜索，我们只看能不能赢 一旦能赢就有PVS零窗口快速结束搜索*/
int alpha_beta_kill(Tree pNode, Line* last_buf, Tree last_move, int depth, int alpha, int beta) {
    Tree p, head;
    Line buf[LENGTH];
    int v, score;
    int found_PV = 0;
    int best = N_INFINITY;
    int i = pNode->i, j = pNode->j;
    int shape_buf[4];

    /* 得分够高的情况下判断禁手 */ 
    if (player == WHITE && pNode->point_score > 3600) { 
        if (forbid(i, j))
            return P_INFINITY;
        else
            renju[i][j] = BLACK;
    }

    /* 先判断有没有已经五连 五连返回成功 */
    if (win(i, j)) 
        return SUCCESS;

    /* 搜索层数耗尽，直接返回失败标志 */
    if (depth == 0){
        return -1;   
    }
    //获取走法
    head = get_move(i, j, depth, last_buf, last_move);
    /* 当场面上没有冲棋走法以及封堵时，退出搜索*/
    if (!kill_cut(head)) {
        free(head);
        head = NULL;
        return 0;   
    }
    buf_move_board(buf);

    for (p = head; p->point_score != NULL_SCORE; p++) {
        node_num++;
        i = p->i;
        j = p->j;

        set_bit_board(i ,j);
        set_point(p, shape_buf);
        //如果找到PV走法 用极小窗口
        if (found_PV) {
            v = -alpha_beta_kill(p, buf, head, depth - 1, -alpha - 1, -alpha);
            if (v > alpha && v < beta)
                v = -alpha_beta_kill(p, buf, head, depth - 1, -beta, -alpha);
        } else 
            v = -alpha_beta_kill(p, buf, head, depth - 1, -beta, -alpha);

        if (v > best) {
            best = v;
            if (v > alpha) {
                alpha = v;
                found_PV = 1;
            }
            if (v >= beta) {
                reset_point(p, shape_buf);
                re_move_board(j, buf);
                break;
            }
        }
        reset_point(p, shape_buf);
        re_move_board(j, buf);
    }
    free(head);
    head = NULL;

    return best;
}

int pvs_kill(Tree head, int depth) {
    Tree p;
    Line buf[LENGTH];
    int i, j, v;
    int alpha = 0;
    int found_PV = 0;
    int shape_buf[4];

    buf_move_board(buf);
    for (p = head; p->point_score != NULL_SCORE; p++) {
        /* 跳过必败点 */
        if (clock() - start_t > 7500)
            break;
        if (p->point_score == LOSE_SCORE)
            continue;
        i = p->i;
        j = p->j;
        set_bit_board(i, j);
        set_point(p, shape_buf);
        if (found_PV) {
            v = -alpha_beta_kill(p, buf, g_move, depth - 1, -alpha - 1, -alpha);
            if (v > alpha)
                v = -alpha_beta_kill(p, buf, g_move, depth - 1, N_INFINITY, -alpha);
        } else 
            v = -alpha_beta_kill(p, buf, g_move, depth - 1, N_INFINITY, -alpha);

        if (v > alpha) {
            kill_node.i = p->i; 
            kill_node.j = p->j; 
            kill_node.left = p->left; 
            kill_node.right = p->right; 
            for (int index = 0; index < 4; index++)
                kill_node.shape[index] = p->shape[index]; 
            PV_p = &kill_node;
            next_score[0] = now_score[0];
            next_score[1] = now_score[1];
            found_PV = 1;
            alpha = v;
        }
        if (v == 0)
            p->point_score = LOSE_SCORE;

        reset_point(p, shape_buf);
        re_move_board(j ,buf);
    }

    return alpha;
}
