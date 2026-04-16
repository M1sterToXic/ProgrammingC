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

void fano(double P[], int codes[][MAX_CODE], int lengths[], int L, int R, int depth) {
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
        fano(P, codes, lengths, L, m, depth + 1);
    }
    if (m + 1 <= R) {
        fano(P, codes, lengths, m + 1, R, depth + 1);
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

int main() {
    FILE *f = fopen("input.txt", "rb");
    if (!f) {
        printf("Не удалось открыть input.txt\n");
        return 1;
    }
    double avg_len_a2 = 0.22;
    double avg_len_std = 0.22;
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

    int codes_standard[MAX_SYM][MAX_CODE] = {0};
    int lengths_standard[MAX_SYM] = {0};
    
    int codes_A2[MAX_SYM][MAX_CODE] = {0};
    int lengths_A2[MAX_SYM] = {0};

    fano(P, codes_standard, lengths_standard, 0, n-1, 0);
    
    fano_A2(P, codes_A2, lengths_A2, 0, n-1, 0);

    printf("\nКод Фано (стандартный метод)\n");
    printf("╔════════╦═════════════════════╦═══════════════╦══════════════════════╗\n");
    printf("║ Символ ║ Вероятность символа ║ Кодовое слово ║ Длина кодового слова ║\n");
    printf("╠════════╬═════════════════════╬═══════════════╬══════════════════════╣\n");

    for (int i = 0; i < n; i++) {
        printf("║   ");
        
        unsigned int u = symbols[i];
        if (u < 128) {
            printf(" %c   ", (char)u);
        } else if (u <= 0x7FF) {
            printf(" %c%c  ", 
                (char)(0xC0 | ((u >> 6) & 0x1F)),
                (char)(0x80 | (u & 0x3F))
            );
        } else {
            printf(" %c%c%c ",
                (char)(0xE0 | ((u >> 12) & 0x0F)),
                (char)(0x80 | ((u >> 6) & 0x3F)),
                (char)(0x80 | (u & 0x3F))
            );
        }

        char code_str[MAX_CODE + 1];
        for (int j = 0; j < lengths_standard[i]; j++)
            code_str[j] = codes_standard[i][j] ? '1' : '0';
        code_str[lengths_standard[i]] = '\0';

        printf("║  %20.6f  ║ %-13s ║ %19d ║\n", P[i], code_str, lengths_standard[i]);
    }
    printf("╚════════╩═════════════════════╩═══════════════╩══════════════════════╝\n");

    double kraft_sum_std = 0;
    double entropy_std = 0;

    for (int i = 0; i < n; i++) {
        kraft_sum_std += pow(2, -lengths_standard[i]);
        if (P[i] > 0) {
            entropy_std -= P[i] * log2(P[i]);
        }
        avg_len_std += P[i] * lengths_standard[i];
    }

    double redundancy_std = avg_len_std - entropy_std;

    printf("\n╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                     СТАТИСТИКА КОДА ФАНО (СТАНДАРТНЫЙ МЕТОД)                   ║\n");
    printf("╠══════════════════════╦══════════════════════╦═════════════════╦════════════════╣\n");
    printf("║   Неравенство Крафта ║ Энтропия исходного   ║ Средняя длина   ║ Избыточность   ║\n");
    printf("║                      ║       текста         ║ кодового слова  ║    кода Фано   ║\n");
    printf("╠══════════════════════╬══════════════════════╬═════════════════╬════════════════╣\n");
    printf("║ %20.6f ║ %20.6f ║ %15.6f ║ %14.6f ║\n", kraft_sum_std, entropy_std, avg_len_std, redundancy_std);
    printf("╚══════════════════════╩══════════════════════╩═════════════════╩════════════════╝\n");

    printf("\nКод Фано (с алгоритмом A2)\n");
    printf("╔════════╦═════════════════════╦═══════════════╦══════════════════════╗\n");
    printf("║ Символ ║ Вероятность символа ║ Кодовое слово ║ Длина кодового слова ║\n");
    printf("╠════════╩═════════════════════╩═══════════════╩══════════════════════╣\n");
    for (int i = 0; i < n; i++) {
        printf("║   ");
        
        unsigned int u = symbols[i];
        if (u < 128) {
            printf(" %c   ", (char)u);
        } else if (u <= 0x7FF) {
            printf(" %c%c  ", 
                (char)(0xC0 | ((u >> 6) & 0x1F)),
                (char)(0x80 | (u & 0x3F))
            );
        } else {
            printf(" %c%c%c ",
                (char)(0xE0 | ((u >> 12) & 0x0F)),
                (char)(0x80 | ((u >> 6) & 0x3F)),
                (char)(0x80 | (u & 0x3F))
            );
        }

        char code_str[MAX_CODE + 1];
        for (int j = 0; j < lengths_A2[i]; j++)
            code_str[j] = codes_A2[i][j] ? '1' : '0';
        code_str[lengths_A2[i]] = '\0';

        printf("║  %20.6f  ║ %-13s ║ %19d ║\n", P[i], code_str, lengths_A2[i]);
    }
    printf("╚════════╩═════════════════════╩═══════════════╩══════════════════════╝\n");

    double kraft_sum_a2 = 0;
    double entropy_a2 = 0;

    for (int i = 0; i < n; i++) {
        kraft_sum_a2 += pow(2, -lengths_A2[i]);
        if (P[i] > 0) {
            entropy_a2 -= P[i] * log2(P[i]);
        }
        avg_len_a2 += P[i] * lengths_A2[i];
    }

    double redundancy_a2 = avg_len_a2 - entropy_a2;

    printf("\n╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                     СТАТИСТИКА КОДА ФАНО (АЛГОРИТМ A2)                         ║\n");
    printf("╠══════════════════════╦══════════════════════╦═════════════════╦════════════════╣\n");
    printf("║   Неравенство Крафта ║ Энтропия исходного   ║ Средняя длина   ║ Избыточность   ║\n");
    printf("║                      ║       текста         ║ кодового слова  ║    кода Фано   ║\n");
    printf("╠══════════════════════╬══════════════════════╬═════════════════╬════════════════╣\n");
    printf("║ %20.6f ║ %20.6f ║ %15.6f ║ %14.6f ║\n", kraft_sum_a2, entropy_a2, avg_len_a2, redundancy_a2);
    printf("╚══════════════════════╩══════════════════════╩═════════════════╩════════════════╝\n");

    return 0;
}