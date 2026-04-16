#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

typedef struct TreeNode {
    int key;
    int weight;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct {
    int key;
    int weight;
} KeyWeight;

typedef struct {
    int size;
    int checksum;
    int height;
    double weighted_height;
} TreeStats;

TreeNode* create_node(int key, int weight);
void build_optimal_bst(KeyWeight* items, int n, TreeNode** root);
void build_a1_tree(KeyWeight* items, int n, TreeNode** root);
void build_a2_tree(KeyWeight* items, int left, int right, TreeNode** root);
void calculate_stats(TreeNode* root, int level, TreeStats* stats);
void inorder_traversal(TreeNode* root);
void free_tree(TreeNode* root);
int compare_keys(const void* a, const void* b);

TreeNode* create_node(int key, int weight) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->key = key;
    node->weight = weight;
    node->left = node->right = NULL;
    return node;
}

void build_optimal_bst(KeyWeight* items, int n, TreeNode** root) {
    if (n == 0) {
        *root = NULL;
        return;
    }
    
    int** AW = (int**)malloc((n+1)*sizeof(int*));
    int** AP = (int**)malloc((n+1)*sizeof(int*));
    int** AR = (int**)malloc((n+1)*sizeof(int*));
    
    for (int i = 0; i <= n; i++) {
        AW[i] = (int*)calloc(n+1, sizeof(int));
        AP[i] = (int*)calloc(n+1, sizeof(int));
        AR[i] = (int*)calloc(n+1, sizeof(int));
    }

    for (int i = 0; i < n; i++) {
        for (int j = i+1; j <= n; j++) {
            AW[i][j] = AW[i][j-1] + items[j-1].weight;
        }
    }

    for (int i = 0; i < n; i++) {
        AP[i][i+1] = items[i].weight;
        AR[i][i+1] = i;
    }

    for (int length = 2; length <= n; length++) {
        for (int i = 0; i <= n - length; i++) {
            int j = i + length;
            int min_val = INT_MAX;
            int best_k = i;

            int lower = AR[i][j-1];
            int upper = (i+1 < n) ? AR[i+1][j] : j-1;

            for (int k = lower; k <= upper; k++) {
                int current = AP[i][k] + AP[k+1][j];
                if (current < min_val) {
                    min_val = current;
                    best_k = k;
                }
            }

            AP[i][j] = min_val + AW[i][j];
            AR[i][j] = best_k;
        }
    }

    TreeNode* build_tree(int i, int j) {
        if (i >= j) return NULL;
        int k = AR[i][j];
        TreeNode* node = create_node(items[k].key, items[k].weight);
        node->left = build_tree(i, k);
        node->right = build_tree(k+1, j);
        return node;
    }

    *root = build_tree(0, n);

    for (int i = 0; i <= n; i++) {
        free(AW[i]);
        free(AP[i]);
        free(AR[i]);
    }
    free(AW);
    free(AP);
    free(AR);
}

void build_a1_tree(KeyWeight* items, int n, TreeNode** root) {
    if (n == 0) {
        *root = NULL;
        return;
    }
    
    KeyWeight* sorted_items = (KeyWeight*)malloc(n * sizeof(KeyWeight));
    for (int i = 0; i < n; i++) {
        sorted_items[i] = items[i];
    }
    
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (sorted_items[j].weight > sorted_items[i].weight || 
               (sorted_items[j].weight == sorted_items[i].weight && sorted_items[j].key < sorted_items[i].key)) {
                KeyWeight temp = sorted_items[i];
                sorted_items[i] = sorted_items[j];
                sorted_items[j] = temp;
            }
        }
    }
    
    TreeNode* build_a1_recursive(KeyWeight* sorted, int size) {
        if (size == 0) return NULL;
        
        TreeNode* root = create_node(sorted[0].key, sorted[0].weight);
        
        int left_size = 0, right_size = 0;
        KeyWeight* left_items = (KeyWeight*)malloc(size * sizeof(KeyWeight));
        KeyWeight* right_items = (KeyWeight*)malloc(size * sizeof(KeyWeight));
        
        for (int i = 1; i < size; i++) {
            if (sorted[i].key < root->key) {
                left_items[left_size++] = sorted[i];
            } else {
                right_items[right_size++] = sorted[i];
            }
        }
        
        root->left = build_a1_recursive(left_items, left_size);
        root->right = build_a1_recursive(right_items, right_size);
        
        free(left_items);
        free(right_items);
        return root;
    }
    
    *root = build_a1_recursive(sorted_items, n);
    free(sorted_items);
}

void build_a2_tree(KeyWeight* items, int left, int right, TreeNode** root) {
    if (left > right) {
        *root = NULL;
        return;
    }
    
    int total_weight = 0;
    for (int i = left; i <= right; i++) {
        total_weight += items[i].weight;
    }
    
    int sum = 0;
    int split = left;
    for (split = left; split <= right; split++) {
        sum += items[split].weight;
        if (sum * 2 >= total_weight) break;
    }
    
    *root = create_node(items[split].key, items[split].weight);
    
    build_a2_tree(items, left, split-1, &((*root)->left));
    build_a2_tree(items, split+1, right, &((*root)->right));
}

void calculate_stats(TreeNode* root, int level, TreeStats* stats) {
    if (root == NULL) return;
    
    stats->size++;
    stats->checksum += root->key;
    if (level > stats->height) stats->height = level;
    stats->weighted_height += level * root->weight;
    
    calculate_stats(root->left, level+1, stats);
    calculate_stats(root->right, level+1, stats);
}

void inorder_traversal(TreeNode* root) {
    if (root == NULL) return;
    inorder_traversal(root->left);
    printf("%d ", root->key);
    inorder_traversal(root->right);
}

void free_tree(TreeNode* root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int compare_keys(const void* a, const void* b) {
    return ((KeyWeight*)a)->key - ((KeyWeight*)b)->key;
}

int main() {
    const int n = 100;
    KeyWeight items[n];
    
    srand(time(NULL));
    
    for (int i = 0; i < n; i++) {
        items[i].key = i + 1;
        items[i].weight = rand() % 100 + 1;
    }
    
    qsort(items, n, sizeof(KeyWeight), compare_keys);
    
    TreeNode *optimal_root = NULL, *a1_root = NULL, *a2_root = NULL;
    
    build_optimal_bst(items, n, &optimal_root);
    build_a1_tree(items, n, &a1_root);
    build_a2_tree(items, 0, n-1, &a2_root);
    
    TreeStats optimal_stats = {0, 0, 0, 0.0};
    TreeStats a1_stats = {0, 0, 0, 0.0};
    TreeStats a2_stats = {0, 0, 0, 0.0};
    
    calculate_stats(optimal_root, 1, &optimal_stats);
    calculate_stats(a1_root, 1, &a1_stats);
    calculate_stats(a2_root, 1, &a2_stats);
    
    double total_weight_optimal = 0, total_weight_a1 = 0, total_weight_a2 = 0;
    for (int i = 0; i < n; i++) {
        total_weight_optimal += items[i].weight;
        total_weight_a1 += items[i].weight;
        total_weight_a2 += items[i].weight;
    }
    
    double optimal_swh = optimal_stats.weighted_height / total_weight_optimal;
    double a1_swh = a1_stats.weighted_height / total_weight_a1;
    double a2_swh = a2_stats.weighted_height / total_weight_a2;
    
    printf("n=%d\n", n);
    printf(" _________________________________________________________________\n");
    printf("|     Метод    | Размер | Контр.сумма| Высота | Сред.взвеш.высота |\n");
    printf("|______________|________|____________|________|___________________|\n");
    printf("| ДОП (точный) | %6d | %10d | %6d | %17.6f |\n", 
           optimal_stats.size, optimal_stats.checksum, optimal_stats.height, optimal_swh);
    printf("| А1 (макс.вес)| %6d | %10d | %6d | %17.6f |\n", 
           a1_stats.size, a1_stats.checksum, a1_stats.height, a1_swh);
    printf("| А2 (медиана) | %6d | %10d | %6d | %17.6f |\n", 
           a2_stats.size, a2_stats.checksum, a2_stats.height, a2_swh);
    printf(" ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
    
    printf("\nОбход ДОП (inorder): ");
    inorder_traversal(optimal_root);
    printf("\nОбход А1 (inorder): ");
    inorder_traversal(a1_root);
    printf("\nОбход А2 (inorder): ");
    inorder_traversal(a2_root);
    printf("\n");
    
    free_tree(optimal_root);
    free_tree(a1_root);
    free_tree(a2_root);
    
    return 0;
}