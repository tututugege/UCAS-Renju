#include "../include/common.h"

#define TABLE_SIZE 256

#define GET_HIS(position) history_table[position]
#define INS_HIS(position) history_table[position] += (1 << g_depth)

extern Line history_table[];

void clear_history_table();
void quick_sort(Tree move_set, int left, int right);
void exchange(Tree move_set, int i, int j);