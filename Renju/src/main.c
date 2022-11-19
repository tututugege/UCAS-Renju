#include "../include/set.h"

int player = 1;
int game_mode;  //游戏模式
int player_color;      //玩家选择的棋色
int fir[LENGTH][LENGTH] = {0};
unsigned long long now_key = 0;
unsigned long long zobrist[LENGTH][LENGTH][2] = {0};


int main() {
	int* position = (int*)malloc(sizeof(int)*2);	 //记录下棋位置			
	int result, step, i, j;
	int time_b, time_w;
	clock_t start_t, end_t;

	get_rand();
	init_point_table();

	step = time_b = time_w = 0;
	AI_i = AI_j = 7;
	fir[7][7] = player + 2;
	position[0] = AI_i;
	position[1] = AI_j;

	change_player();
	set_board();

	while (position && (result = win(position[0], position[1])) == 0) {
		if (player == 1) {
			start_t = clock();
			position = AI_set(position);
			if(position)
				now_key ^= zobrist[position[0]][position[1]][BLACK - 1];
			else
				break;
			end_t = clock();
			time_b += end_t - start_t;
			step++;
		} else {
			start_t = clock();
			position = AI_set(position);
			if(position)
				now_key ^= zobrist[position[0]][position[1]][WHITE - 1];
			else 
				break;
			end_t = clock();
			time_w += end_t - start_t;
			step++;
		}
	}
	set_board();
	if (result == BLACK) {
		printf("黑棋赢\n");
	}
	else if(result == WHITE) {
		printf("白棋赢\n");
	}
	printf("黑棋平均搜索时间%d\n白棋平均搜索时间%d\n总步数%d\n", time_b/(step/2), time_w/(step/2), step);
	free(position);
	system("pause");
}