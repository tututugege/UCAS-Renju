#include "../include/main.h"
#include "../include/common.h"

int player = 1;
int fir[LENGTH][LENGTH] = {0};
unsigned long long now_key = 0;
unsigned long long zobrist[LENGTH][LENGTH][2] = {0};

tree g_move = NULL;

int main() {
	int result, step, i, j;
	int time_b, time_w;
	clock_t start_t, end_t;

	init_rand();
	init_point_table();
	init_bit_board();
	init_move_table();

	step = time_b = time_w = 0;
	g_i = g_j = 7;
	fir[7][7] = player + 2;
	set_bit_board(7, 7); 
	now_key ^= zobrist[7][7][BLACK - 1];
	change_player();
	set_board();
	printf("%d",sizeof(Node));

	while (!(result = win(g_i, g_j)) && !is_full()) {
		if (player == BLACK) {
			start_t = clock();
			AI_set();
			now_key ^= zobrist[g_i][g_j][BLACK - 1];
			end_t = clock();
			time_b += end_t - start_t;
			step++;
		} else {
			start_t = clock();
			AI_set();
			now_key ^= zobrist[g_i][g_j][WHITE - 1];
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

