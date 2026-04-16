#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void elias_gamma_encode(int n, char* code) {
    if (n <= 0) {
        code[0] = '\0';
        return;
    }
    
    int bits = 0;
    int temp = n;
    while (temp > 0) {
        bits++;
        temp >>= 1;
    }
    
    int pos = 0;
    for (int i = 0; i < bits - 1; i++) {
        code[pos++] = '0';
    }
    for (int i = bits - 1; i >= 0; i--) {
        code[pos++] = ((n >> i) & 1) ? '1' : '0';
    }
    code[pos] = '\0';
}

void elias_omega_encode(int n, char* code) {
    if (n <= 0) {
        code[0] = '\0';
        return;
    }
    
    if (n == 1) {
        strcpy(code, "0");
        return;
    }
    
    char groups[20][20];
    int group_count = 0;
    int k = n;
    
    while (k > 1) {
        int len = 0;
        int temp = k;
        while (temp > 0) {
            len++;
            temp >>= 1;
        }
        
        int idx = 0;
        for (int i = len - 1; i >= 0; i--) {
            groups[group_count][idx++] = ((k >> i) & 1) ? '1' : '0';
        }
        groups[group_count][idx] = '\0';
        group_count++;
        
        k = len - 1;
    }
    
    int pos = 0;
    for (int i = group_count - 1; i >= 0; i--) {
        for (int j = 0; groups[i][j] != '\0'; j++) {
            code[pos++] = groups[i][j];
        }
    }
    code[pos++] = '0';
    code[pos] = '\0';
}

void fixed_variable_encode(int n, char* code) {
    if (n == 0) {
        strcpy(code, "0");
        return;
    }
    
    int bits = 0;
    int temp = n;
    while (temp > 0) {
        bits++;
        temp >>= 1;
    }
    
    int pos = 0;
    for (int i = 0; i < bits - 1; i++) {
        code[pos++] = '0';
    }
    code[pos++] = '1';
    
    for (int i = bits - 2; i >= 0; i--) {
        code[pos++] = ((n >> i) & 1) ? '1' : '0';
    }
    code[pos] = '\0';
}

int main() {
    printf("┏━━━━━━┳━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━┓\n");
    printf("┃ Число┃   Fixed+Variable  ┃  Гамма-код Элиаса ┃ Омега-код Элиаса ┃\n");
    printf("┣━━━━━━╋━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━┫\n");
    
    for (int n = 0; n <= 256; n++) {
        char fv[100];
        char gamma[100];
        char omega[100];
        
        fixed_variable_encode(n, fv);
        elias_gamma_encode(n, gamma);
        elias_omega_encode(n, omega);
        
        printf("┃ %4d ┃ %17s ┃ %17s ┃ %16s ┃\n", n, fv, gamma, omega);
    }
    
    printf("┗━━━━━━┻━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━┛\n");
    
    return 0;
}