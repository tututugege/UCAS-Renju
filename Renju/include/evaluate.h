#ifndef _EVALUATE_H_
#define _EVALUATE_H_

#include "../include/common.h"

#define one_1w 18//00200
#define one_2w 6//00020
#define one_3w 2//00002

#define two_1w 60  //02020
#define two_2w 8 //00022           10
#define two_3w 24 //00220          50
#define two_4w 56    //02002
#define two_5w 20    //00202

#define three_1w 78 //02220       100
#define three_2w 26 //00222		  50
#define three_3w 74	//02202
#define three_4w 62	//02022
#define three_5w 170 //10011
#define three_6w 182 //10101

#define four_1w 224 //22022     50000
#define four_2w 80 //02222      10000
#define four_3w 188 //20222     50000

#define one_1b 9//00100
#define one_2b 3//00010
#define one_3b 1//00001

#define two_1b 30  //01010
#define two_2b 4 //00011           10
#define two_3b 12 //00110          30
#define two_4b 28  //01001
#define two_5b 10  //00101

#define three_1b 39 //01110       100
#define three_2b 13 //00111		  50
#define three_3b 37	 //01101
#define three_4b 31	 //01011
#define three_5b 85  //10011
#define three_6b 91 //10101

#define four_1b 112 //11011	   50000
#define four_2b 40 //01111      10000
#define four_3b 94 //10111     50000

#define five_b 121   //11111   1000000
#define five_w 242   //22222   1000000 

#define NUM_POINT 19683 
#define NULLKEY 33554432

void init_point_table();
int init_evaluate(int* board);
int point_evaluate(int i, int j, int depth);					
int move_evaluate(int i, int j, int board_score, int depth);					
int init_point_evaluate(int i, int j, int dx, int dy);
int TT_search(int depth);
void TT_insert(int point, int depth);

void set_point(int i, int j);
void reset_point(int i, int j);

#endif