#include "../include/game_mode.h"
 
/* 三种游戏模式入口 */
 
int player = 1;         //1为黑2为白
int bool_player = 0;    //0为黑1为白
Tree g_move = NULL;     //指向上一步走法，更新走法时用
int g_i = 7;            
int g_j = 7;            //这俩存当前这步要走的位置

/*人人对战*/
void PvsP() {
    int result;

    system("cls");
    set_board();
    while (!(result = win(g_i, g_j)) && !is_full()) {
        printf("输入格式(h8)或(H8)\n");
        set();
        system("cls");
        set_board();
        print_pos();
        CHANGE_PLAYER;
    }
    judge_result(result);
}

void PvsAI() {
	int result;
	int time_AI = 0;
    char player_color;

    /*初始化AI相关的哈希表之类的东西*/
	init_hash();        //评分表
	init_bit_board();   //位棋盘
	init_move_table();  //走法表

    while (getchar() != '\n')
        ;	
    printf("选择玩家持方：黑棋先手(1)，白棋后手(2):");
    player_color = getchar();
	while ( !isdigit(player_color) || ((player_color -= '0') != 1 && player_color != 2)) {
		printf("输入错误，请重新输入");
		while (getchar() != '\n')
			;	
		player_color = getchar();
	}

    /*选白棋就在天元先下*/
    if (player_color == WHITE) {
        new_set(g_i, g_j);
        board_shape[0][7] = 2187; //2187即三进制数的10000000
        board_shape[1][7] = 2187; 
        board_shape[2][14] = 2187; 
        board_shape[3][14] = 2187; 
        set_board();
        print_pos();
        CHANGE_PLAYER;
    } else 
        set_board();

	while (!(result = win(g_i, g_j)) && !is_full()) {
		if (player == player_color) {
			player_set();
            system("cls");
        }
		else 
			AI_set();
        // system("cls");
        set_board();
        print_pos();
		CHANGE_PLAYER;
	}
	free(g_move);
	g_move = NULL;
	set_board();
    judge_result(result);
}

//机机对战用于测试AI
void AIvsAI() {
	int result, step;
	int time_b, time_w;
	clock_t start_t, end_t;

    step = time_b = time_w = 0;

    init_hash();
	init_bit_board();   //位棋盘
	init_move_table();  //走法表

    renju[g_i][g_j] = player + 2;
    bit_board[g_j] &= bit_set[g_i];
    set_bit_board(g_i, g_j);
    board_shape[0][7] = 2187; 
    board_shape[1][7] = 2187; 
    board_shape[2][14] = 2187; 
    board_shape[3][14] = 2187; 

	set_board();
    print_pos();
    CHANGE_PLAYER;

	while (!(result = win(g_i, g_j)) && !is_full()) {
        start_t = clock();
        AI_set();
        end_t = clock();
        CHANGE_PLAYER;
        set_board();
    }
	free(g_move);
	g_move = NULL;
	set_board();
}

void print_pos() {
    if (player == WHITE) 
        printf("白棋下在了%c%d位置\n", g_j + 'a', g_i + 1);
    else 
        printf("黑棋落子在%c%d\n", g_j + 'a', g_i + 1);
}

