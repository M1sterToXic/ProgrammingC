#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

// Линейное пробирование с проверкой уникальности
int linear_probing_chars(char A[], char table[], int n, int m) {
    int collisions = 0;
    bool unique_chars[256] = {false}; // Для отслеживания уникальных символов (ASCII)
    
    for (int i = 0; i < n; i++) {
        // Пропускаем дубликаты
        if (unique_chars[(unsigned char)A[i]]) {
            continue;
        }
        unique_chars[(unsigned char)A[i]] = true;
        
        int h = A[i] % m;
        int j = 0;
        while (j < m) {
            int idx = (h + j) % m;
            if (table[idx] == 0) {
                table[idx] = A[i];
                break;
            }
            // Считаем коллизию только если символы разные
            if (table[idx] != A[i]) {
                collisions++;
            }
            j++;
        }
    }
    return collisions;
}

// Квадратичное пробирование с проверкой уникальности
int quadratic_probing_chars(char A[], char table[], int n, int m) {
    int collisions = 0;
    bool unique_chars[256] = {false}; // Для отслеживания уникальных символов (ASCII)
    
    for (int i = 0; i < n; i++) {
        // Пропускаем дубликаты
        if (unique_chars[(unsigned char)A[i]]) {
            continue;
        }
        unique_chars[(unsigned char)A[i]] = true;
        
        int h = A[i] % m;
        int j = 1;
        int attempt = 0;
        while (attempt < m) {
            if (table[h] == 0) {
                table[h] = A[i];
                break;
            }
            // Считаем коллизию только если символы разные
            if (table[h] != A[i]) {
                collisions++;
            }
            h = (h + j * j) % m;
            j++;
            attempt++;
        }
    }
    return collisions;
}

// Красивая печать таблицы символов
void PrintSymbolTableFormatted(const char table[], int size, const char* title) {
    printf("\n%s:\n", title);
    const int cols = 5; // Уменьшил количество колонок для лучшей читаемости

    for (int row = 0; row < (size + cols - 1) / cols; row++) {
        int start = row * cols;
        int end = (start + cols < size) ? start + cols : size;

        // Индексы
        printf("Индексы: ");
        for (int i = start; i < end; i++) {
            printf("%4d", i);
        }
        printf("\nСимволы: ");
        for (int i = start; i < end; i++) {
            if (table[i] == 0)
                printf("%4c", '.');
            else
                printf("%4c", table[i]);
        }
        printf("\n");
    }
    printf("\n");
}

// Генерация случайного текста без дубликатов
void generate_unique_chars(char A[], int n) {
    bool used_chars[256] = {false};
    const char* charset = "Lorem ipsum dolor sit amet, consectetur adipiscing elit." 
                            "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua." 
                            "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat." 
                            "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur." 
                            "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum." 
                            "Lorem ipsum dolor sit amet, consectetur adipiscing elit."
                            "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua." 
                            "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat." 
                            "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur." 
                            "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum." 
                            "Lorem ipsum dolor sit amet, consectetur adipiscing elit." 
                            "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua." 
                            "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat." 
                            "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur." 
                            "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum." 
                            "Lorem ipsum dolor sit amet, consectetur adipiscing elit." 
                            "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua." 
                            "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat." 
                            "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur." 
                            "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
    int charset_len = strlen(charset);
    
    for (int i = 0; i < n; ) {
        char c = charset[rand() % charset_len];
        if (!used_chars[(unsigned char)c]) {
            A[i] = c;
            used_chars[(unsigned char)c] = true;
            i++;
        }
    }
    A[n] = '\0';
}

// ЗАДАНИЕ 2
void task2() {
    const int n = 20; // Количество уникальных символов
    const int m = 23; // Размер хеш-таблицы (простое число)
    
    char linear_table[m];
    char quadratic_table[m];
    char A[n+1];
    memset(linear_table, 0, m);
    memset(quadratic_table, 0, m);

    srand(time(NULL));
    generate_unique_chars(A, n); // Генерируем текст без дубликатов

    printf("ХЕШИРОВАНИЕ УНИКАЛЬНЫХ СИМВОЛОВ (m = %d) \n", m);

    int linear_coll = linear_probing_chars(A, linear_table, n, m);
    int quad_coll = quadratic_probing_chars(A, quadratic_table, n, m);

    PrintSymbolTableFormatted(linear_table, m, "Хеш-таблица (линейные пробы)");
    PrintSymbolTableFormatted(quadratic_table, m, "Хеш-таблица (квадратичные пробы)");
}

// ЗАДАНИЕ 3
void task3() {
    int table_sizes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    int sizes_count = sizeof(table_sizes) / sizeof(table_sizes[0]);
    const int n = 23;

    printf(" СРАВНЕНИЕ КОЛЛИЗИЙ (n = %d)\n", n);
    
    printf("┌────────────────┬───────────┬────────────┬────────────────┐\n");
    printf("│ Размер таблицы │ Элементов │ Линейные   │ Квадратичные   │\n");
    printf("│       (m)      │   (n)     │ коллизии   │   коллизии     │\n");
    printf("├────────────────┼───────────┼────────────┼────────────────┤\n");

    for (int i = 0; i < sizes_count; i++) {
        int m = table_sizes[i];
        char A[n];
        char linear_table[m];
        char quadratic_table[m];
        memset(linear_table, 0, m);
        memset(quadratic_table, 0, m);
        
        // Генерируем уникальные символы для каждого теста
        generate_unique_chars(A, n);

        int linear_coll = linear_probing_chars(A, linear_table, n, m);
        int quad_coll = quadratic_probing_chars(A, quadratic_table, n, m);
        
        printf("│ %14d │ %9d │ %10d │ %14d │\n", m, n, linear_coll, quad_coll);
    }
    printf("└────────────────┴───────────┴────────────┴────────────────┘\n");
}

// MAIN
int main() {
    task2();
    task3();
    return 0;
}