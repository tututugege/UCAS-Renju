#include "./bit_board.h"

/*
 *位棋盘bit_board由15个short(2字节16位)组成 0表示该位有棋子
 *走法位棋盘bit_move_board中 1表示为当前有价值的走法(两步内有其他棋子)
 *每下一步棋就将已有的bit_move_set模板和我们的位棋盘异或即可获得新走法
 *进行或操作即可更新bit_move_board
 *预先缓存哈希表move_table，根据索引直接取出该行能走的位置
 *move_table中以每行的位棋盘值为索引，直接获得能走的位置的横坐标
 *64位long long可以存储16个16进制数 因此可以用其中15个表示一行的15个位置是否可以下棋
 *位棋盘速度非常快，测试时生成一千万个节点所用时间在100ms左右
 */

unsigned long long move_table[MOVE_NUM]; //走法缓存表

/* 每次下棋都将对应的模板在相应的5x5区域内作或操作*/
const Line bit_move_set[LENGTH][3] = {
	0b0111000000000000, 0b0110000000000000, 0b0101000000000000,
	0b0111100000000000, 0b0111000000000000, 0b0010100000000000,
	0b0111110000000000, 0b0011100000000000, 0b0101010000000000,
	0b0011111000000000, 0b0001110000000000, 0b0010101000000000,
	0b0001111100000000, 0b0000111000000000, 0b0001010100000000,
	0b0000111110000000, 0b0000011100000000, 0b0000101010000000,
	0b0000011111000000, 0b0000001110000000, 0b0000010101000000,
	0b0000001111100000, 0b0000000111000000, 0b0000001010100000,
	0b0000000111110000, 0b0000000011100000, 0b0000000101010000,
	0b0000000011111000, 0b0000000001110000, 0b0000000010101000,
	0b0000000001111100, 0b0000000000111000, 0b0000000001010100,
	0b0000000000111110, 0b0000000000011100, 0b0000000000101010,
	0b0000000000011111, 0b0000000000001110, 0b0000000000010101,
	0b0000000000001111, 0b0000000000000111, 0b0000000000001010,
	0b0000000000000111, 0b0000000000000011, 0b0000000000000101,
};

const Line bit_set[LENGTH] = {
	0b1011111111111111,
	0b1101111111111111,
	0b1110111111111111,
	0b1111011111111111,
	0b1111101111111111,
	0b1111110111111111,
	0b1111111011111111,
	0b1111111101111111,
	0b1111111110111111,
	0b1111111111011111,
	0b1111111111101111,
	0b1111111111110111,
	0b1111111111111011,
	0b1111111111111101,
	0b1111111111111110
};

Line bit_board[LENGTH] = {0};
Line bit_move_board[LENGTH] = {0};

/* 初始化bit_board,全部置零*/
void init_bit_board() {
	for (int i = 0; i < LENGTH; i++)
		bit_board[i] = ~0;
}

/* 初始化move_table*/
void init_move_table() {
    int index1, index2, temp;
    unsigned long long val;

	g_move = (Tree)malloc(sizeof(Node) * 2);
	g_move->i = 7;
	g_move->j = 7;
    g_move->left = LEFT_I(7, 7);
    g_move->right = RIGHT_I(7, 7);
    for (index1 = 0; index1 < 4; index1++)
        g_move->shape[index1] = 0;
	(g_move + 1)->j = NULLPOSITION;
    g_move->point_score = 4;

    for (index1 = 0; index1 < MOVE_NUM; index1++) {
        temp = index1;
        val = 0;
        for (index2 = LENGTH - 1; index2 >= 0; index2--) {
            if (temp & 0x1) {
                val = val << 4;
                val += index2;
            }
            temp = temp >> 1;
        }
        move_table[index1] = val;
    }
}

/*获取走法*/
Tree get_move(int i, int j, int depth, Line* last_buf, Tree last_move) {           
    time_b = clock();
    Tree move_set, p, q;
    int index;

    /*直接申请足够大内存比一个一个申请更快,内存大就是任性*/
    move_set = (Tree)malloc(sizeof(Node)*200); 
    index = get_new_move(move_set, last_buf, j, depth);  
    node_num += index;

    /*获得新走法后直接加上上一步的走法
     *需要去除上一步下的位置i, j*/
    for (p = move_set + index, q = last_move; (q->i != i || q->j != j) && q->j != NULLPOSITION; p++, q++, index++) {
        p->i = q->i;
        p->j = q->j;
        p->left = q->left;
        p->right = q->right;
        if (is_change(i, j, q))
            get_point_score(p);
        else {
            for (int index = 0; index < 4; index++)
                p->shape[index] = q->shape[index];
            p->point_score = q->point_score;
        }
    }
    if (q->j != NULLPOSITION) {
        q++;
        for (; q->j != NULLPOSITION; p++, q++, index++){
            p->i = q->i;
            p->j = q->j;
            p->left = q->left;
            p->right = q->right;
            if (is_change(i, j, q))
                get_point_score(p);
            else {
                for (int index = 0; index < 4; index++)
                    p->shape[index] = q->shape[index];
                p->point_score = q->point_score;
            }
        }
    }

    p->j = NULLPOSITION;
    time_e = clock();
    generate_time += time_e - time_b;
    time_b = clock();
    quick_sort(move_set, 0, index - 1);
    time_e = clock();
    sort_time += time_e - time_b;

    return move_set;
}

/*根据缓存的buf和当前位置5*5取余内取异或获取新走法,返回新走法数目*/
int get_new_move(Tree move_set, Line* last_buf, int j, int depth) {
    int start_j, end_j, i, index;
    unsigned long long val;
    Line new_move;

    index = 0;
    get_range(&start_j, &end_j, j);
    for (; start_j != end_j; start_j++) {
        /*取异或之后用位棋盘将已有棋子的位置置零*/
        new_move = (bit_move_board[start_j] ^ last_buf[start_j]) & bit_board[start_j];
        val = move_table[new_move];
        while (val) {
            i = val & 0b1111;
            move_set[index].i = i;
            move_set[index].j = start_j;
            move_set[index].left = LEFT_I(i, start_j);
            move_set[index].right = RIGHT_I(i, start_j);
            get_point_score(move_set + index);
            val = val >> 4;
            index++;
        }
    }

    return index;
}

/* ij位置下棋后对位棋盘和走法棋盘进行改动 */
void set_bit_board(int i, int j) {
    int start_j, end_j;
    int temp = j, index;

    get_range(&start_j, &end_j, j);
    start_j--;
    for (index = 0; temp != end_j; temp++, index++) {
        bit_move_board[temp] |= bit_move_set[i][index];
        bit_move_board[temp] &= bit_board[temp];
    }
    for (index = 1, j--; j != start_j; j--, index++) {
        bit_move_board[j] |= bit_move_set[i][index];
        bit_move_board[j] &= bit_board[j];
    }
}

/* 恢复move_board */
void re_move_board(int j, Line* buf) {
    int start_j, end_j;

    get_range(&start_j, &end_j, j);
    for (; start_j != end_j; start_j++)
        bit_move_board[start_j] = buf[start_j];
}

/* 缓存move_board */
void buf_move_board(Line* buf) {
    for (int i = 0; i < LENGTH; i++) 
        buf[i] = bit_move_board[i];
}

/* 获取j的范围（主要是为了考虑边界的情况）*/
void get_range(int *pstart_j, int *pend_j, int j) {
    switch (j) {
    case 0: 
        *pstart_j = 0;
        *pend_j = 3;
        break;
    case 1:
        *pstart_j = 0;
        *pend_j = 4;
        break;
    case 13:
        *pstart_j = 11;  
        *pend_j = 15;  
        break;
    case 14:
        *pstart_j = 12;
        *pend_j = 15;
        break;
    default:
        *pstart_j = j - 2;
        *pend_j = j + 3;
        break;
    }
}

/* 判断该点是否与上一步在同一行 以此判断是否需要更新点的信息 */
int is_change(int i, int j, Tree p) {
    if (p->point_score == NULL_SCORE || p->point_score == LOSE_SCORE)
        return 1;

    int now_i = p->i;
    int now_j = p->j;
    int val;

    if ((now_i == i) || (now_j == j))
        return 1;

    if (abs(now_i - i) == abs(now_j - j))
        return 1;

    return 0;
}

/* 仍然存在问题的获取冲棋走法函数 */
// Tree get_move_kill(int i, int j, int depth, Line* last_buf, Tree last_move) {           
//     time_b = clock();
//     Tree move_set, p, q;
//     int index;

//     //直接申请足够大内存比一个一个申请更快
//     move_set = (Tree)malloc(sizeof(Node)*200); 
//     index = get_new_move(move_set, last_buf, j, depth);  
//     node_num += index;

//     //获得新走法后直接加上上一步的走法
//     //需要去除上一步下的位置
//     for (p = move_set + index, q = last_move; (q->i != i || q->j != j) && q->j != NULLPOSITION; p++, q++, index++) {
//         p->i = q->i;
//         p->j = q->j;
//         p->left = q->left;
//         p->right = q->right;
//         get_point_score_kill(p);

//     }
//     if (q->j != NULLPOSITION) {
//         q++;
//         for (; q->j != NULLPOSITION; p++, q++, index++){
//             p->i = q->i;
//             p->j = q->j;
//             p->left = q->left;
//             p->right = q->right;
//             get_point_score_kill(p);
//         }
//     }
//     p->j = NULLPOSITION;
//     time_e = clock();
//     generate_time += time_e - time_b;
//     time_b = clock();
//     quick_sort(move_set, 0, index - 1);
//     time_e = clock();
//     sort_time += time_e - time_b;
    
//     return move_set;
// }