#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Vertex {
    int val;
    int num;                // номер вершины
    struct Vertex *left;
    struct Vertex *right;
} Vertex;

Vertex* createVertex(int val) {
    Vertex* v = (Vertex*)malloc(sizeof(Vertex));
    if (!v) {
        fprintf(stderr, "Allocation failed\n");
        exit(EXIT_FAILURE);
    }
    v->val = val;
    v->num = 0;
    v->left = NULL;
    v->right = NULL;
    return v;
}

// --- Обходы ---
void Obhod1(Vertex* root) { // Preorder
    if (!root) return;
    printf("%d(%d) ", root->val, root->num);
    Obhod1(root->left);
    Obhod1(root->right);
}

void Obhod2(Vertex* root) { // Inorder
    if (!root) return;
    Obhod2(root->left);
    printf("%d(%d) ", root->val, root->num);
    Obhod2(root->right);
}

void Obhod3(Vertex* root) { // Postorder
    if (!root) return;
    Obhod3(root->left);
    Obhod3(root->right);
    printf("%d(%d) ", root->val, root->num);
}

// Размер дерева
int size(Vertex* root) {
    if (!root) return 0;
    return 1 + size(root->left) + size(root->right);
}

// Контрольная сумма
int checksum(Vertex* root) {
    if (!root) return 0;
    return root->val + checksum(root->left) + checksum(root->right);
}

// Высота дерева (количество уровней)
int height(Vertex* root) {
    if (!root) return 0;
    int hl = height(root->left);
    int hr = height(root->right);
    return 1 + (hl > hr ? hl : hr);
}

// Сумма глубин (root на глубине d)
int depthSum(Vertex* root, int d) {
    if (!root) return 0;
    return d + depthSum(root->left, d + 1) + depthSum(root->right, d + 1);
}

// Нумерация вершин по возрастанию (Inorder)
void assignNumbers(Vertex* root, int* counter) {
    if (!root) return;
    assignNumbers(root->left, counter);
    root->num = ++(*counter);
    assignNumbers(root->right, counter);
}

// Печатная красивая ASCII-структура (корень сверху)
void printTreeAsciiRec(Vertex* root, const char* prefix, int isLeft) {
    if (!root) return;

    printf("%s", prefix);
    if (isLeft) printf("|-- ");
    else printf("\\-- ");
    printf("%d(%d)\n", root->val, root->num);

    char newPref[256];
    strncpy(newPref, prefix, sizeof(newPref)-1);
    newPref[sizeof(newPref)-1] = '\0';

    if (isLeft) strncat(newPref, "|   ", sizeof(newPref)-strlen(newPref)-1);
    else strncat(newPref, "    ", sizeof(newPref)-strlen(newPref)-1);

    printTreeAsciiRec(root->left, newPref, 1);
    printTreeAsciiRec(root->right, newPref, 0);
}

void printTree(Vertex* root) {
    if (!root) return;
    printf("%d(%d)\n", root->val, root->num);
    printTreeAsciiRec(root->left, "", 1);
    printTreeAsciiRec(root->right, "", 0);
}

int main() {

    Vertex* n6 = createVertex(6);
    Vertex* n4 = createVertex(4); n4->right = n6;
    Vertex* n5 = createVertex(5);
    Vertex* n3 = createVertex(3); n3->left = n4; n3->right = n5;
    Vertex* n2 = createVertex(2); n2->left = n3;
    Vertex* n1 = createVertex(1); n1->right = n2;
    Vertex* R = n1; // корень

    // ---- Присвоение номеров (Inorder)
    int counter = 0;
    assignNumbers(R, &counter);

    // ---- Печать структуры ----
    printf("Структура дерева (корень сверху):\n");
    printTree(R);

    // ---- Обходы ----
    printf("\nObhod1 (сверху вниз): ");
    Obhod1(R); printf("\n\n");

    printf("Obhod2 (слева направо): ");
    Obhod2(R); printf("\n\n");

    printf("Obhod3 (снизу вверх): ");
    Obhod3(R); printf("\n\n");

    // ---- Метрики ----
    int n = size(R);
    int chk = checksum(R);
    int h = height(R);
    double avgDepth = (double)depthSum(R, 0) / n;

    printf("Размер дерева: %d\n", n);
    printf("Контрольная сумма: %d\n", chk);
    printf("Высота: %d\n", h);
    printf("Средняя глубина: %.2f\n", avgDepth);

    // Освобождаем память
    free(n6); free(n4); free(n5); free(n3); free(n2); free(n1);

    return 0;
}