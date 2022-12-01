#include "../include/zobrist.h"
//对置换表长度取余的对应的下标
int num_zobrist = 0;

void init_rand() {
    int i, j, k, l;
    srand((unsigned int)time(NULL));

	for (i = 0; i < LENGTH; i++) {
		for (j = 0; j < LENGTH; j++) {
			for (k = 0; k < 2; k++) {
				for(l = 0; l < 64; l++) {
					zobrist[i][j][k] = zobrist[i][j][k] << 1;
					zobrist[i][j][k] += rand() & 0b1;
				}
			}
		}
	}
}

//初始化一个置换表
table* init_table() {
    int i;
    table* p_table = (table*)malloc(sizeof(table));
    p_table->data = (item*)malloc(sizeof(item) * HASHSIZE);
    for (i = 0; i < HASHSIZE; i++) {
        p_table->data[i].key = NULLKEY;
    }
    return p_table;
}

void TT_insert(int point, int depth) {
    int index = ZOBRIST(now_key);
    (tt->data)[index].depth = (char)depth; 
    (tt->data)[index].key = now_key; 
    (tt->data)[index].point = point; 
}

int TT_search(int depth) {
    int index = ZOBRIST(now_key);
    if (((tt->data)[index]).key != NULLKEY && ((tt->data)[index]).depth <= depth && (tt->data[index].key == now_key)) {
        num_zobrist++;
        return tt->data[index].point;
    }
    else return NULLKEY;
} 

void TT_free() {
    free(tt->data);
    tt->data = NULL;
    free(tt);
    tt = NULL;
}