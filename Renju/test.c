#include <stdio.h>
#define LENGTH 15
#define MOVE_NUM 32768

unsigned long long move_table[MOVE_NUM];

void init_move_table() {
    int index1, index2, temp;
    unsigned long long val = 0;
    for (index1 = 0; index1 < MOVE_NUM; index1++) {
        temp = index1;
        val = 0;
        for (index2 = LENGTH - 1; index2 >= 0; index2--) {
            if (temp & 0x1) {
                val = val << 4;
                val += index2;
            }
            temp = temp >> 1;
        }
        move_table[index1] = val;
    }
}

int main(int argc, char const *argv[]) {
    init_move_table();
    for (int i = 0; i < 100; i++)
        printf("%d %x\n", i, move_table[i]);

    printf("%llx", move_table[32767]);
    system("pause");
    return 0;
}
