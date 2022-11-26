#include "../include/zobrist.h"
//对置换表长度取余的对应的下标
int zobrist_hash(unsigned long long key) {
    return (int)(key & (HASHSIZE - 1));
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
    int index = zobrist_hash(now_key);
    (tt->data)[index].depth = (char)depth; 
    (tt->data)[index].key = now_key; 
    (tt->data)[index].point = point; 
}

int TT_search(int depth) {
    int index = zobrist_hash(now_key);
    if (((tt->data)[index]).key != NULLKEY && ((tt->data)[index]).depth <= depth && (tt->data[index].key == now_key))
        return tt->data[index].point;
    else return NULLKEY;
} 