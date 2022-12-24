#ifndef _EVALUATE_H_
#define _EVALUATE_H_
/*
 *棋盘评估，核心思想是三进制哈希匹配，将一种棋形映射为一个三进制数作为索引
 *穷举所有能成五的六位三进制棋形(实在太多)
 */
#include "../include/common.h"

/* 棋形模板 */
#define one_1w 486 //200000
#define one_2w 162 //020000
#define one_3w 54  //002000
#define one_4w 18  //000200
#define one_5w 6   //000020
#define one_6w 2   //000002
#define one_7w 405 //120000
#define one_8w 297 //102000
#define one_9w 261 //100200
#define one_aw 249 //100020
#define one_bw 163 //020001
#define one_cw 55  //002001
#define one_dw 19  //000201
#define one_ew 7   //000021

#define two_1w 648  // 220000
#define two_2w 540  // 202000        
#define two_3w 504  // 200200          
#define two_4w 492  // 200020

#define two_5w 8    // 000022
#define two_6w 20   // 000202        
#define two_7w 56   // 002002          
#define two_8w 164  // 020002
#define two_9w 459  // 122000
#define two_aw 423  // 120200
#define two_bw 411  // 120020
#define two_cw 407  // 120002
#define two_dw 315  // 102200
#define two_ew 303  // 102020
#define two_fw 299  // 102002
#define two_gw 267  // 100220
#define two_hw 263  // 100202
#define two_iw 251  // 100022

#define two_jw 649  // 220001
#define two_kw 541  // 202001
#define two_lw 505  // 200201
#define two_mw 493  // 200021
#define two_nw 217  // 022001
#define two_ow 181  // 020201
#define two_pw 169  // 020021
#define two_qw 73   // 002201
#define two_rw 69  // 002021
#define two_sw 25   // 000221

#define huo_two_1w 216 //022000
#define huo_two_2w 180 //020200
#define huo_two_3w 168 //020020
#define huo_two_4w 72  //002200
#define huo_two_5w 24 //000220
#define huo_two_6w 60 //002020

#define three_1w 702 //222000       
#define three_2w 666 //220200		  
#define three_3w 654 //220020
#define three_4w 558 //202200
#define three_5w 546 //202020
#define three_6w 510 //200220

#define three_7w 26  //000222       
#define three_8w 74  //002202		  
#define three_9w 218 //022002
#define three_aw 62  //002022
#define three_bw 182 //020202
#define three_cw 170 //020022

#define three_dw 477 //122200
#define three_ew 465 //122020
#define three_fw 461 //122002
#define three_gw 429 //120220
#define three_hw 425 //120202
#define three_iw 413 //120022
#define three_jw 321 //102220
#define three_kw 317 //102202
#define three_lw 305 //102022
#define three_mw 269 //100222

#define three_nw 703 //222001
#define three_ow 667 //220201
#define three_pw 655 //220021
#define three_qw 559 //202201
#define three_rw 547 //202021
#define three_sw 511 //200221
#define three_tw 235 //022201
#define three_uw 223 //022021
#define three_vw 187 //020221
#define three_ww 79 //002221


#define huo_three_1w 234 //022200
#define huo_three_2w 222 //022020
#define huo_three_3w 78 //002220
#define huo_three_4w 186 //020220

#define four_1w 720 //222200     
#define four_2w 708 //222020      
#define four_3w 672 //220220    
#define four_4w 564 //202220 

#define four_5w 80 //002222     
#define four_6w 236 //022202      
#define four_7w 224 //022022    
#define four_8w 188 //020222  
#define four_9w 483 //122220    
#define four_aw 323 //102222    
#define four_bw 479 //122202    
#define four_cw 431 //120222    

#define four_dw 721 //222201    
#define four_ew 241 //022221    
#define four_fw 709 //222021    
#define four_gw 565 //202221    

#define huo_four_w 240 //022220

#define five_w1 726 //222220
#define five_w2 727 //222221
#define five_w3 242 //022222
#define five_w4 485 //122222
#define five_w5 728 //222222

#define one_1b 243 
#define one_2b 81 
#define one_3b 27 
#define one_4b 9
#define one_5b 3
#define one_6b 1
#define one_7b 567 
#define one_8b 513 
#define one_9b 495
#define one_ab 489 
#define one_bb 83
#define one_cb 29
#define one_db 11
#define one_eb 5

#define two_1b 324  
#define two_2b 270            
#define two_3b 252           
#define two_4b 246  

#define two_5b 4 
#define two_6b 10  
#define two_7b 28  
#define two_8b 82  
#define two_9b 594 
#define two_ab 576 
#define two_bb 570
#define two_cb 568 
#define two_db 522
#define two_eb 516 
#define two_fb 514 
#define two_gb 498
#define two_hb 496
#define two_ib 490

#define two_jb 326 
#define two_kb 272 
#define two_lb 254 
#define two_mb 248 
#define two_nb 110 
#define two_ob 92 
#define two_pb 86 
#define two_qb 38
#define two_rb 32 
#define two_sb 14

#define huo_two_1b 108
#define huo_two_2b 90 
#define huo_two_3b 84 
#define huo_two_4b 36 
#define huo_two_5b 12 
#define huo_two_6b 30

#define three_1b 351
#define three_2b 333 
#define three_3b 327	
#define three_4b 279
#define three_5b 273 
#define three_6b 255
#define three_7b 13
#define three_8b 37
#define three_9b 109
#define three_ab 31
#define three_bb 91
#define three_cb 85 

#define three_db 603 
#define three_eb 597
#define three_fb 595
#define three_gb 579
#define three_hb 577
#define three_ib 571
#define three_jb 525
#define three_kb 523
#define three_lb 517
#define three_mb 499

#define three_nb 353
#define three_ob 335
#define three_pb 329
#define three_qb 281
#define three_rb 275
#define three_sb 257
#define three_tb 119
#define three_ub 113
#define three_vb 95
#define three_wb 41

#define huo_three_1b 117
#define huo_three_2b 111 
#define huo_three_3b 39
#define huo_three_4b 93 

#define four_1b 360 
#define four_2b 354 
#define four_3b 336 
#define four_4b 282 

#define four_5b 40
#define four_6b 118
#define four_7b 112
#define four_8b 94
#define four_9b 606
#define four_ab 526
#define four_bb 604
#define four_cb 580

#define four_db 362
#define four_eb 122
#define four_fb 356
#define four_gb 284

#define huo_four_b 120

#define five_b1 363   
#define five_b2 365   
#define five_b3 121 
#define five_b4 607   
#define five_b5 364   

/* 函数声明 */
void init_hash();                               //初始化所有score_table
void init_score_table(int);                     //初始化6-15长度某一长度的score_table
void init_score_table_5();                      //初始化长度为5的score_table
int init_evaluate(int* board, int i, int len);  //分析某一棋形
int get_line_num(int, int, int);                //获得某行某方向长度
void get_point_score(Tree p);                   //获得某点四个方向上的棋形和位置价值
// void get_point_score_kill(Tree p);

/* 全局变量 */
extern int *score_table[][16];              //黑白两色的棋盘评分表头指针数组,0-5空置
extern int (*transX[4])(int i, int j);      //四种坐标变换的函数指针
extern int (*transY[4])(int i, int j);
#endif