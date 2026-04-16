#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100
#define EXPERIMENTS 10

// Функция для заполнения массива возрастающими числами
void FillInc(int A[], int n) {
    for (int i = 0; i < n; i++) {
        A[i] = i + 1;
    }
}

// Функция для заполнения массива убывающими числами
void FillDec(int A[], int n) {
    for (int i = 0; i < n; i++) {
        A[i] = n - i;
    }
}

// Функция для заполнения массива случайными числами
void FillRand(int A[], int n) {
    for (int i = 0; i < n; i++) {
        A[i] = rand();
    }
}

// Функция для подсчета контрольной суммы массива
int CheckSum(int A[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += A[i];
    }
    return sum;
}

// Функция для подсчета числа серий в массиве
int RunNumber(int A[], int n) {
    if (n == 0) return 0;
    
    int count = 1;
    for (int i = 1; i < n; i++) {
        if (A[i] < A[i - 1]) { // Начало новой серии
            count++;
        }
    }
    return count;
}

// Функция для вывода массива на экран
void PrintMas(int A[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
}

// Функция для вычисления средней длины серии
double AvgRunLength(int A[], int n) {
    int seriesCount = RunNumber(A, n);
    return (double)n / seriesCount;
}

int main() {
    srand(time(NULL));

    int n=100;

    int A[n];

    // 
    // FillInc(A, n);
    // printf("Массив с возрастающими числами:\n");
    // PrintMas(A, n);
    // printf("Число серий: %d\n\n", RunNumber(A, n));

    // 
    // FillDec(A, n);
    // printf("Массив с убывающими числами:\n");
    // PrintMas(A, n);
    // printf("Число серий: %d\n\n", RunNumber(A, n));

    // 
    // FillRand(A, n);
    // printf("Случайный массив:\n");
    // PrintMas(A, n);
    // printf("Контрольная сумма: %d\n", CheckSum(A, n));
    // printf("Число серий: %d\n", RunNumber(A, n));
    // printf("Средняя длина серии: %.2f\n\n", AvgRunLength(A, n));

    // Дополнительное задание:
    double totalAvgLength = 0.0;
    int B[N]; // Отдельный массив для экспериментов

    for (int i = 0; i < EXPERIMENTS; i++) {
        FillRand(B, N);
        printf("Эксперимент %d: Сгенерированный массив:\n", i + 1);
        PrintMas(B, N);
        
        double avgLength = AvgRunLength(B, N);
        printf("Средняя длина серии = %.2f\n\n", avgLength);
        totalAvgLength += avgLength;
    }

    printf("\nСредняя длина серии по %d экспериментам: %.2f\n", EXPERIMENTS, totalAvgLength / EXPERIMENTS);

    return 0;
}