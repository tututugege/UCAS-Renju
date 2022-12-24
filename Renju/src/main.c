/********************************************************************************
* @File name: tututuRenju
* @Author: Tututu(侯汝垚)
* @Date: 2022-12-24
* @Compiler: MinGW gcc
* @Description: UCAS c语言程序设计大作业，五子棋带禁手人人对战、人机对战和机机对战
				AI主干部分为PVS主体变例搜索的alpha-beta搜索实现约8层搜索

********************************************************************************/
#include "./main.h"

char game_mode; //游戏模式
int main() {
	system("color F0"); //设置颜色为白底黑线,美观好看
	printf("请输入游戏模式：人人对战(0) 人机对战(1) 机机对战(2)：");
	game_mode = getchar();
	while ( !isdigit(game_mode) || (game_mode -= '0') > 2) {
		printf("输入错误，请重新输入");
		while (getchar() != '\n')
			;	//清空输入流
		game_mode = getchar();
	}

	/*三种游戏模式*/
	if (game_mode == 0)
		PvsP();
	else if (game_mode == 1)
		PvsAI();
	else 
		AIvsAI();

	system("pause");
}