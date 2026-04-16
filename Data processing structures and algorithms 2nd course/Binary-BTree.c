#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct B2Node {
    int key;
    struct B2Node* left;
    struct B2Node* right;
    int Bal;
} B2Node;

typedef struct AVLNode {
    int key;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

typedef struct {
    int size;
    int sum;
    int height;
    double avg_height;
} TreeStats;

int VR = 0;
int HR = 0;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int getHeight(AVLNode* node) {
    if (node == NULL) return 0;
    return node->height;
}

AVLNode* createAVLNode(int key) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

B2Node* createB2Node(int key) {
    B2Node* node = (B2Node*)malloc(sizeof(B2Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->Bal = 0;
    return node;
}

void B2INSERT(int D, B2Node** p) {
    if (*p == NULL) {
        *p = createB2Node(D);
        VR = 1;
        HR = 0;
    } else if ((*p)->key > D) {
        B2INSERT(D, &((*p)->left));
        if (VR == 1) {
            if ((*p)->Bal == 0) {
                B2Node* q = (*p)->left;
                (*p)->left = q->right;
                q->right = *p;
                *p = q;
                q->Bal = 1;
                VR = 0;
                HR = 1;
            } else {
                (*p)->Bal = 0;
                VR = 1;
                HR = 0;
            }
        } else {
            HR = 0;
        }
    } else if ((*p)->key < D) {
        B2INSERT(D, &((*p)->right));
        if (VR == 1) {
            (*p)->Bal = 1;
            HR = 1;
            VR = 0;
        } else {
            if (HR == 1) {
                if ((*p)->Bal == 1) {
                    B2Node* q = (*p)->right;
                    (*p)->Bal = 0;
                    q->Bal = 0;
                    (*p)->right = q->left;
                    q->left = *p;
                    *p = q;
                    VR = 1;
                    HR = 0;
                } else {
                    HR = 0;
                }
            }
        }
    }
}

AVLNode* rotateLL(AVLNode* p) {
    AVLNode* q = p->left;
    p->left = q->right;
    q->right = p;
    p->height = 1 + max(getHeight(p->left), getHeight(p->right));
    q->height = 1 + max(getHeight(q->left), getHeight(q->right));
    return q;
}

AVLNode* rotateRR(AVLNode* p) {
    AVLNode* q = p->right;
    p->right = q->left;
    q->left = p;
    p->height = 1 + max(getHeight(p->left), getHeight(p->right));
    q->height = 1 + max(getHeight(q->left), getHeight(q->right));
    return q;
}

AVLNode* rotateLR(AVLNode* p) {
    p->left = rotateRR(p->left);
    return rotateLL(p);
}

AVLNode* rotateRL(AVLNode* p) {
    p->right = rotateLL(p->right);
    return rotateRR(p);
}

int getBalance(AVLNode* node) {
    if (node == NULL) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

AVLNode* insertAVL(AVLNode* node, int key) {
    if (node == NULL)
        return createAVLNode(key);

    if (key < node->key)
        node->left = insertAVL(node->left, key);
    else if (key > node->key)
        node->right = insertAVL(node->right, key);
    else
        return node;

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)
        return rotateLL(node);
    if (balance < -1 && key > node->right->key)
        return rotateRR(node);
    if (balance > 1 && key > node->left->key)
        return rotateLR(node);
    if (balance < -1 && key < node->right->key)
        return rotateRL(node);

    return node;
}

void inOrderTraversal(B2Node* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("%d ", root->key);
        inOrderTraversal(root->right);
    }
}

void calculateStatsB2(B2Node* node, int current_height, TreeStats* stats, int* total_height) {
    if (node == NULL) return;
    stats->size++;
    stats->sum += node->key;
    if (current_height > stats->height) {
        stats->height = current_height;
    }
    *total_height += current_height;
    calculateStatsB2(node->left, current_height + 1, stats, total_height);
    calculateStatsB2(node->right, current_height + 1, stats, total_height);
}

void calculateStatsAVL(AVLNode* node, int current_height, TreeStats* stats, int* total_height) {
    if (node == NULL) return;
    stats->size++;
    stats->sum += node->key;
    if (current_height > stats->height) {
        stats->height = current_height;
    }
    *total_height += current_height;
    calculateStatsAVL(node->left, current_height + 1, stats, total_height);
    calculateStatsAVL(node->right, current_height + 1, stats, total_height);
}

void shuffleArray(int arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void freeB2Tree(B2Node* root) {
    if (root == NULL) return;
    freeB2Tree(root->left);
    freeB2Tree(root->right);
    free(root);
}

void freeAVLTree(AVLNode* root) {
    if (root == NULL) return;
    freeAVLTree(root->left);
    freeAVLTree(root->right);
    free(root);
}

int main() {
    srand(time(NULL));

    int numbers[100];
    for (int i = 0; i < 100; i++) {
        numbers[i] = i + 1;
    }
    shuffleArray(numbers, 100);

    B2Node* b2Root = NULL;
    AVLNode* avlRoot = NULL;

    for (int i = 0; i < 100; i++) {
        VR = 0;
        HR = 0;
        B2INSERT(numbers[i], &b2Root);
        avlRoot = insertAVL(avlRoot, numbers[i]);
    }

    printf("Обход ДБД слева направо:\n");
    inOrderTraversal(b2Root);
    printf("\n\n");

    TreeStats b2Stats = {0, 0, 0, 0.0};
    TreeStats avlStats = {0, 0, 0, 0.0};
    int b2TotalHeight = 0;
    int avlTotalHeight = 0;

    calculateStatsB2(b2Root, 1, &b2Stats, &b2TotalHeight);
    calculateStatsAVL(avlRoot, 1, &avlStats, &avlTotalHeight);

    b2Stats.avg_height = (double)b2TotalHeight / b2Stats.size;
    avlStats.avg_height = (double)avlTotalHeight / avlStats.size;

    printf("|n=100|Размер|Контр.Сумма|Высота|Средн.высота|\n");
    printf("|_____|______|___________|______|____________|\n");
    printf("| АВЛ |  %d |   %d    |   %d  |    %.2f    |\n", 
           avlStats.size, avlStats.sum, avlStats.height, avlStats.avg_height);
        printf("|_____|______|___________|______|____________|\n");
    printf("| ДБД |  %d |   %d    |   %d  |    %.2f    |\n", 
           b2Stats.size, b2Stats.sum, b2Stats.height, b2Stats.avg_height);
    printf("|_____|______|___________|______|____________|\n");

    freeB2Tree(b2Root);
    freeAVLTree(avlRoot);

    return 0;
}