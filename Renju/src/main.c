#include "../include/main.h"

int main() {
	char game_mode;

	printf("请输入游戏模式：人人对战(0) 人机对战(1) 机机对战(2)：");
	game_mode = getchar();
	while ( !isdigit(game_mode) || (game_mode -= '0') > 2) {
		printf("输入错误，请重新输入");
		while (getchar() != '\n')
			;	
		game_mode = getchar();
	}

	if (game_mode == 0)
		PvsP();
	else if (game_mode == 1)
		PvsAI();
	else 
		AIvsAI();

	system("pause");
}