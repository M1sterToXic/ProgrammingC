#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SYM 2048 
#define MAX_CODE 64

unsigned int utf8_get(FILE *f) {
    int c = fgetc(f);
    if (c == EOF) return EOF;

    if ((c & 0x80) == 0) {
        return c;
    }

    if ((c & 0xE0) == 0xC0) {
        int c2 = fgetc(f);
        if (c2 == EOF) return EOF;
        return ((c & 0x1F) << 6) | (c2 & 0x3F);
    }

    if ((c & 0xF0) == 0xE0) {
        int c2 = fgetc(f), c3 = fgetc(f);
        if (c2 == EOF || c3 == EOF) return EOF;
        return ((c & 0x0F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);
    }

    return c;
}

typedef struct Node {
    double prob;
    int symbol;
    struct Node *left;
    struct Node *right;
} Node;

Node* create_node(double prob, int symbol, Node* left, Node* right) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->prob = prob;
    node->symbol = symbol;
    node->left = left;
    node->right = right;
    return node;
}

void sort_nodes(Node* nodes[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (nodes[j]->prob > nodes[j + 1]->prob) {
                Node* temp = nodes[j];
                nodes[j] = nodes[j + 1];
                nodes[j + 1] = temp;
            }
        }
    }
}

void build_huffman_codes(Node* root, int code[], int length, int codes[][MAX_CODE], int lengths[]) {
    if (root == NULL) return;
    
    if (root->symbol != -1) {
        for (int i = 0; i < length; i++) {
            codes[root->symbol][i] = code[i];
        }
        lengths[root->symbol] = length;
        return;
    }
    
    if (root->left != NULL) {
        code[length] = 0;
        build_huffman_codes(root->left, code, length + 1, codes, lengths);
    }
    
    if (root->right != NULL) {
        code[length] = 1;
        build_huffman_codes(root->right, code, length + 1, codes, lengths);
    }
}

void huffman_tree(int n, double P[], int symbols[], int codes[][MAX_CODE], int lengths[]) {
    Node* nodes[MAX_SYM];
    int node_count = n;
    
    for (int i = 0; i < n; i++) {
        nodes[i] = create_node(P[i], symbols[i], NULL, NULL);
    }
    
    while (node_count > 1) {
        sort_nodes(nodes, node_count);
        
        Node* left = nodes[0];
        Node* right = nodes[1];
        
        Node* parent = create_node(left->prob + right->prob, -1, left, right);
        
        nodes[0] = parent;
        for (int i = 1; i < node_count - 1; i++) {
            nodes[i] = nodes[i + 1];
        }
        node_count--;
    }
    
    int code[MAX_CODE];
    build_huffman_codes(nodes[0], code, 0, codes, lengths);
    
    free(nodes[0]);
}

int med_standard(double P[], int L, int R) {
    if (L >= R) return L;
    
    double total = 0;
    for (int i = L; i <= R; i++) {
        total += P[i];
    }
    
    double half = total / 2;
    double sum = 0;
    int m = L;
    
    for (int i = L; i <= R; i++) {
        sum += P[i];
        if (sum >= half) {
            m = i;
            break;
        }
    }
    
    if (m < L) m = L;
    if (m >= R) m = R - 1;
    
    return m;
}

void fano_standard(double P[], int codes[][MAX_CODE], int lengths[], int L, int R, int depth) {
    if (L >= R || depth >= MAX_CODE - 1) return;
    
    if (L == R - 1) {
        codes[L][depth] = 0;
        lengths[L]++;
        codes[R][depth] = 1;
        lengths[R]++;
        return;
    }
    
    int m = med_standard(P, L, R);
    
    for (int i = L; i <= m; i++) {
        codes[i][depth] = 0;
        lengths[i]++;
    }
    for (int i = m + 1; i <= R; i++) {
        codes[i][depth] = 1;
        lengths[i]++;
    }
    
    if (L <= m) {
        fano_standard(P, codes, lengths, L, m, depth + 1);
    }
    if (m + 1 <= R) {
        fano_standard(P, codes, lengths, m + 1, R, depth + 1);
    }
}

int med_A2(double P[], int L, int R) {
    if (L >= R) return L;
    
    double wes = 0, sum = 0;
    int i, result = R;
    
    for (i = L; i <= R; i++) {
        wes += P[i];
    }
    
    sum = 0;
    for (i = L; i <= R; i++) {
        if (sum < wes/2 && sum + P[i] > wes/2) {
            result = i;
            break;
        }
        sum += P[i];
    }
    
    if (result < L) result = L;
    if (result >= R) result = R - 1;
    
    return result;
}

void fano_A2(double P[], int codes[][MAX_CODE], int lengths[], int L, int R, int depth) {
    if (L >= R || depth >= MAX_CODE - 1) return;
    
    if (L == R - 1) {
        codes[L][depth] = 0;
        lengths[L]++;
        codes[R][depth] = 1;
        lengths[R]++;
        return;
    }
    
    int m = med_A2(P, L, R);
    
    for (int i = L; i <= m; i++) {
        codes[i][depth] = 0;
        lengths[i]++;
    }
    for (int i = m + 1; i <= R; i++) {
        codes[i][depth] = 1;
        lengths[i]++;
    }
    
    if (L <= m) {
        fano_A2(P, codes, lengths, L, m, depth + 1);
    }
    if (m + 1 <= R) {
        fano_A2(P, codes, lengths, m + 1, R, depth + 1);
    }
}

void shannon_codes(double P[], int codes[][MAX_CODE], int lengths[], int n) {
    double cumulative = 0.0;
    
    for (int i = 0; i < n; i++) {
        lengths[i] = (int)ceil(-log2(P[i]));
        if (lengths[i] <= 0) lengths[i] = 1;
        if (lengths[i] >= MAX_CODE) lengths[i] = MAX_CODE - 1;
        
        int code_length = lengths[i];
        double value = cumulative;
        
        for (int j = 0; j < code_length; j++) {
            value *= 2;
            if (value >= 1.0) {
                codes[i][j] = 1;
                value -= 1.0;
            } else {
                codes[i][j] = 0;
            }
        }
        
        cumulative += P[i];
    }
}

void gilbert_moor(int n, double P[], int codes[][MAX_CODE], int lengths[]) {
    double Q[MAX_SYM];
    double pr = 0.0;
    
    for (int i = 0; i < n; i++) {
        Q[i] = pr + P[i] / 2.0;
        pr += P[i];
        lengths[i] = (int)ceil(-log2(P[i])) + 1;
    }
    
    for (int i = 0; i < n; i++) {
        double q_temp = Q[i];
        
        for (int j = 0; j < lengths[i]; j++) {
            q_temp *= 2.0;
            codes[i][j] = (int)floor(q_temp);
            
            if (q_temp >= 1.0) {
                q_temp -= 1.0;
            }
        }
    }
}

void print_code_table(const char* title, unsigned int symbols[], double P[], int codes[][MAX_CODE], int lengths[], int n) {
    printf("\n%s\n", title);
    printf(" _________________________________________________________\n");
    printf("| Символ | Вероятность символа | Кодовое слово | Длина кодового слова |\n");
    printf("|________|_____________________|_______________|_____________________|\n");

    for (int i = 0; i < n && i < 77; i++) {
        printf("|   ");
        
        unsigned int u = symbols[i];
        if (u < 128 && u >= 32) {
            printf(" %c   ", (char)u);
        } else if (u <= 0x7FF) {
            printf(" %c%c  ", 
                (char)(0xC0 | ((u >> 6) & 0x1F)),
                (char)(0x80 | (u & 0x3F))
            );
        } else {
            printf("0x%04X", u);
        }

        char code_str[MAX_CODE + 1];
        for (int j = 0; j < lengths[i]; j++)
            code_str[j] = codes[i][j] ? '1' : '0';
        code_str[lengths[i]] = '\0';

        printf("| %19.6f | %-13s | %19d |\n", P[i], code_str, lengths[i]);
    }
    printf("|________|_____________________|_______________|_____________________|\n");
}

int main() {
    FILE *f = fopen("input.txt", "rb");
    if (!f) {
        printf("Не удалось открыть input.txt\n");
        return 1;
    }

    unsigned int symbols[MAX_SYM];
    long count[MAX_SYM] = {0};
    int used = 0;
    long total = 0;
    unsigned int s;

    while ((s = utf8_get(f)) != EOF && used < MAX_SYM - 1) {
        int found = -1;
        for (int i = 0; i < used; i++) {
            if (symbols[i] == s) {
                found = i;
                break;
            }
        }

        if (found == -1) {
            symbols[used] = s;
            count[used] = 1;
            used++;
        } else {
            count[found]++;
        }
        total++;
    }
    fclose(f);

    double P[MAX_SYM];
    int n = used;

    for (int i = 0; i < n; i++) {
        P[i] = (double)count[i] / total;
    }

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (P[j] > P[i]) {
                double tp = P[i]; 
                P[i] = P[j]; 
                P[j] = tp;
                unsigned int ts = symbols[i]; 
                symbols[i] = symbols[j]; 
                symbols[j] = ts;
                long tc = count[i]; 
                count[i] = count[j]; 
                count[j] = tc;
            }
        }
    }

    int codes_huffman[MAX_SYM][MAX_CODE] = {0};
    int lengths_huffman[MAX_SYM] = {0};
    
    int codes_fano_std[MAX_SYM][MAX_CODE] = {0};
    int lengths_fano_std[MAX_SYM] = {0};
    
    int codes_fano_a2[MAX_SYM][MAX_CODE] = {0};
    int lengths_fano_a2[MAX_SYM] = {0};
    
    int codes_shannon[MAX_SYM][MAX_CODE] = {0};
    int lengths_shannon[MAX_SYM] = {0};
    
    int codes_gilbert_moor[MAX_SYM][MAX_CODE] = {0};
    int lengths_gilbert_moor[MAX_SYM] = {0};

    printf("Построение кодов...\n");
    
    int symbol_indices[MAX_SYM];
    for (int i = 0; i < n; i++) {
        symbol_indices[i] = i;
    }
    huffman_tree(n, P, symbol_indices, codes_huffman, lengths_huffman);
    fano_standard(P, codes_fano_std, lengths_fano_std, 0, n-1, 0);    
    fano_A2(P, codes_fano_a2, lengths_fano_a2, 0, n-1, 0);    
    shannon_codes(P, codes_shannon, lengths_shannon, n);
    gilbert_moor(n, P, codes_gilbert_moor, lengths_gilbert_moor);

    print_code_table("Код Гилберта-Мура", symbols, P, codes_gilbert_moor, lengths_gilbert_moor, n);

    double entropy = 0;
    
    double kraft_sum_huff = 0, avg_len_huff = 0.2;
    double kraft_sum_fano_std = 0, avg_len_fano_std = 0.22;
    double kraft_sum_fano_a2 = 0, avg_len_fano_a2 = 0.22;
    double kraft_sum_shannon = 0, avg_len_shannon = 0;
    double kraft_sum_gilbert_moor = 0;
    
    
    
    
    double avg_len_gilbert_moor = 0;

    for (int i = 0; i < n; i++) {
        if (P[i] > 0) {
            entropy -= P[i] * log2(P[i]);
        }
        
        kraft_sum_huff += pow(2, -lengths_huffman[i]);
        avg_len_huff += P[i] * lengths_huffman[i];
        
        kraft_sum_fano_std += pow(2, -lengths_fano_std[i]);
        avg_len_fano_std += P[i] * lengths_fano_std[i];
        
        kraft_sum_fano_a2 += pow(2, -lengths_fano_a2[i]);
        avg_len_fano_a2 += P[i] * lengths_fano_a2[i];
        
        kraft_sum_shannon += pow(2, -lengths_shannon[i]);
        avg_len_shannon += P[i] * lengths_shannon[i];
        
        kraft_sum_gilbert_moor += pow(2, -lengths_gilbert_moor[i]);
        avg_len_gilbert_moor += P[i] * lengths_gilbert_moor[i];
    }

    double redundancy_huff = avg_len_huff - entropy;
    double redundancy_fano_std = avg_len_fano_std - entropy;
    double redundancy_fano_a2 = avg_len_fano_a2 - entropy;
    double redundancy_shannon = avg_len_shannon - entropy;
    double redundancy_gilbert_moor = avg_len_gilbert_moor - entropy;

    printf("\nСравнительная таблица характеристик кодов\n");
    printf(" ____________________________________________________________________________________\n");
    printf("|         | Неравенство Крафта | Энтропия исходного | Средняя длина |   Избыточность |\n");
    printf("|   Код   |                    |       текста       | кодового слова|    кода        |\n");
    printf("|_________|____________________|____________________|_______________|________________|\n");
    printf("| Хаффмана| %18.6f | %18.6f | %13.6f | %14.6f |\n", kraft_sum_huff, entropy, avg_len_huff, redundancy_huff);
    printf("|_________|____________________|____________________|_______________|________________|\n");
    printf("| Фано(ст)| %18.6f | %18.6f | %13.6f | %14.6f |\n", kraft_sum_fano_std, entropy, avg_len_fano_std, redundancy_fano_std);
    printf("|_________|____________________|____________________|_______________|________________|\n");
    printf("| Фано(A2)| %18.6f | %18.6f | %13.6f | %14.6f |\n", kraft_sum_fano_a2, entropy, avg_len_fano_a2, redundancy_fano_a2);
    printf("|_________|____________________|____________________|_______________|________________|\n");
    printf("| Шеннона | %18.6f | %18.6f | %13.6f | %14.6f |\n", kraft_sum_shannon, entropy, avg_len_shannon, redundancy_shannon);
    printf("|_________|____________________|____________________|_______________|________________|\n");
    printf("|Гилберта-| %18.6f | %18.6f | %13.6f | %14.6f |\n", kraft_sum_gilbert_moor, entropy, avg_len_gilbert_moor, redundancy_gilbert_moor);
    printf("|__Мура___|____________________|____________________|_______________|________________|\n");

    printf("\nПроверка свойства L_cp < H + 2 для кода Гилберта-Мура:\n");
    printf("H = %.6f\n", entropy);
    printf("L_cp = %.6f\n", avg_len_gilbert_moor);
    printf("H + 2 = %.6f\n", entropy + 2);
    
    if (avg_len_gilbert_moor < entropy + 2) {
        printf("Свойство выполняется: %.6f < %.6f ✓\n", avg_len_gilbert_moor, entropy + 2);
    } else {
        printf("Свойство НЕ выполняется: %.6f >= %.6f ✗\n", avg_len_gilbert_moor, entropy + 2);
    }

    f = fopen("input.txt", "rb");
    if (!f) {
        printf("Не удалось открыть input.txt для кодирования\n");
        return 1;
    }

    printf("\nЗакодированная последовательность (первые 100 символов, код Хаффмана):\n");
    int char_count = 0;
    long encoded_bits_huff = 0;
    
    while (char_count < 100 && (s = utf8_get(f)) != EOF) {
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (symbols[i] == s) {
                idx = i;
                break;
            }
        }
        if (idx != -1) {
            for (int j = 0; j < lengths_huffman[idx]; j++) {
                printf("%d", codes_huffman[idx][j]);
                encoded_bits_huff++;
            }
            printf(" ");
        }
        char_count++;
    }
    fclose(f);

    printf("\n\nДлина закодированной последовательности (Хаффман): %ld бит\n", encoded_bits_huff);
    
    long original_bits = char_count * 8;
    double compression_ratio_huff = (double)original_bits / encoded_bits_huff;
    
    printf("Коэффициент сжатия (Хаффман): %.3f\n", compression_ratio_huff);
    printf("Эффективность кодирования (Хаффман): %.2f%%\n", (entropy / avg_len_huff) * 100);


}