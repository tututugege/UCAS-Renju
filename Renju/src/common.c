#include "../include/common.h"

//判断i是否在0-14闭区间内
int within_range(int i) {
    if (i >= 0 && i < LENGTH) 
        return 1;
    else 
        return 0; 
}

int in_range(int i, int j) {
    if (within_range(i) && within_range (j))
        return 1;
    else
        return 0;
}

int is_full() {
    int i, j;

    for (i = 0; i < LENGTH; i++) {
        for (j = 0; j < LENGTH; j++) {
            if (!renju[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

