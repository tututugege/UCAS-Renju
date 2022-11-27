#include "../include/history.h"

Line history_table[TABLE_SIZE] = {0};

void clear_history_table() {
    for (int i = 0; i < TABLE_SIZE; i++) 
        history_table[i] = 0;
}

int get_history(Chess_point position){
    return history_table[position];
}

void insert_history(int position, int depth) {
    history_table[position] += (1 << depth);
} 

void quick_sort(Tree move_set, int left, int right) {
    int i, j, std;

     if (left > right)
        return;
   
    i = left;
    j = right;
    std = move_set[left].history_score;

    while (i != j) {
        while (move_set[j].history_score <= std && j > i)
            j--;
        while (move_set[i].history_score >= std && j > i)
            i++;
        
        if (j > i) 
            exchange(move_set, i, j);
    }
    exchange(move_set, i, left);

    quick_sort(move_set, left, i - 1);
    quick_sort(move_set, i + 1, right);
}

void exchange(Tree move_set, int i, int j) {
    Node temp;
    temp = move_set[i];
    move_set[i] = move_set[j];
    move_set[j] = temp;
}
