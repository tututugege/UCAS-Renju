#include "../include/common.h"

#define TABLE_SIZE 256

void clear_history_table();
int get_history(Chess_point position);
void insert_history(int position, int depth);
void quick_sort(Tree move_set, int left, int right);
void exchange(Tree move_set, int i, int j);