#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void quick_sort(int a[], int , int);
void shell_sort(int a[], int size);

int main(int argc, char const *argv[]) {
    clock_t t1, t2, t3;
    int tt1, tt2;
    int i;
    int data1[100];
    int data2[100];
    tt1 = tt2 = 0;

    srand((unsigned int)time(NULL));
    for (int j = 0; j < 1000000; j++) {
        for (i = 0; i < 100; i++) {
            data1[i] = data2[i] = rand() % 10000;
        }
        t1 = clock();
        shell_sort(data1, 100);
        t2 = clock();
        quick_sort(data2, 0, 99);
        t3 = clock();
        // for (i = 0; i < 100; i++) {
        //     printf("%d %d\n", data1[i], data2[i]);
        // }
        // system("pause");

        tt1 += t2 - t1;
        tt2 += t3 - t2;
    }

    printf("快排：%d 希尔：%d", tt2, tt1);
    system("pause");
    return 0;
}

void quick_sort(int a[], int left, int right) {
    int i, j, temp, tmp;

    
    i = left;
    j = right;
    tmp = a[left];

    if (i > j)
        return;

    while (i != j) {
        while (a[j] >= tmp && j > i)
            j--;
        while (a[i] <= tmp && j > i)
            i++;
        
        if (j > i) {
            temp = a[j];
            a[j] = a[i];
            a[i] = temp;
        }
    }
    a[left] = a[i];
    a[i] = tmp;

    quick_sort(a, left, i - 1);
    quick_sort(a, i + 1, right);
}
    
void shell_sort(int* arr, int size)
{
    int gap = size;
    while (gap > 1)
    {
        gap = gap / 3 + 1;	//调整希尔增量
        int i = 0;
        for (i = 0; i < size - gap; i++)	//从0遍历到size-gap-1
        {
            int end = i;
            int temp = arr[end + gap];
            while (end >= 0)
            {
                if (arr[end] > temp)
                {
                    arr[end + gap] = arr[end];
                    end -= gap;
                }
                else
                {
                    break;
                }
            }
            arr[end + gap] = temp;	//以 end+gap 作为插入位置
        }
    }
}
