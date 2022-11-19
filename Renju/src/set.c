#include "../include/set.h"
//棋盘基本操作
//打印棋盘 下棋 判断输赢

void AI_operation();//从这开始进入AI
void set_board();   //打印当前局势的棋盘
void set();          //下棋 
void AI_set();       //AI下棋
void print_piece(int i, int j);
void get_input();

extern int player_i;
extern int player_j;

/*逐行打印棋盘*/
void set_board() { 
    int i, j;
    //自上而下打印，因此i从LENGTH-1开始递减
    //每行第一个符号分三种边框情况和四种棋子情况特殊处理
    for (i = LENGTH - 1; i >= 0; i--) {    
        printf("%2d", i + 1);               //打印行标
        if (fir[i][0] == 0) {
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
            if (fir[i][j] == 0) {
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
        if (fir[i][LENGTH - 1] == 0) {
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

void print_piece(int i, int j) {
    if (fir[i][j] == BLACK) printf("●");
    else if (fir[i][j] == WHITE) printf("○");
    else if (fir[i][j] == LASTBLACK) {
        printf("▲");
        fir[i][j] = BLACK;
    }
    else {
        printf("○");
        fir[i][0] = WHITE;
    }
}

void set() {
    get_input();
    fir[player_i][player_j] = player + 2;
    now_key = now_key ^ zobrist[player][player_j][player - 1];

    if (player == WHITE) printf("白棋下在了%c%d位置\n", player_j + 'a', player_i + 1);
    else printf("黑棋落子在%c%d\n", player_j + 'a', player_i + 1);
}

void AI_set() {
    AI_operation();
    fir[AI_i][AI_j] = player + 2;
    now_key ^= zobrist[AI_i][AI_j][player - 1];

    if (player == WHITE) printf("白棋(AI)下在了%c%d位置\n", AI_j + 'a', AI_i + 1);
    else printf("黑棋(AI)落子在%c%d\n", AI_j + 'a', AI_i + 1);
}

int lineLength(int i, int j, int dx, int dy) {
    int k, l;
    int num = 1;
    int last_player = player % 2 + 1;
    for (k = i + dx, l = j + dy; within_range(k) && within_range(l) && fir[k][l] == last_player; k += dx, l += dy, num++);    
    for (k = i - dx, l = j - dy; within_range(k) && within_range(l) && fir[k][l] == last_player; k -= dx, l -= dy, num++);    

    return num;
} //最长列棋子数目

//根据下棋位置判断是否已经结束
int win(int i, int j) {
    if (lineLength(i, j, 1, 0) >= 5 || lineLength(i, j, 1, 1) >= 5 || lineLength(i, j, 1, -1) >= 5 || lineLength(i, j, 0, 1) >= 5) {
        if (player == BLACK)
            return WHITE;
        else
            return BLACK;
    }
    else return 0;
}

//判断i是否在0-14闭区间内
int within_range(int i) {
    if (i >= 0 && i < LENGTH) return 1;
    else return 0; 
}

//1变2 2变1
void change_player() {
    player ^= 0b11;
}

int is_full() {
    int i, j;

    for (i = 0; i < LENGTH; i++) {
        for (j = 0; j < LENGTH; j++) {
            if (fir[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

void get_input() {
    int right_input = 0;
    char temp_j;
    do { //若输入位置在棋盘内并且该位置无棋子则right_input为1 否则为0一直循环
        //直到输入字母和数字在范围内才能出循环
        while (getchar() != '\n')
            ;   //吸收输入流中多余内容 下同
        scanf("\n%c%d", &temp_j, &player_i);  //\n吸收残留的回车符 下同
        while (((temp_j < 'a' || temp_j > 'o') && (temp_j <'A' || temp_j >'O')) || (player_i <= 0 || player_i > LENGTH)) { 
            printf("输入字符非法，请重新输入\n");
            while (getchar() != '\n')
                ;  
            scanf("\n%c%d", &temp_j, &player_i);  
        }
        player_j = (temp_j >= 'a' && temp_j <='o') ? temp_j - 'a' : temp_j - 'A';
        player_i--;
        //判断输入位置是否已经有棋子
        if (fir[player_i][player_j] != 0) {
            printf("输入位置有棋子，请重新输入 \n");
        } else if (player == WHITE || !forbid(player_i, player_j)) 
            right_input = 1;
    } while (right_input != 1);
}