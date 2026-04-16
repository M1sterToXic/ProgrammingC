#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <stdint.h>

typedef struct
{
    uint32_t cp;
    long long f;
    double p, q;
    int L;
    char code[512];
} Sym;

typedef struct
{
    Sym *a;
    int n, cap;
} Vec;
static void vec_init(Vec *v)
{
    v->a = NULL;
    v->n = 0;
    v->cap = 0;
}
static void vec_push(Vec *v, Sym s)
{
    if (v->n == v->cap)
    {
        v->cap = v->cap ? v->cap * 2 : 256;
        v->a = (Sym *)realloc(v->a, v->cap * sizeof(Sym));
    }
    v->a[v->n++] = s;
}
static Sym *vec_find(Vec *v, uint32_t cp)
{
    for (int i = 0; i < v->n; i++)
        if (v->a[i].cp == cp)
            return &v->a[i];
    return NULL;
}

static inline double log2d(double x) { return log(x) / log(2.0); }

static int cmp_freq_desc(const void *a, const void *b)
{
    const Sym *x = (const Sym *)a, *y = (const Sym *)b;
    if (y->f != x->f)
        return (y->f > x->f) ? 1 : -1;
    return (x->cp > y->cp) - (x->cp < y->cp);
}

static int utf8_decode_one(const unsigned char *s, size_t n, uint32_t *out, size_t *consumed)
{
    if (n == 0)
        return 0;
    unsigned char b0 = s[0];
    if (b0 < 0x80)
    {
        *out = b0;
        *consumed = 1;
        return 1;
    }
    if ((b0 & 0xE0) == 0xC0)
    {
        if (n < 2)
            return 0;
        uint32_t cp = ((b0 & 0x1F) << 6) | (s[1] & 0x3F);
        if (cp < 0x80)
            return 0;
        *out = cp;
        *consumed = 2;
        return 1;
    }
    if ((b0 & 0xF0) == 0xE0)
    {
        if (n < 3)
            return 0;
        uint32_t cp = ((b0 & 0x0F) << 12) | ((s[1] & 0x3F) << 6) | (s[2] & 0x3F);
        if (cp < 0x800)
            return 0;
        *out = cp;
        *consumed = 3;
        return 1;
    }
    if ((b0 & 0xF8) == 0xF0)
    {
        if (n < 4)
            return 0;
        uint32_t cp = ((b0 & 0x07) << 18) | ((s[1] & 0x3F) << 12) | ((s[2] & 0x3F) << 6) | (s[3] & 0x3F);
        if (cp < 0x10000 || cp > 0x10FFFF)
            return 0;
        *out = cp;
        *consumed = 4;
        return 1;
    }
    return 0;
}

static int utf8_encode_one(uint32_t cp, char out[5])
{
    if (cp < 0x80)
    {
        out[0] = (char)cp;
        out[1] = 0;
        return 1;
    }
    if (cp < 0x800)
    {
        out[0] = (char)(0xC0 | (cp >> 6));
        out[1] = (char)(0x80 | (cp & 0x3F));
        out[2] = 0;
        return 2;
    }
    if (cp < 0x10000)
    {
        out[0] = (char)(0xE0 | (cp >> 12));
        out[1] = (char)(0x80 | ((cp >> 6) & 0x3F));
        out[2] = (char)(0x80 | (cp & 0x3F));
        out[3] = 0;
        return 3;
    }
    if (cp <= 0x10FFFF)
    {
        out[0] = (char)(0xF0 | (cp >> 18));
        out[1] = (char)(0x80 | ((cp >> 12) & 0x3F));
        out[2] = (char)(0x80 | ((cp >> 6) & 0x3F));
        out[3] = (char)(0x80 | (cp & 0x3F));
        out[4] = 0;
        return 4;
    }
    return 0;
}

typedef struct
{
    uint32_t cp;
    int idx;
} Map;
static int cmp_map_cp(const void *a, const void *b)
{
    const Map *x = (const Map *)a, *y = (const Map *)b;
    if (x->cp < y->cp)
        return -1;
    if (x->cp > y->cp)
        return 1;
    return 0;
}
static int map_bsearch_idx(const Map *m, int n, uint32_t cp)
{
    int lo = 0, hi = n - 1;
    while (lo <= hi)
    {
        int mid = (lo + hi) >> 1;
        if (m[mid].cp == cp)
            return m[mid].idx;
        if (m[mid].cp < cp)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return -1;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Использование: %s <файл UTF-8>\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "rb");
    if (!f)
    {
        fprintf(stderr, "Не удалось открыть '%s': %s\n", argv[1], strerror(errno));
        return 1;
    }

    Vec v;
    vec_init(&v);
    long long total_syms = 0;
    long long total_bytes = 0;
    uint32_t first_cps[100];
    int nfirst = 0;
    long long first_bytes = 0;

    unsigned char buf[1 << 15];
    size_t r;
    while ((r = fread(buf, 1, sizeof(buf), f)) > 0)
    {
        total_bytes += (long long)r;
        size_t i = 0;
        while (i < r)
        {
            uint32_t cp;
            size_t got;
            if (!utf8_decode_one(buf + i, r - i, &cp, &got))
            {
                cp = (uint32_t)buf[i];
                got = 1;
            }
            if (nfirst < 100)
            {
                first_cps[nfirst++] = cp;
                first_bytes += (long long)got;
            }
            i += got;
            Sym *s = vec_find(&v, cp);
            if (!s)
            {
                Sym t;
                t.cp = cp;
                t.f = 1;
                t.p = 0.0;
                t.q = 0.0;
                t.L = 0;
                t.code[0] = 0;
                vec_push(&v, t);
            }
            else
                s->f++;
            total_syms++;
        }
    }
    fclose(f);
    if (total_syms == 0)
    {
        fprintf(stderr, "Файл пуст\n");
        free(v.a);
        return 1;
    }

    for (int i = 0; i < v.n; i++)
        v.a[i].p = (double)v.a[i].f / (double)total_syms;
    qsort(v.a, v.n, sizeof(Sym), cmp_freq_desc);

    double Q = 0.0;
    for (int i = 0; i < v.n; i++)
    {
        v.a[i].q = Q;
        v.a[i].L = (int)ceil(-log2d(v.a[i].p));
        Q += v.a[i].p;
    }
    for (int i = 0; i < v.n; i++) 
    {
        double q = v.a[i].q;
        int L = v.a[i].L;
        for (int j = 0; j < L; j++)
        {
            q *= 2.0;
            int bit = (int)floor(q);
            v.a[i].code[j] = bit ? '1' : '0';
            if (q > 1.0)
                q -= 1.0;
        }
        v.a[i].code[v.a[i].L] = '\0';
    }

    printf("Код Шеннона\n");
    printf("Символ\tВероятность символа\tКодовое слово\tДлина кодового слова\n");
    printf("-----------------------------------------------------------------\n");
    for (int i = 0; i < v.n; i++)
    {
        char utf8[5] = {0};
        if (!utf8_encode_one(v.a[i].cp, utf8))
        {
            utf8[0] = '?';
            utf8[1] = 0;
        }
        printf("%s\t%.6f\t\t%s\t\t%d\n", utf8, v.a[i].p, v.a[i].code, v.a[i].L);
    }

    double kraft = 0.0, H = 0.0, Lavg = 0.0;
    for (int i = 0; i < v.n; i++)
    {
        kraft += pow(2.0, -(double)v.a[i].L);
        if (v.a[i].p > 0)
        {
            H += -v.a[i].p * log2d(v.a[i].p);
            Lavg += v.a[i].p * (double)v.a[i].L;
        }
    }

    printf("\nКод Шеннона\n");
    printf("Неравенство Крафта: %.6f (%s)\n", kraft, kraft <= 1.0 ? "выполняется" : "не выполняется");
    printf("Энтропия исходного текста: %.6f\n", H);
    printf("Средняя длина кодового слова: %.6f\n", Lavg);
    printf("Избыточность кода Шеннона: %.6f\n\n", (Lavg - H));

    Map *map = (Map *)malloc(v.n * sizeof(Map));
    for (int i = 0; i < v.n; i++)
    {
        map[i].cp = v.a[i].cp;
        map[i].idx = i;
    }
    qsort(map, v.n, sizeof(Map), cmp_map_cp);

    size_t cap = 8192;
    size_t blen = 0;
    char *bits = (char *)malloc(cap);
    for (int i = 0; i < nfirst; i++)
    {
        int idx = map_bsearch_idx(map, v.n, first_cps[i]);
        if (idx < 0)
            continue;
        int L = v.a[idx].L;
        if (blen + (size_t)L + 1 > cap)
        {
            while (blen + (size_t)L + 1 > cap)
                cap *= 2;
            bits = (char *)realloc(bits, cap);
        }
        memcpy(bits + blen, v.a[idx].code, (size_t)L);
        blen += (size_t)L;
    }
    bits[blen] = '\0';

    long long enc_bits = (long long)blen;
    long long orig_bits = (long long)first_bytes * 8LL;
    double cr = enc_bits ? (double)orig_bits / (double)enc_bits : 0.0;

    printf("=== Кодирование 100 символов ===\n");
    printf("Исходный текст (100 символов):\n");
    for (int i = 0; i < nfirst; i++) {
        char utf8[5] = {0};
        utf8_encode_one(first_cps[i], utf8);
        printf("%s", utf8);
    }
    printf("\n\n");
    
    printf("Закодированная последовательность (%lld бит):\n", enc_bits);
    for (size_t i = 0; i < blen; i++) {
        printf("%c", bits[i]);
        if ((i + 1) % 80 == 0) printf("\n");
    }
    printf("\n\n");
    
    printf("Исходная длина: %lld бит\n", orig_bits);
    printf("Закодированная длина: %lld бит\n", enc_bits);
    printf("Коэффициент сжатия: %.4f\n", cr);

    free(bits);
    free(map);
    free(v.a);
    return 0;
}