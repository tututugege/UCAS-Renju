#include "./evaluate.h"
#include <math.h>

/*评分思路：
 *评分以行为单位，只需72个int即可知道全局评分采用哈希表预先缓存5-15长度的所有可能的行的得分
 *长度为5的直接穷举，长度6-15程序生成 总共约40mb内存 内存大就是任性
 *缓存过程在开始游戏前，因此可以不做太多速度上的优化
 *每次下棋只需要更新4个方向的int即可 */

/*四种坐标变换，本身采用宏函数，为了方便采用了函数指针数组存这几个函数*/
int par_i(int, int);
int par_j(int, int);
int ver_i(int, int);
int ver_j(int, int);
int left_i(int, int);
int left_j(int, int);
int right_i(int, int);
int right_j(int, int);

/* 四个函数指针存四个坐标变换函数 */
int (*transX[4])(int i, int j) = {par_i, ver_i, left_i, right_i};
int (*transY[4])(int i, int j) = {par_j, ver_j, left_j, right_j};

/* 白棋黑棋评分权值
 * 最初的思想是将黑棋白棋相同棋形采用不同分
 * 不过最后还是成了相同的，这俩其实可以用一个数组 */
const int black_point[8] = {1, 10, 100, 2000, 10000, 100000};
const int white_point[8] = {1, 10, 100, 2000, 10000, 100000};

/* 各长度评分表 */
int score_table5_w[243] = {0};
int score_table6_w[729] = {0};
int score_table7_w[2187] = {0};
int score_table8_w[6561] = {0};
int score_table9_w[19683] = {0};
int score_table10_w[59049] = {0};
int score_table11_w[177147] = {0};
int score_table12_w[531441] = {0};
int score_table13_w[1594323] = {0};
int score_table14_w[4782969] = {0};
int score_table15_w[14348907] = {0};
int score_table5_b[243] = {0};
int score_table6_b[729] = {0};
int score_table7_b[2187] = {0};
int score_table8_b[6561] = {0};
int score_table9_b[19683] = {0};
int score_table10_b[59049] = {0};
int score_table11_b[177147] = {0};
int score_table12_b[531441] = {0};
int score_table13_b[1594323] = {0};
int score_table14_b[4782969] = {0};
int score_table15_b[14348907] = {0};

/* 存所有长度的哈希表的指针*/
int *score_table[2][16] = {0, 0, 0, 0, 0, score_table5_b, score_table6_b, score_table7_b, score_table8_b, score_table9_b, score_table10_b, score_table11_b, score_table12_b, score_table13_b, score_table14_b, score_table15_b,
                           0, 0, 0, 0, 0, score_table5_w, score_table6_w, score_table7_w, score_table8_w, score_table9_w, score_table10_w, score_table11_w, score_table12_w, score_table13_w, score_table14_w, score_table15_w};

/*初始化评分哈希表*/
void init_hash() {
    int len; 
    for (len = 6; len <= 15; len++) {
        init_score_table(len);
    }
    init_score_table_5();
}

/*初始化某一长度评分哈希表*/
void init_score_table(int len) {
    int *board = (int*)malloc(sizeof(int) * len);
    int i, j, temp;
    for (i = 0; i < pow(3, len); i++) {
        temp = i;
        for (j = 0; j < len; j++) {
            board[len - 1 - j] = temp % 3;
            temp /= 3;
        }
        init_evaluate(board, i, len);
    }
    free(board);
    board = NULL;
}

/*获取当前行棋子数目*/
int get_line_num(int i, int j, int index) {
    int num;
    switch (index) {
    case 0:
    case 1:
        return 15;
    case 2:
        if (i + j < LENGTH)
            return i + j + 1;
        else    
            return 29 - i - j;
    case 3:
        if (i > j)
            return 15 + j - i;
        else 
            return 15 + i - j;
    }
}

/*获得下棋后节点p四个方向的shape以及该走法的评价(己方收益+对方收益)*/
void get_point_score(Tree p) {
    int i = p->i;
    int j = p->j;
    int x, y, num;
    int score, index, oppo;
    int b_player = bool_player ^ 1;

    score = 0;
    for (index = 0; index < 4; index++) {
        x = (transX[index])(i, j);
        y = (transY[index])(i, j);
        p->shape[index] = board_shape[index][x] + player * base[y];
        if ((num = get_line_num(i, j, index)) < 5)
            continue;
        oppo = board_shape[index][x] + (player ^ 0b11) * base[y];
        score += (score_table[bool_player][num][p->shape[index]] - score_table[bool_player][num][board_shape[index][x]]);
        score += (score_table[b_player][num][oppo] - score_table[b_player][num][board_shape[index][x]]);
    }
    p->point_score = score;
}

/*获得下棋后节点p四个方向的shape以及该走法的评价(己方收益+对方损失，配合算杀使用)*/
// void get_point_score_kill(Tree p) {
//     int i = p->i;
//     int j = p->j;
//     int x, y, num;
//     int score, index, oppo;
//     int b_player = bool_player ^ 1;

//     score = 0;
//     for (index = 0; index < 4; index++) {
//         x = (transX[index])(i, j);
//         y = (transY[index])(i, j);
//         p->shape[index] = board_shape[index][x] + player * base[y];
//         if ((num = get_line_num(i, j, index)) < 5)
//             continue;
//         score += (score_table[bool_player][num][p->shape[index]] - score_table[bool_player][num][board_shape[index][x]]);
//         score -= (score_table[b_player][num][p->shape[index]] - score_table[b_player][num][board_shape[index][x]]);
//     }
//     p->point_score = score;
// }

/*坐标变换*/
int par_i(int i, int j) {
    return PAR_I(i, j);
}

int par_j(int i, int j) {
    return PAR_J(i, j);
}

int ver_i(int i, int j) {
    return VER_I(i, j);
}

int ver_j(int i, int j) {
    return VER_J(i, j);
}

int left_i(int i, int j) {
    return LEFT_I(i, j);
}

int left_j(int i, int j) {
    return LEFT_J(i, j);
}

int right_i(int i, int j) {
    return RIGHT_I(i, j);
}

int right_j(int i, int j) {
    return RIGHT_J(i, j);
}

/*以下都是5长度和6长度的暴力模板匹配,几百行case非常丑陋，但是在评估棋局时准确方便
 *毕竟写case总比分情况分析一大串东西来的简单
 *选用6长度是为了方便判断活二活三活四棋形*/

void init_score_table_5() {
    int i;
    for (i = 0; i < 243; i++) {
        switch (i) {
        case 0:
            break;
        case one_2w:
        case one_3w:
        case one_4w:
        case one_5w:
        case one_6w:
            score_table5_w[i] = 1; 
            break;
        case one_2b:
        case one_3b:
        case one_4b:
        case one_5b:
        case one_6b:
            score_table5_b[i] = 1; 
            break;
        case 216:
        case 180:
        case 168:
        case 164:
        case 72:
        case 60:
        case 56:
        case 24:
        case 20:
        case 8:
            score_table5_w[i] = 10; 
            break;
        case 108:
        case 90:
        case 84:
        case 82:
        case 36:
        case 30:
        case 28:
        case 12:
        case 10:
        case 4:
            score_table5_b[i] = 10; 
            break;
        case 234:
        case 222:
        case 218:
        case 186:
        case 182:
        case 170:
        case 78:
        case 74:
        case 62:
        case 26:
            score_table5_w[i] = 100; 
            break;
        case 117:
        case 111:
        case 109:
        case 93:
        case 91:
        case 85:
        case 39:
        case 37:
        case 31:
        case 13:
            score_table5_b[i] = 100; 
            break;
        case 240:
        case 236:
        case 224:
        case 188:
        case 80:
            score_table5_w[i] = 2000; 
            break;
        case 120:
        case 118:
        case 112:
        case 94:
        case 40:
            score_table5_b[i] = 2000; 
            break;
        case 242:
            score_table5_w[i] = 100000; 
            break;
        case 121:
            score_table5_b[i] = 100000; 
            break;
        }
    }
}

int init_evaluate(int* board, int i, int len) {
    int index, now_i;    
    int value;
    int black_evaluate[10] = {0};
    int white_evaluate[10] = {0};
    int score_w = 0;
    int score_b = 0;

    for (now_i = 0; now_i < len - 5; now_i++) {
        value = 0;
        for (index = 0; index < 6; index++) 
            value += board[now_i + index] * base[index];
        switch (value) {
            case 0:
                break;
            case one_1w:
            case one_2w:
            case one_3w:
            case one_4w:
            case one_bw:
            case one_cw:
            case one_dw:
                if (now_i + index > len - 1)
                    white_evaluate[0]++;
                break;
            case one_7w:
            case one_8w:
            case one_9w:
            case one_aw:
            case one_6w:
                white_evaluate[0]++;
                break;
            case one_5w:
            case one_ew:
                white_evaluate[0]++;
                now_i++;
                break;
            case two_1w:
            case two_2w:
            case two_3w:
            case two_4w:
            case two_jw:
            case two_kw:
            case two_lw:
            case two_mw:
            case two_nw:
            case two_ow:
            case two_pw:
            case two_qw:
            case two_rw:
            case two_sw:
                if (now_i + index > len - 1)
                    white_evaluate[1]++;
                break;
            case two_5w:
            case two_6w:
            case two_7w:
            case two_8w:
            case two_9w:
            case two_aw:
            case two_bw:
            case two_cw:
            case two_dw:
            case two_ew:
            case two_fw:
            case two_gw:
            case two_hw:
            case two_iw:
                white_evaluate[1]++;
                break;
            case huo_two_1w:
            case huo_two_2w:
            case huo_two_4w:
                if (now_i + index > len - 1 || board[now_i + index] == 1)
                    white_evaluate[2]++;
                break;
            case huo_two_3w:
            case huo_two_5w:
            case huo_two_6w:
                white_evaluate[2]++;
                break;
            case three_1w:
            case three_2w:
            case three_3w:
            case three_4w:
            case three_5w:
            case three_6w:
            case three_nw:
            case three_ow:
            case three_pw:
            case three_qw:
            case three_rw:
            case three_sw:
            case three_tw:
            case three_uw:
            case three_vw:
            case three_ww:
                if (now_i + index > len - 1)
                    white_evaluate[2]++;
                break;
            case three_7w:
            case three_8w:
            case three_9w:
            case three_aw:
            case three_bw:
            case three_cw:
            case three_dw:
            case three_ew:
            case three_fw:
            case three_gw:
            case three_hw:
            case three_iw:
            case three_jw:
            case three_kw:
            case three_lw:
            case three_mw:
                white_evaluate[2]++;
                break;
            case huo_three_1w:
                if (now_i + index > len - 1 || board[now_i + index] == 1)
                    white_evaluate[3]++;
                break;
            case huo_three_2w:
            case huo_three_3w:
            case huo_three_4w:
                white_evaluate[3]++;
                break;
            case four_1w:
            case four_2w:
            case four_3w:
            case four_4w:
            case four_dw:
            case four_ew:
            case four_fw:
            case four_gw:
                if (now_i + index > len - 1)
                    white_evaluate[3]++;
                break;
            case four_5w:
            case four_6w:
            case four_7w:
            case four_8w:
            case four_9w:
            case four_aw:
            case four_bw:
            case four_cw:
                white_evaluate[3]++;
                break;
            case huo_four_w:
                white_evaluate[4]++;
                break;
            case five_w1:
            case five_w2:
                if (now_i + index > len - 1)
                    white_evaluate[5]++;
                break;
            case five_w3:
            case five_w4:
            case five_w5:
                white_evaluate[5]++;
                break;
            case one_1b:
            case one_2b:
            case one_3b:
            case one_4b:
            case one_bb:
            case one_cb:
            case one_db:
                if (now_i + index > len - 1)
                    black_evaluate[0]++;
                break;
            case one_7b:
            case one_8b:
            case one_9b:
            case one_ab:
            case one_6b:
                black_evaluate[0]++;
                break;
            case one_5b:
            case one_eb:
                black_evaluate[0]++;
                now_i++;
                break;

            case two_1b:
            case two_2b:
            case two_3b:
            case two_4b:
            case two_jb:
            case two_kb:
            case two_lb:
            case two_mb:
            case two_nb:
            case two_ob:
            case two_pb:
            case two_qb:
            case two_rb:
            case two_sb:
                if (now_i + index > len - 1)
                    black_evaluate[1]++;
                break;
            case two_5b:
            case two_6b:
            case two_7b:
            case two_8b:
            case two_9b:
            case two_ab:
            case two_bb:
            case two_cb:
            case two_db:
            case two_eb:
            case two_fb:
            case two_gb:
            case two_hb:
            case two_ib:
                black_evaluate[1]++;
                break;
            case huo_two_1b:
            case huo_two_2b:
            case huo_two_4b:
                if (now_i + index > len - 1 || board[now_i + index] == 1)
                    black_evaluate[2]++;
                break;
            case huo_two_3b:
            case huo_two_5b:
            case huo_two_6b:
                black_evaluate[2]++;
                break;
            case three_1b:
            case three_2b:
            case three_3b:
            case three_4b:
            case three_5b:
            case three_6b:
            case three_nb:
            case three_ob:
            case three_pb:
            case three_qb:
            case three_rb:
            case three_sb:
            case three_tb:
            case three_ub:
            case three_vb:
            case three_wb:
                if (now_i + index > len - 1)
                    black_evaluate[2]++;
                break;
            case three_7b:
            case three_8b:
            case three_9b:
            case three_ab:
            case three_bb:
            case three_cb:
            case three_db:
            case three_eb:
            case three_fb:
            case three_gb:
            case three_hb:
            case three_ib:
            case three_jb:
            case three_kb:
            case three_lb:
            case three_mb:
                black_evaluate[2]++;
                break;
            case huo_three_1b:
                if (now_i + index > len - 1 || board[now_i + index] == 1)
                    black_evaluate[3]++;
                break;
            case huo_three_2b:
            case huo_three_3b:
            case huo_three_4b:
                black_evaluate[3]++;
                break;
            case four_1b:
            case four_2b:
            case four_3b:
            case four_4b:
            case four_db:
            case four_eb:
            case four_fb:
            case four_gb:
                if (now_i + index > len - 1)
                    black_evaluate[3]++;
                break;
            case four_5b:
            case four_6b:
            case four_7b:
            case four_8b:
            case four_9b:
            case four_ab:
            case four_bb:
            case four_cb:
                black_evaluate[3]++;
                break;
            case huo_four_b:
                black_evaluate[4]++;
                break;
            case five_b1:
            case five_b2:
                if (now_i + index > len - 1)
                    black_evaluate[5]++;
                break;
            case five_b3:
            case five_b4:
                black_evaluate[5]++;
                break;
            case five_b5:
                black_evaluate[6]++;
                break;
        }
    }
    for (int j = 0; j < 6; j++) {
        score_b += black_evaluate[j]*black_point[j];
        score_w += white_evaluate[j]*white_point[j];
    }
    score_table[0][len][i] += score_b;
    score_table[1][len][i] += score_w;
    if (black_evaluate[6])
        score_table[0][len][i] = -2000000;
}

