#include "../include/set.h"

extern int player_i;
extern int player_j;
extern int AI_i;
extern int AI_j;
int player_i = 0;
int player_j = 0;
int player = 1;
int fir[LENGTH][LENGTH] = {0};
unsigned long long now_key = 0;
unsigned long long zobrist[LENGTH][LENGTH][2] = {0};
line bit_board[LENGTH];
line bit_move_board[LENGTH] = {0};
line g_last_buf[5] = {0};
tree g_move = NULL;

//为了追求速度 牺牲空间预先缓存大量内容
void init_rand();
void init_point_table();
void init_move_table();
void init_bit_board();
void set_bit_board(int i, int j);

void set_board(); //打印当前局势的棋盘
void set();      //下棋 
void AI_set();    //AI下棋

int main() {
	int result, step, i, j;
	int time_b, time_w;
	clock_t start_t, end_t;
	g_move = (tree)malloc(sizeof(Node) * 2);
	g_move->position = 119;
	(g_move + 1)->position = NULLPOSITION;

	init_rand();
	init_point_table();
	init_bit_board();
	init_move_table();

	step = time_b = time_w = 0;
	AI_i = AI_j = 7;
	fir[7][7] = player + 2;
	set_bit_board(7, 7); 
	now_key ^= zobrist[7][7][BLACK - 1];
	change_player();
	set_board();
	printf("%d",sizeof(Node));

	while (!(result = win(AI_i, AI_j)) && !is_full()) {
		if (player == BLACK) {
			start_t = clock();
			AI_set();
			now_key ^= zobrist[AI_i][AI_j][BLACK - 1];
			end_t = clock();
			time_b += end_t - start_t;
			step++;
		} else {
			start_t = clock();
			AI_set();
			now_key ^= zobrist[AI_i][AI_j][WHITE - 1];
			end_t = clock();
			time_w += end_t - start_t;
			step++;
		}
		change_player();
		set_board();
	}
	free(g_move);
	g_move = NULL;
	set_board();
	if (result == BLACK) 
		printf("黑棋赢\n");
	else if(result == WHITE) 
		printf("白棋赢\n");
	else	
		printf("棋盘满，平局\n");
	printf("黑棋平均搜索时间%d\n白棋平均搜索时间%d\n总步数%d\n", time_b/(step/2), time_w/(step/2), step);
	system("pause");
}

void init_rand() {
    int i, j, k;
    srand((unsigned int)time(NULL));

	for (k = 0; k < 2; k++) {
		for (i = 0; i < LENGTH; i++) {
			for (j = 0; j < LENGTH; j++) {
				for(k = 0; k < 63; k++) {
					zobrist[i][j][0] += rand() & 0b1;
					zobrist[i][j][0] = zobrist[i][j][0] << 1;
				}
				zobrist[i][j][0] += rand() & 0b1;
			}
		}
	}
}

void init_bit_board() {
	for (int i = 0; i < LENGTH; i++)
		bit_board[i] = ~0;
}