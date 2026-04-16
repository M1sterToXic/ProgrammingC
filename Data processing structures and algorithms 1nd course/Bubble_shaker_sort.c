#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 5
#define MAX 500

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
void generateArray(int arr[], int n, int type) {
    if (type == -1) { // Убывающий порядок
        for (int i = 0; i < n; i++)
            arr[i] = n - i;
    } else if (type == 1) { // Возрастающий порядок
        for (int i = 0; i < n; i++)
            arr[i] = i + 1;
    } else { // Случайный порядок
        for (int i = 0; i < n; i++)
            arr[i] = rand() % MAX;
    }
}
void bubbleSort(int arr[], int n, int *comparisons, int *swaps) {
    *comparisons = 0;
    *swaps = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            (*comparisons)++;
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                (*swaps)+=3;
            }
        }
    }
}

void shakerSort(int arr[], int n, int *comparisons, int *swaps) {
    *comparisons = 0;
    *swaps = 0;
    int left = 0, right = n - 1;
    int k = n;

    do {
        for (int j = right; j > left; j--) {
            (*comparisons)++;
            if (arr[j] < arr[j - 1]) {
                swap(&arr[j], &arr[j - 1]);
                (*swaps) += 3;
                k = j;
            }
        }
        left = k;

        for (int j = left; j < right; j++) {
            (*comparisons)++;
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                (*swaps) += 3;
                k = j;
            }
        }
        right = k;
    } while (left < right);
}

int main() {
    srand(time(NULL));
    int sizes[SIZE] = {100, 200, 300, 400, 500};
    int arr[MAX];
    
    printf("| n   | Bubble Sort (Убыв) | Bubble Sort (Случ) | Bubble Sort (Возр) | Shaker Sort (Убыв) | Shaker Sort (Случ) | Shaker Sort (Возр) |\n");
    printf("|-----|--------------------|--------------------|--------------------|--------------------|--------------------|--------------------|\n");
    
    for (int i = 0; i < SIZE; i++) {
        int n = sizes[i];
        int comparisons, swaps;
       
        generateArray(arr, n, -1);
        bubbleSort(arr, n, &comparisons, &swaps);
        int bubbleDesc = comparisons + swaps;
        
        generateArray(arr, n, 0);
        bubbleSort(arr, n, &comparisons, &swaps);
        int bubbleRand = comparisons + swaps;
        
        generateArray(arr, n, 1);
        bubbleSort(arr, n, &comparisons, &swaps);
        int bubbleAsc = comparisons + swaps;
        
 
        generateArray(arr, n, -1);
        shakerSort(arr, n, &comparisons, &swaps);
        int shakerDesc = comparisons + swaps;
        
        generateArray(arr, n, 0);
        shakerSort(arr, n, &comparisons, &swaps);
        int shakerRand = comparisons + swaps;
        
        generateArray(arr, n, 1);
        shakerSort(arr, n, &comparisons, &swaps);
        int shakerAsc = comparisons + swaps;
        
        printf("| %3d | %18d | %18d | %18d | %18d | %18d | %18d |\n", n, bubbleDesc, bubbleRand, bubbleAsc, shakerDesc, shakerRand, shakerAsc);
    }
    return 0;
}
