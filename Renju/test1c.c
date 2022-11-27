#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int data1[100] = {0};
void quick_sort(int a[], int , int);
void exchange(int i, int j);
int main(int argc, char const *argv[]) {
    int i;

    srand((unsigned int)time(NULL));
    for (i = 0; i < 100; i++) {
        data1[i] = rand() % 10000;
    }
    quick_sort(data1, 0, 99);
    for (i = 0; i < 100; i++) {
        printf("%d\n", data1[i]);
    }
    system("pause");

    return 0;
}

void quick_sort(int a[], int left, int right) {
    int i, j, temp, tmp;

    if (left >= right)
        return; 

    i = left;
    j = right;
    tmp = a[left];

    while (i != j) {
        while (a[j] <= tmp && j > i)
            j--;
        while (a[i] >= tmp && j > i)
            i++;
        
        if (j > i) {
           exchange(i, j);
        }
    }
    exchange(left, i);
    quick_sort(a, left, i - 1);
    quick_sort(a, i + 1, right);
}

void exchange(int i, int j) {
    int temp;
    temp = data1[i];
    data1[i] = data1[j];
    data1[j] = temp;
}