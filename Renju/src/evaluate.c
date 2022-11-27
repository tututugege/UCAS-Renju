#include "../include/evaluate.h"
int black_point[10] = {5, 10, 50, 100, 500, 1000, 10000, 100000, 1000000};
int white_point[10] = {5, 10, 50, 100, 500, 1000, 10000, 100000, 1000000};
int score_table[NUM_POINT] = {0};

//初始化评分表

int point_evaluate(int i, int j, int depth) {
    return init_point_evaluate(i, j, 1, 0) + init_point_evaluate(i, j, 0, 1) + \
    init_point_evaluate(i, j, 1, 1) + init_point_evaluate(i, j, 1, -1);
}

int move_evaluate(int i, int j, int board_score, int depth) {
    int score; 

    set_point(i, j);
    if ((score = TT_search(depth - 1)) == NULLKEY) {
        score = board_score + point_evaluate(i, j, depth);
        TT_insert(score, depth - 1);
    }
    return score;
}

int init_point_evaluate(int i, int j, int dx, int dy) {
    int num = 0;
    int index1, index2, search_i, search_j; 

    index1 = 0;
    search_i = i - 4*dx;
    search_j = j - 4*dy;
    while (search_i != i || search_j != j) { 
        if (in_range(search_i, search_j)) 
            index1 += fir[search_i][search_j] * base[num];
        search_i += dx;
        search_j += dy;
        num++;
    } 
    
    index2 = fir[i][j] * base[num++];
    for (i += dx, j += dy; num < 9; i += dx, j += dy, num++) {
        if (in_range(i ,j))
            index1 += fir[i][j] * base[num];
    }

    index2 += index1;
    return score_table[index2] - score_table[index1];
}

int init_evaluate(int* board) {
    int index, now_i;    
    int value;
    int black_evaluate[10] = {0};
    int white_evaluate[10] = {0};
    int score = 0;

    for (now_i = 0; now_i < 5; now_i += 1) {
        value = 0;
        for (index = 0; index < 5; index++) {
            value += board[now_i + index] * base[index];
            switch (value) {
                case 0:
                    break;
                case one_1b:
                    black_evaluate[0]++;
                    break;
                case one_2b:
                case one_3b:
                    black_evaluate[1]++;
                    break;
                case one_1w:
                    white_evaluate[0]++;
                    break;
                case one_2w:
                case one_3w:
                    white_evaluate[1]++;
                    break;
                case two_1b:
                    black_evaluate[2]++;
                    break;
                case two_2b:
                case two_3b:
                case two_4b:
                case two_5b:
                    black_evaluate[3]++;
                    break;
                case two_1w:
                    white_evaluate[2]++;
                    break;
                case two_3w:
                case two_2w:
                case two_4w:
                case two_5w:
                    white_evaluate[3]++;
                    break;
                case three_1b:
                    black_evaluate[5]++;
                    break;
                case three_2b:
                case three_3b:
                case three_4b:
                    black_evaluate[4]++;
                    break;
                case three_1w:
                    white_evaluate[5]++;
                    break;
                case three_2w:
                case three_3w:
                case three_4w:
                    white_evaluate[4]++;
                    break;
                case three_5b:
                case three_6b:
                    black_evaluate[3]++;
                    break;
                case three_5w:
                case three_6w:
                    white_evaluate[3]++;
                    break;
                case four_1b:
                case four_2b:
                case four_3b:
                    black_evaluate[5]++;
                    break;
                case four_1w:
                case four_2w:
                case four_3w:
                    white_evaluate[5]++;
                    break;
                case five_b:
                    black_evaluate[6]++;
                    break;
                case five_w:
                    white_evaluate[6]++;
                    break;
            }
        }
    }
    for (int i = 0; i < 10; i++) {
        score += black_evaluate[i]*black_point[i] - white_evaluate[i]*white_point[i];
    }
    return score;
}

void init_point_table() {
    int board[9] = {0};
    int index[9];
    int i, j, temp;
    for (i = 0; i < NUM_POINT; i++) {
        temp = i;
        for (j = 0; j < 9; j++) {
            board[8 - j] = temp % 3;
            temp /= 3;
        }
        score_table[i] = init_evaluate(board);
        temp = i;
        for (j = 0; j < 9; j++) {
            board[j] = temp % 3;
            temp /= 3;
        }
        score_table[i] += init_evaluate(board);
    }
}