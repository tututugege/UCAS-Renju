#include "../include/set.h"

int renju[LENGTH][LENGTH] = {0};

//棋盘基本操作
//打印棋盘 下棋 判断输赢

/*逐行打印棋盘*/
void set_board() { 
    int i, j;
    //自上而下打印，因此i从LENGTH-1开始递减
    //每行第一个符号分三种边框情况和四种棋子情况特殊处理
    for (i = LENGTH - 1; i >= 0; i--) {    
        printf("%2d", i + 1);               //打印行标
        if (renju[i][0] == EMPTY) {
            if (i == LENGTH - 1) 
                printf("┏");
            else if (i == 0) 
                printf("┗");
            else 
                printf("┣");
        } else 
            print_piece(i, 0);
       //每行第二个符号到倒数第二个符号可以批量处理
        //同样分为三种边框情况和四种棋子情况
        for (j = 1; j < LENGTH - 1; j++){   
            if (renju[i][j] == EMPTY) {
                if (i == LENGTH - 1) 
                    printf("┳");
                else if (i == 0) 
                    printf("┻");
                else 
                    printf("╋");
            } else         
                print_piece(i, j);
        }
        //每行最后一个符号分三种边框情况和四种棋子情况
        if (renju[i][LENGTH - 1] == EMPTY) {
            if (i == LENGTH - 1) 
                printf("┓");
            else if (i == 0) 
                printf("┛");
            else 
                printf("┫");
        } else 
            print_piece(i, LENGTH - 1);
            //每排最后一个
        printf("\n");
    }
    printf("   ");
    //打印列标
    for (i = 'A'; i < 'A' + LENGTH; i++) printf("%c ", i);
    printf("\n\n");
}

//根据情况打印棋子
void print_piece(int i, int j) {
    switch (renju[i][j]) {
    case BLACK:
        printf("●");
        break;
    case WHITE:
        printf("○");
        break;
    case LASTBLACK:
        printf("▲");
        renju[i][j] = BLACK;
        break;
    default:
        printf("△");
        renju[i][j] = WHITE;
        break;
    }
}

//人机对战时人的部分
void player_set() {
    Line buf[5];
    player_op();
    get_input();
    buf_bit_move(g_j, buf);
    g_score = move_evaluate(g_i, g_j, g_score, 0);
    printf("当前局面得分%d\n", g_score);
    reset_point(g_i, g_j);
    re_bit_move(g_j ,buf);
    new_set(g_i, g_j);
}

void AI_set() {
    AI_op();
    new_set(g_i, g_j);
}

void set() {
    get_input();
    renju[g_i][g_j] = player + 2;
}

int lineLength(int i, int j, int dx, int dy) {
    int k, l;
    int num = 1;
    int last_player = player % 2 + 1;
    for (k = i + dx, l = j + dy; in_range(k, l) && renju[k][l] == last_player; k += dx, l += dy, num++);    
    for (k = i - dx, l = j - dy; in_range(k, l) && renju[k][l] == last_player; k -= dx, l -= dy, num++);    

    return num;
} //最长列棋子数目

//根据下棋位置判断是否已经结束


void get_input() {
    int correct_input = 0;
    char temp_j;
    do { 
        while (getchar() != '\n')
            ;   //清空输入流
        scanf("\n%c%d", &temp_j, &g_i);  //\n吸收残留的回车符 下同

        while ((!within_range(temp_j - 'a') && !within_range(temp_j - 'A')) || !within_range(--g_i)) { 
            printf("输入字符非法，请重新输入\n");
            while (getchar() != '\n')
                ;  
            scanf("\n%c%d", &temp_j, &g_i);  
        }
        g_j = (temp_j > 'Z') ? temp_j - 'a' : temp_j - 'A';
        //判断输入位置是否已经有棋子
        if (renju[g_i][g_j]) 
            printf("输入位置有棋子，请重新输入 \n");
        else if (player == WHITE || !forbid(g_i, g_j)) 
            correct_input = 1;
        else 
            printf("落子位置为禁手\n");
    } while (correct_input != 1);
}

void new_set(int i, int j) {
    renju[i][j] = player + 2;
    now_key ^= zobrist[i][j][player - 1];
    cache_move_board();
    set_bit_board(i, j);
}

void judge_result(int result) {
	if (result == BLACK) 
		printf("黑棋赢\n");
	else if(result == WHITE) 
		printf("白棋赢\n");
	else	
		printf("棋盘满，平局\n");
}

int win(int i, int j) {
    if (lineLength(i, j, 1, 0) >= 5 || lineLength(i, j, 1, 1) >= 5 || lineLength(i, j, 1, -1) >= 5 || lineLength(i, j, 0, 1) >= 5) {
        if (player == BLACK)
            return WHITE;
        else
            return BLACK;
    }
    else return 0;
}