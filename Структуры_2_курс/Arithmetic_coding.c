#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <sys/stat.h>
#include <wchar.h>
#include <wctype.h>
#include <ctype.h>

#define CODE_BITS     32
#define TOP_VALUE     ((1ULL << CODE_BITS) - 1)
#define FIRST_QTR     (TOP_VALUE / 4 + 1)
#define HALF          (2ULL * FIRST_QTR)
#define THIRD_QTR     (3ULL * FIRST_QTR)

typedef struct {
    unsigned int cp;
    unsigned int cum;
    unsigned int freq;
} Symbol;

Symbol syms[8192];
int nsym = 0;
unsigned int total_freq = 0;

unsigned long long low, high;
long long pending_bits = 0;

unsigned char *bitstream = NULL;
int bitcount = 0;
int bytes_allocated = 0;

void put_bit(int b) {
    if (bitcount >= bytes_allocated * 8) {
        bytes_allocated += 65536;
        bitstream = realloc(bitstream, bytes_allocated);
        if (!bitstream) {
            fprintf(stderr, "Ошибка выделения памяти для bitstream\n");
            exit(1);
        }
    }
    int byte_idx = bitcount >> 3;
    int bit_idx  = 7 - (bitcount & 7);
    if (b) bitstream[byte_idx] |=  (1 << bit_idx);
    else   bitstream[byte_idx] &= ~(1 << bit_idx);
    bitcount++;
}

void put_bits_follow(int b) {
    put_bit(b);
    while (pending_bits > 0) {
        put_bit(!b);
        pending_bits--;
    }
}

void encode_symbol(unsigned int cp) {
    int i;
    for (i = 0; i < nsym; i++) {
        if (syms[i].cp == cp) {
            goto found;
        }
    }
    fprintf(stderr, "\nКРИТИЧЕСКАЯ ОШИБКА: символ U+%04X не найден в таблице символов!\n", cp);
    fprintf(stderr, "Всего символов в таблице: %d\n", nsym);
    exit(1);

found:
    unsigned long long range = high - low + 1;
    high = low + range * (syms[i].cum + syms[i].freq) / total_freq - 1;
    low  = low + range * syms[i].cum / total_freq;

    for (;;) {
        if (high < HALF) {
            put_bits_follow(0);
        } else if (low >= HALF) {
            put_bits_follow(1);
            low  -= HALF;
            high -= HALF;
        } else if (low >= FIRST_QTR && high < THIRD_QTR) {
            pending_bits++;
            low  -= FIRST_QTR;
            high -= FIRST_QTR;
        } else {
            break;
        }
        low  <<= 1;
        high = (high << 1) | 1;
    }
}

void finish_encoding() {
    pending_bits++;
    put_bits_follow(low < FIRST_QTR ? 0 : 1);
}

int utf8_to_codepoint(const unsigned char *p, unsigned int *out) {
    if (p[0] < 0x80) { *out = p[0]; return 1; }
    if ((p[0] & 0xE0) == 0xC0 && (p[1] & 0xC0) == 0x80) { *out = ((p[0]&0x1F)<<6) | (p[1]&0x3F); return 2; }
    if ((p[0] & 0xF0) == 0xE0 && (p[1] & 0xC0) == 0x80 && (p[2] & 0xC0) == 0x80) {
        *out = ((p[0]&0x0F)<<12) | ((p[1]&0x3F)<<6) | (p[2]&0x3F); return 3;
    }
    if ((p[0] & 0xF8) == 0xF0 && (p[1] & 0xC0) == 0x80 && (p[2] & 0xC0) == 0x80 && (p[3] & 0xC0) == 0x80) {
        *out = ((p[0]&0x07)<<18) | ((p[1]&0x3F)<<12) | ((p[2]&0x3F)<<6) | (p[3]&0x3F); return 4;
    }
    *out = p[0];
    return 1;
}

void codepoint_to_string(unsigned int cp, char *buffer, size_t buffer_size) {
    if (buffer_size < 10) return;
    
    if (cp == ' ') {
        strncpy(buffer, "пробел", buffer_size);
    }
    else if (cp == '\n') {
        strncpy(buffer, "\\n", buffer_size);
    }
    else if (cp == '\t') {
        strncpy(buffer, "\\t", buffer_size);
    }
    else if (cp == '\r') {
        strncpy(buffer, "\\r", buffer_size);
    }
    else if (cp == 0) {
        strncpy(buffer, "NUL", buffer_size);
    }
    else if (cp < 128 && isprint((int)cp)) {
        snprintf(buffer, buffer_size, "%c", (char)cp);
    }
    else if (cp < 0x110000) {
        unsigned char utf8[5] = {0};
        if (cp < 0x80) {
            utf8[0] = (unsigned char)cp;
        }
        else if (cp < 0x800) {
            utf8[0] = 0xC0 | (unsigned char)(cp >> 6);
            utf8[1] = 0x80 | (unsigned char)(cp & 0x3F);
        }
        else if (cp < 0x10000) {
            utf8[0] = 0xE0 | (unsigned char)(cp >> 12);
            utf8[1] = 0x80 | (unsigned char)((cp >> 6) & 0x3F);
            utf8[2] = 0x80 | (unsigned char)(cp & 0x3F);
        }
        else if (cp < 0x110000) {
            utf8[0] = 0xF0 | (unsigned char)(cp >> 18);
            utf8[1] = 0x80 | (unsigned char)((cp >> 12) & 0x3F);
            utf8[2] = 0x80 | (unsigned char)((cp >> 6) & 0x3F);
            utf8[3] = 0x80 | (unsigned char)(cp & 0x3F);
        }
        
        if ((cp >= 0x0410 && cp <= 0x044F) ||  
            (cp >= 0x0400 && cp <= 0x04FF) ||  
            (cp >= 0x0370 && cp <= 0x03FF) ||  
            (cp >= 0x1F00 && cp <= 0x1FFF)) {  
            snprintf(buffer, buffer_size, "%s", utf8);
        }
        else {
            snprintf(buffer, buffer_size, "U+%04X", cp);
        }
    }
    else {
        snprintf(buffer, buffer_size, "U+%04X", cp);
    }
    buffer[buffer_size - 1] = '\0'; 
}

int main(int argc, char **argv) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    if (argc != 2) { printf("Использование: %s <файл.txt>\n", argv[0]); return 1; }

    FILE *f = fopen(argv[1], "rb");
    if (!f) { perror("Не удалось открыть файл"); return 1; }

    struct stat st;
    if (stat(argv[1], &st) != 0) {
        perror("Не удалось получить информацию о файле");
        fclose(f);
        return 1;
    }
    
    if (st.st_size < 10240) { 
        printf("Файл должен быть ≥10 КБ\n"); 
        fclose(f);
        return 1; 
    }

    unsigned char *buf = malloc(st.st_size + 1);
    if (!buf) {
        fprintf(stderr, "Ошибка выделения памяти для буфера\n");
        fclose(f);
        return 1;
    }
    
    size_t bytes_read = fread(buf, 1, st.st_size, f);
    if (bytes_read != st.st_size) {
        fprintf(stderr, "Предупреждение: прочитано %zu байт из %lld\n", bytes_read, (long long)st.st_size);
    }
    fclose(f);
    buf[bytes_read] = '\0'; 

    // 1. Подсчёт частот
    unsigned int *freq = calloc(0x110000, sizeof(unsigned int));
    if (!freq) {
        fprintf(stderr, "Ошибка выделения памяти для частот\n");
        free(buf);
        return 1;
    }
    
    size_t pos = 0;
    while (pos < bytes_read) {
        unsigned int cp;
        int adv = utf8_to_codepoint(buf + pos, &cp);
        if (cp < 0x110000) {
            freq[cp]++;
        }
        pos += adv;
    }

    // 2. Строим таблицу символов
    unsigned int cum = 0;
    nsym = 0;
    for (int i = 0; i < 0x110000; i++) {
        if (freq[i] > 0) {
            if (nsym >= 8192) {
                fprintf(stderr, "Превышен максимальный размер таблицы символов (8192)\n");
                break;
            }
            syms[nsym].cp   = i;
            syms[nsym].freq = freq[i];
            syms[nsym].cum  = cum;
            cum += freq[i];
            nsym++;  
        }
    }
    total_freq = cum;

    if (nsym == 0 || total_freq == 0) {
        printf("Ошибка: таблица символов пуста!\n");
        free(buf);
        free(freq);
        return 1;
    }

    printf("Файл: %s (%.1f КБ)  Уникальных символов: %d\n\n", argv[1], bytes_read/1024.0, nsym);

    // 3. Тестируем разные размеры блока с отображением сужения интервалов
    int blocks[] = {1,5,10,20,50,100,200,500,1000,0};

    printf("┌─────────┬───────────────────┬──────────────────────┬──────────────────────┐\n");
    printf("│  Блок   │    Сжатие, %%      │   Начальный диапазон │   Конечный диапазон  │\n");
    printf("├─────────┼───────────────────┼──────────────────────┼──────────────────────┤\n");

    for (int k = 0; blocks[k]; k++) {
        int bs = blocks[k];
        bitcount = 0; low = 0; high = TOP_VALUE; pending_bits = 0;
        if (bitstream) {
            free(bitstream);
            bitstream = NULL;
        }
        bytes_allocated = 0;

        pos = 0;
        while (pos < bytes_read) {
            low = 0; high = TOP_VALUE; pending_bits = 0;
            int count = 0;
            while (count < bs && pos < bytes_read) {
                unsigned int cp;
                int adv = utf8_to_codepoint(buf + pos, &cp);
                if (cp >= 0x110000) {
                    cp = '?'; 
                }
                encode_symbol(cp);
                pos += adv;
                count++;
            }
            finish_encoding();
        }

        double ratio = 100.0 * bitcount / (bytes_read * 8.0);
        
        // Сохраняем начальное и конечное состояние интервала
        unsigned long long initial_low = 0;
        unsigned long long initial_high = TOP_VALUE;
        unsigned long long final_low = low;
        unsigned long long final_high = high;
        
        // Рассчитываем ширину начального и конечного диапазонов
        unsigned long long initial_range = initial_high - initial_low + 1;
        unsigned long long final_range = final_high - final_low + 1;
        
        printf("│ %7d │ %17.2f%% │ %20llu │ %20llu │\n",
               bs, ratio, initial_range, final_range);
    }
    
    printf("└─────────┴───────────────────┴──────────────────────┴──────────────────────┘\n");

    printf("\n┌─────────────────────────────────────────────────────────────────────────────────────────┐\n");
    printf("│                      Процесс сужения интервала (первые 15 символов файла)              │\n");
    printf("├─────┬──────────────────┬──────────────────────┬──────────────────────┬─────────────────┤\n");
    printf("│  №  │     Символ       │        Low           │        High          │     Диапазон    │\n");
    printf("├─────┼──────────────────┼──────────────────────┼──────────────────────┼─────────────────┤\n");
    
    low = 0; high = TOP_VALUE;
    int detailed_count = 0;
    pos = 0;
    
    while (detailed_count < 15 && pos < bytes_read) {
        unsigned int cp;
        int adv = utf8_to_codepoint(buf + pos, &cp);
        
        if (cp >= 0x110000) {
            pos += adv;
            continue;
        }
        
        char symbol_str[32] = {0};
        codepoint_to_string(cp, symbol_str, sizeof(symbol_str));
        
        int i = 0;
        while (i < nsym && syms[i].cp != cp) i++;
        
        if (i >= nsym) {
            pos += adv;
            continue;
        }
        
        unsigned long long before_low = low;
        unsigned long long before_high = high;
        unsigned long long before_range = before_high - before_low + 1;
        
        unsigned long long range = high - low + 1;
        high = low + range * (syms[i].cum + syms[i].freq) / total_freq - 1;
        low = low + range * syms[i].cum / total_freq;
        
        unsigned long long after_low = low;
        unsigned long long after_high = high;
        unsigned long long after_range = after_high - after_low + 1;
        
        printf("│ %3d │ %-16s │ %20llu │ %20llu │ %15llu │\n",
               detailed_count + 1, symbol_str, after_low, after_high, after_range);
        
        for (;;) {
            if (high < HALF) {
                low <<= 1;
                high = (high << 1) | 1;
            }
            else if (low >= HALF) {
                low = (low - HALF) << 1;
                high = (high - HALF) << 1 | 1;
            }
            else if (low >= FIRST_QTR && high < THIRD_QTR) {
                low = (low - FIRST_QTR) << 1;
                high = (high - FIRST_QTR) << 1 | 1;
            }
            else break;
        }
        
        pos += adv;
        detailed_count++;
    }
    
    printf("└─────┴──────────────────┴──────────────────────┴──────────────────────┴─────────────────┘\n");

    printf("\nПроцесс сужения интервала (первые 35 уникальных символов):\n");
    printf("═══════════════════════════════════════════════════════════════════════════════════════════════════════════════\n");
    printf("№  Символ              Low (в десятичной)       High (в десятичной)        Диапазон (high-low+1)\n");
    printf("── ─────────────────── ─────────────────────── ─────────────────────── ─────────────────────────────\n");

    low = 0; high = TOP_VALUE;
    int *used = calloc(0x110000, sizeof(int));
    if (!used) {
        fprintf(stderr, "Ошибка выделения памяти для массива used\n");
        free(buf);
        free(freq);
        if (bitstream) free(bitstream);
        return 1;
    }
    
    int count = 0;
    pos = 0;

    while (count < 35 && pos < bytes_read) {
        unsigned int cp;
        int adv = utf8_to_codepoint(buf + pos, &cp);
        pos += adv;

        if (cp >= 0x110000 || used[cp]) continue;
        used[cp] = 1;

        unsigned long long range = high - low + 1;
        int i = 0; 
        while (i < nsym && syms[i].cp != cp) i++;
        
        if (i >= nsym) {
            continue; 
        }
        
        unsigned long long nl = low + range * syms[i].cum / total_freq;
        unsigned long long nh = low + range * (syms[i].cum + syms[i].freq) / total_freq - 1;

        char symbol_str[32] = {0};
        codepoint_to_string(cp, symbol_str, sizeof(symbol_str));

        printf("%2d  %-18s %23llu %23llu %30llu\n",
               count+1, symbol_str, nl, nh, nh-nl+1);

        low = nl; high = nh;

        for (;;) {
            if (high < HALF) {
                low <<= 1;
                high = (high << 1) | 1;
            }
            else if (low >= HALF) {
                low = (low - HALF) << 1;
                high = (high - HALF) << 1 | 1;
            }
            else if (low >= FIRST_QTR && high < THIRD_QTR) {
                low = (low - FIRST_QTR) << 1;
                high = (high - FIRST_QTR) << 1 | 1;
            }
            else break;
        }
        count++;
    }
    printf("═══════════════════════════════════════════════════════════════════════════════════════════════════════════════\n");

    free(buf);
    free(freq);
    free(used);
    if (bitstream) free(bitstream);
    return 0;
}