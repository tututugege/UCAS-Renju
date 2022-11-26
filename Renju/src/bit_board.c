#include "../include/set.h"
#include "../include/bit_board.h"

extern clock_t generate_time ,time_e, time_b;
extern int node_num;
unsigned long long move_table[MOVE_NUM];
line bit_move_set[LENGTH] = {
	0b0111000000000000,
	0b0111100000000000,
	0b0111110000000000,
	0b0011111000000000,
	0b0001111100000000,
	0b0000111110000000,
	0b0000011111000000,
	0b0000001111100000,
	0b0000000111110000,
	0b0000000011111000,
	0b0000000001111100,
	0b0000000000111110,
	0b0000000000011111,
	0b0000000000001111,
	0b0000000000000111
};

line bit_set[LENGTH] = {
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

//位棋盘bit_board由15个short(2字节16位)组成
//为1代表该位置是当前步骤下的着法点
//预先缓存move_table，根据索引直接取出该行能走的位置

//初始化步骤表
void init_move_table() {
    int index1, index2, temp;
    unsigned long long val;

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

tree get_move(line* last_buf, tree last_move, int position, int j) {            
    time_b = clock();
    tree move_set, p, q;
    int index;

    move_set = (tree)malloc(sizeof(Node)*200); 
    index = get_new_move(move_set, last_buf, j);  
    node_num += index;

    for (p = move_set + index, q = last_move; q->position != position; p++, q++, index++) {
        p->position = q->position;
        p->history_point = 0;
        node_num++;
        //查历史表
    }
    q++;
    for (; q->position != NULLPOSITION; p++, q++, index++){
        p->position = q->position;
        p->history_point = 0;
        node_num++;
        //查历史表
    }
    p->position = NULLPOSITION;

    time_e = clock();
    generate_time += time_e - time_b;

    return move_set;
}

//获取当前位置可走的横坐标
int get_new_move(tree move_set, line* last_buf, int j) {
    int start_j, end_j, i, index, buf_j;
    unsigned long long val;
    line new_move;

    index = 0;
    get_range(&start_j, &end_j, j);
    for (buf_j = 0; start_j != end_j; start_j++, buf_j++) {
        new_move = (bit_move_board[start_j] ^ last_buf[buf_j]) & bit_board[start_j];
        val = move_table[new_move];
        while (val) {
            i = val & 0b1111;
            // printf("%d ", i);
            move_set[index].position = i*(LENGTH + 1) + start_j;
            val = val >> 4;
            index++;
        }
    }

    return index;
}

void set_bit_board(int i, int j) {
    int start_j, end_j;

    get_range(&start_j, &end_j, j);
    bit_board[j] &= bit_set[i];
    for (; start_j != end_j; start_j++) {
        bit_move_board[start_j] |= bit_move_set[i];
        bit_move_board[start_j] &= bit_board[start_j];
    }
}

void reset_bit_move(int j, line* buf) {
    int start_j, end_j, i;

    get_range(&start_j, &end_j, j);
    for (i = 0; start_j != end_j; start_j++, i++)
        bit_move_board[start_j] = buf[i];
}

void buf_bit_move(int j, line* buf) {
    int start_j, end_j, i;

    get_range(&start_j, &end_j, j);
    for (i = 0; start_j != end_j; start_j++, i++) 
        buf[i] = bit_move_board[start_j];
}

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

