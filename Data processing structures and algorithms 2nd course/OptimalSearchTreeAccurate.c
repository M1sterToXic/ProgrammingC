#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>

#define N 100

typedef struct Node {
    int key;
    int weight;
    struct Node* left;
    struct Node* right;
} Node;

Node* create_node(int key, int weight) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->key = key;
    new_node->weight = weight;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

void free_tree(Node* root) {
    if (root) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

void optimal_bst(int keys[], int weights[], int n, 
                 int AW[N+1][N+1], int AP[N+1][N+1], int AR[N+1][N+1]) {
    int i, j, h, k, m;
    int lower, upper;
    int min_val, cost, left_cost, right_cost;
    
    for (i = 0; i <= n; i++) {
        for (j = 0; j <= n; j++) {
            AW[i][j] = 0;
            AP[i][j] = 0;
            AR[i][j] = 0;
        }
    }
    
    for (i = 0; i < n; i++) {
        for (j = i + 1; j <= n; j++) {
            AW[i][j] = AW[i][j-1] + weights[j-1];
        }
    }
    
    for (i = 0; i < n; i++) {
        AP[i][i+1] = weights[i];
        AR[i][i+1] = i;
    }
    
    for (h = 2; h <= n; h++) {
        for (i = 0; i <= n - h; i++) {
            j = i + h;
            m = AR[i][j-1];
            
            if (m+1 <= j) {
                min_val = AP[i][m] + AP[m+1][j];
            } else {
                min_val = AP[i][m];
            }
            
            lower = AR[i][j-1];
            upper = (i+1 < n) ? AR[i+1][j] : (j-1);
            if (upper > j-1) upper = j-1;
            
            for (k = lower; k <= upper; k++) {
                left_cost = (i <= k-1) ? AP[i][k] : 0;
                right_cost = (k+1 <= j-1) ? AP[k+1][j] : 0;
                cost = left_cost + right_cost;
                
                if (cost < min_val) {
                    min_val = cost;
                    m = k;
                }
            }
            
            AP[i][j] = min_val + AW[i][j];
            AR[i][j] = m;
        }
    }
}

Node* build_tree_from_matrix(int keys[], int weights[], int AR[N+1][N+1], int i, int j) {
    if (i >= j) {
        return NULL;
    }
    
    int root_index = AR[i][j];
    Node* root = create_node(keys[root_index], weights[root_index]);
    
    root->left = build_tree_from_matrix(keys, weights, AR, i, root_index);
    root->right = build_tree_from_matrix(keys, weights, AR, root_index + 1, j);
    
    return root;
}

void left_to_right_traversal(Node* root, int* count, int* result_keys, int* result_weights) {
    if (root) {
        left_to_right_traversal(root->left, count, result_keys, result_weights);
        result_keys[*count] = root->key;
        result_weights[*count] = root->weight;
        (*count)++;
        left_to_right_traversal(root->right, count, result_keys, result_weights);
    }
}

void calculate_tree_properties(Node* root, int depth, 
                              int* size, int* control_sum, int* height, 
                              int* weighted_height, int* total_weight) {
    if (!root) {
        return;
    }
    
    int left_size = 0, left_sum = 0, left_height = 0, left_weighted = 0, left_total = 0;
    int right_size = 0, right_sum = 0, right_height = 0, right_weighted = 0, right_total = 0;
    
    calculate_tree_properties(root->left, depth + 1, 
                             &left_size, &left_sum, &left_height, 
                             &left_weighted, &left_total);
    
    calculate_tree_properties(root->right, depth + 1, 
                             &right_size, &right_sum, &right_height, 
                             &right_weighted, &right_total);
    
    *size = 1 + left_size + right_size;
    *control_sum = root->key + left_sum + right_sum;
    *height = 1 + ((left_height > right_height) ? left_height : right_height);
    *weighted_height = (root->weight * depth) + left_weighted + right_weighted;
    *total_weight = root->weight + left_total + right_total;
}

void print_matrix_first_10(int matrix[N+1][N+1], const char* title, int n) {
    int i, j;
    int display_size = (n > 10) ? 10 : n;
    
    printf("\n%s (первые %d элементов):\n", title, display_size);
    
    printf("     ");
    for (j = 0; j <= display_size; j++) {
        printf("%4d", j);
    }
    printf("\n     ");
    for (j = 0; j <= display_size; j++) {
        printf("----");
    }
    printf("\n");
    
    for (i = 0; i <= display_size; i++) {
        printf("%3d |", i);
        for (j = 0; j <= display_size; j++) {
            if (i < j) {
                printf("%4d", matrix[i][j]);
            } else {
                printf("    ");
            }
        }
        printf("\n");
    }
    
    if (n > 10) {
        printf("... (показаны только первые 10 элементов из %d)\n", n);
    }
}

void print_keys_and_weights_first_10(int keys[], int weights[], int n) {
    int i;
    int display_count = 100;
    
    printf("\nУпорядоченные ключи и их веса:\n");
    printf("Индекс  Ключ  Вес\n");
    printf("------  ----  ---\n");
    for (i = 0; i < display_count; i++) {
        printf("%6d  %4d  %3d\n", i, keys[i], weights[i]);
    }
}

void print_traversal_first_10(int keys[], int weights[], int count) {
    int i;
    int display_count = 100;
    
    printf("\nОБХОД ДЕРЕВА СЛЕВА НАПРАВО:\n");
    printf("Порядок  Ключ  Вес\n");
    printf("-------  ----  ---\n");
    for (i = 0; i < display_count; i++) {
        printf("%7d  %4d  %3d\n", i+1, keys[i], weights[i]);
    }
}

int main() {
    int keys[N];
    int weights[N];
    int AW[N+1][N+1], AP[N+1][N+1], AR[N+1][N+1];
    int i, total_weight = 0;
    
    int traversal_keys[N];
    int traversal_weights[N];
    int traversal_count = 0;
    
    srand(time(NULL));
    
    printf("ПОСТРОЕНИЕ ДЕРЕВА ОПТИМАЛЬНОГО ПОИСКА\n");
    printf("Количество вершин: %d\n", N);
    
    for (i = 0; i < N; i++) {
        keys[i] = i + 1;
        weights[i] = rand() % 100 + 1;
        total_weight += weights[i];
    }
    
    printf("Сумма весов: %d\n", total_weight);
    
    //print_keys_and_weights_first_10(keys, weights, N);
    
    optimal_bst(keys, weights, N, AW, AP, AR);
    
    print_matrix_first_10(AW, "МАТРИЦА ВЕСОВ AW", N);
    print_matrix_first_10(AP, "МАТРИЦА ВЗВЕШЕННЫХ ВЫСОТ AP", N);
    print_matrix_first_10(AR, "МАТРИЦА КОРНЕЙ AR", N);
    
    Node* root = build_tree_from_matrix(keys, weights, AR, 0, N);
    
    left_to_right_traversal(root, &traversal_count, traversal_keys, traversal_weights);
    print_traversal_first_10(traversal_keys, traversal_weights, traversal_count);
    
    int size, control_sum, height, weighted_height, tree_total_weight;
    calculate_tree_properties(root, 1, &size, &control_sum, &height, &weighted_height, &tree_total_weight);
    double average_height = (double)weighted_height / tree_total_weight;
    
    double theoretical_ratio = (double)AP[0][N] / AW[0][N];
    
    // printf("\n");
    // printf("╔══════════════════════════════════════════════════════════════════════╗\n");
    // printf("║                    ПРОВЕРКА ПРАВИЛЬНОСТИ АЛГОРИТМА                  ║\n");
    // printf("╠══════════════════════════════╦═══════════════════════════════════════╣\n");
    // printf("║ AP[0,n]                      ║ %-35d ║\n", AP[0][N]);
    // printf("║ AW[0,n]                      ║ %-35d ║\n", AW[0][N]);
    // printf("║ AP[0,n] / AW[0,n]            ║ %d / %d = %-18.3f ║\n", AP[0][N], AW[0][N], theoretical_ratio);
    // printf("║ Средневзвешенная высота      ║ %-35.3f ║\n", average_height);
    // printf("║ Абсолютная разница           ║ %-35.3f ║\n", fabs(theoretical_ratio - average_height));
    // printf("║ Относительная разница        ║ %-35.2f%% ║\n", fabs(theoretical_ratio - average_height) / theoretical_ratio * 100);
    // printf("╚══════════════════════════════╩═══════════════════════════════════════╝\n");
    
    printf("\n");
    
    // printf("╔══════════════════════════════════════════════════════════════════════╗\n");
    // printf("║               ХАРАКТЕРИСТИКИ ДЕРЕВА ОПТИМАЛЬНОГО ПОИСКА             ║\n");
    // printf("╠══════════════════════════════╦═══════════════════════════════════════╣\n");
    // printf("║ Количество вершин (n)        ║ %-35d ║\n", N);
    // printf("║ Размер дерева                ║ %-35d ║\n", size);
    // printf("║ Контрольная сумма            ║ %-35d ║\n", control_sum);
    // printf("║ Высота ДОП                   ║ %-35d ║\n", height);
    // printf("║ Средневзвешенная высота      ║ %-35.10f ║\n", average_height);
    // printf("║ Взвешенная высота P          ║ %-35d ║\n", weighted_height);
    // printf("║ Общий вес W                  ║ %-35d ║\n", tree_total_weight);
    // printf("║ Теоретическое отношение      ║ %-35.10f ║\n", theoretical_ratio);
    // printf("╚══════════════════════════════╩═══════════════════════════════════════╝\n");
    
    printf("\n");
    
    int min_weight = weights[0], max_weight = weights[0];
    double avg_weight = 0;
    for (i = 0; i < N; i++) {
        if (weights[i] < min_weight) min_weight = weights[i];
        if (weights[i] > max_weight) max_weight = weights[i];
        avg_weight += weights[i];
    }
    avg_weight /= N;
    
    // printf("╔══════════════════════════════════════════════════════════════════════╗\n");
    // printf("║                          СТАТИСТИКА ВЕСОВ                          ║\n");
    // printf("╠══════════════════════════════╦═══════════════════════════════════════╣\n");
    // printf("║ Минимальный вес              ║ %-35d ║\n", min_weight);
    // printf("║ Максимальный вес             ║ %-35d ║\n", max_weight);
    // printf("║ Средний вес                  ║ %-35.2f ║\n", avg_weight);
    // printf("║ Сумма всех весов             ║ %-35d ║\n", total_weight);
    // printf("╚══════════════════════════════╩═══════════════════════════════════════╝\n");

    printf("\n");
    printf("╔═════════════════════════════════════════════════════════════════════╗\n");
    printf("║                      ОСНОВНЫЕ ХАРАКТЕРИСТИКИ ДОП                    ║\n");
    printf("╠══════════╦══════════╦═══════════════╦══════════╦════════════════════╣\n");
    printf("║ n=100    ║ Размер   ║ Контр. Сумма  ║ Высота   ║ Средневзвеш.высота ║\n");
    printf("╠══════════╬══════════╬═══════════════╬══════════╬════════════════════╣\n");
    printf("║ ДОП      ║ %-8d ║ %-13d ║ %-8d ║ %-18.6f ║\n", 
           size, control_sum, height, average_height);
    printf("╚══════════╩══════════╩═══════════════╩══════════╩════════════════════╝\n");
    
    free_tree(root);
    
    return 0;
}