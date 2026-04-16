#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Структура узла дерева
typedef struct Node {
    int key;
    struct Node* left;
    struct Node* right;
} Node;

// Структура для очереди
typedef struct QueueNode {
    Node* data;
    struct QueueNode* next;
} QueueNode;

// Создание нового узла дерева
Node* createNode(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Функции для очереди
void enqueue(QueueNode** head, Node* data) {
    QueueNode* newQ = (QueueNode*)malloc(sizeof(QueueNode));
    newQ->data = data;
    newQ->next = NULL;
    if (*head == NULL) {
        *head = newQ;
        return;
    }
    QueueNode* temp = *head;
    while (temp->next) {
        temp = temp->next;
    }
    temp->next = newQ;
}

Node* dequeue(QueueNode** head) {
    if (*head == NULL) return NULL;
    QueueNode* temp = *head;
    Node* data = temp->data;
    *head = temp->next;
    free(temp);
    return data;
}

int isQueueEmpty(QueueNode* head) {
    return head == NULL;
}

// Построение сбалансированного дерева (структура с равными поддеревьями)
Node* buildBalancedTree(int size) {
    if (size <= 0) {
        return NULL;
    }
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = 0; // Временный ключ
    newNode->left = NULL;
    newNode->right = NULL;

    int leftSize = (size - 1) / 2;
    int rightSize = size - 1 - leftSize;

    newNode->left = buildBalancedTree(leftSize);
    newNode->right = buildBalancedTree(rightSize);

    return newNode;
}

// Присвоение ключей в порядке level-order
void assignLevelOrderKeys(Node* root) {
    if (root == NULL) return;

    QueueNode* queue = NULL;
    enqueue(&queue, root);
    int key = 1;

    while (!isQueueEmpty(queue)) {
        Node* node = dequeue(&queue);
        node->key = key++;
        if (node->left) enqueue(&queue, node->left);
        if (node->right) enqueue(&queue, node->right);
    }
}

// Обход дерева слева-направо (симметричный обход)
void inOrderTraversal(Node* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("%d ", root->key);
        inOrderTraversal(root->right);
    }
}

// Обход сверху-вниз (прямой обход)
void preOrderTraversal(Node* root) {
    if (root != NULL) {
        printf("%d ", root->key);
        preOrderTraversal(root->left);
        preOrderTraversal(root->right);
    }
}

// Вычисление размера дерева
int getSize(Node* root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + getSize(root->left) + getSize(root->right);
}

// Вычисление контрольной суммы
int getChecksum(Node* root) {
    if (root == NULL) {
        return 0;
    }
    return root->key + getChecksum(root->left) + getChecksum(root->right);
}

// Вычисление высоты дерева
int getHeight(Node* root) {
    if (root == NULL) {
        return 0;
    }
    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

// Вычисление суммы высот всех узлов
int getTotalHeight(Node* root, int currentHeight) {
    if (root == NULL) {
        return 0;
    }
    return currentHeight + 
           getTotalHeight(root->left, currentHeight + 1) + 
           getTotalHeight(root->right, currentHeight + 1);
}

// Вычисление средней высоты
double getAverageHeight(Node* root) {
    int size = getSize(root);
    if (size == 0) {
        return 0.0;
    }
    int totalHeight = getTotalHeight(root, 1);
    return (double)totalHeight / size;
}

// Печать уровня дерева
void printLevel(Node* root, int level) {
    if (root == NULL) {
        printf("   ");
        return;
    }
    if (level == 1) {
        printf("%2d ", root->key);
    } else if (level > 1) {
        printLevel(root->left, level - 1);
        printLevel(root->right, level - 1);
    }
}

// Печать дерева по уровням
void printTree(Node* root) {
    int h = getHeight(root);
    for (int i = 1; i <= h; i++) {
        printf("Уровень %d: ", i);
        printLevel(root, i);
        printf("\n");
    }
}

int main() {
    int n;
    
    printf("Введите количество вершин: ");
    scanf("%d", &n);
    
    if (n <= 0) {
        printf("Количество вершин должно быть положительным числом.\n");
        return 1;
    }
    
    // Построение сбалансированной структуры дерева
    Node* root = buildBalancedTree(n);
    // Присвоение ключей в level-order
    assignLevelOrderKeys(root);
    
    // Вывод структуры дерева
    printf("\nСтруктура дерева:\n");
    printTree(root);
    printf("\n");
    
    // Вывод обхода слева-направо (симметричный)
    printf("Обход слева-направо: ");
    inOrderTraversal(root);
    printf("\n\n");
    
    // Вывод обхода сверху-вниз
    printf("Обход сверху-вниз: ");
    preOrderTraversal(root);
    printf("\n\n");
    
    // Вычисление характеристик дерева
    int size = getSize(root);
    int checksum = getChecksum(root);
    int height = getHeight(root);
    double avgHeight = getAverageHeight(root);
    
    printf("Характеристики дерева:\n");
    printf("Размер: %d\n", size);
    printf("Контрольная сумма: %d\n", checksum);
    printf("Высота: %d\n", height);
    printf("Средняя высота: %.2f\n", avgHeight);
    printf("\n");
    
    // Освобождение памяти (опционально, но рекомендуется)
    // Здесь можно добавить функцию для освобождения дерева
    
    return 0;
}