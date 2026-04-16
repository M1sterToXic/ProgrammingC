#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct {
    int indices[1000];
    int count;
    int comparisons;
} SearchResult;

SearchResult brute_force_search(const char* text, const char* pattern) {
    SearchResult result;
    result.count = 0;
    result.comparisons = 0;
    
    int n = strlen(text);       
    int m = strlen(pattern);    
    
    for (int i = 0; i <= n - m; i++) {
        int j;
        for (j = 0; j < m; j++) {
            result.comparisons++;
            if (text[i + j] != pattern[j]) {
                break;
            }
        }
        if (j == m) {
            result.indices[result.count] = i;
            result.count++;
        }
    }
    
    return result;
}

SearchResult rabin_karp_search(const char* text, const char* pattern) {
    SearchResult result;
    result.count = 0;
    result.comparisons = 0;
    
    int n = strlen(text);
    int m = strlen(pattern);
    
    int d = 256;    
    int q = 101;    
    
    if (n < m) return result;
    
    int h = 1;
    for (int i = 0; i < m - 1; i++) {
        h = (h * d) % q;
    }
    
    int p_hash = 0; 
    int t_hash = 0; 
    
    for (int i = 0; i < m; i++) {
        p_hash = (d * p_hash + pattern[i]) % q;
        t_hash = (d * t_hash + text[i]) % q;
    }
    
    for (int i = 0; i <= n - m; i++) {
        if (p_hash == t_hash) {
            int j;
            for (j = 0; j < m; j++) {
                result.comparisons++;
                if (text[i + j] != pattern[j]) {
                    break;
                }
            }
            if (j == m) {
                result.indices[result.count] = i;
                result.count++;
            }
        }
        
        if (i < n - m) {
            t_hash = (d * (t_hash - text[i] * h) + text[i + m]) % q;
            
            if (t_hash < 0) {
                t_hash += q;
            }
        }
    }
    
    return result;
}

void analyze_pattern_length_impact(const char* text) {
    printf("\n=== АНАЛИЗ ЗАВИСИМОСТИ ОТ ДЛИНЫ ПОДСТРОКИ ===\n");
    printf("Длина текста: %zu символов\n", strlen(text));
    
    const char* patterns[] = {
        "a",
        "world",
        "revolution",
        "technologicalgd",
        "interdisciplinarymkl",
        "international collaborati",
        "artificialintelligence becomes",
        "Nanotechnologyopensnewhornmaterials",
        "the secrets of quantum mechanics and str",
        "Interdisciplinary leadbreakthroughdiscoveries",
        "Renewable energy sources gradually replace traditi"
    };
    int pattern_count = 11;
    
    for (int i = 0; i < pattern_count; i++) {
        const char* pattern = patterns[i];
        SearchResult result = rabin_karp_search(text, pattern);
        
        printf("Длина: %2zu | Подстрока: %-50s | Сравнений: %5d | Вхождений: %3d\n", 
               strlen(pattern), pattern, result.comparisons, result.count);
    }
}

void print_results_summary(const char* pattern, SearchResult result, 
                          const char* method_name, double time_taken) {
    printf("=== %s ===\n", method_name);
    printf("Подстрока: %s\n", pattern);
    printf("Найдено вхождений: %d\n", result.count);
    
    if (result.count > 0) {
        printf("Индексы начала вхождений: ");
        for (int i = 0; i < result.count; i++) {
            printf("%d ", result.indices[i]);
        }
        printf("\n");
    }
    
    printf("Количество сравнений: %d\n", result.comparisons);
    printf("Время выполнения: %.6f секунд\n\n", time_taken);
}

int main() {
    char text[] = 
        "In the vast expanse of cosmic space, among billions of stars and galaxies, "
        "countless mysteries of the Universe are hidden. Scientists worldwide work tirelessly "
        "to decipher the secrets of quantum mechanics and string theory. Spacecraft "
        "explore Mars, Jupiter and Saturn, sending priceless data back to Earth. "
        "Artificial intelligence becomes increasingly sophisticated, helping to solve "
        "complex mathematical problems. In biological laboratories, researchers study "
        "the human genome, seeking ways to combat incurable diseases. Technological "
        "progress accelerates at an incredible pace, transforming our daily lives. "
        "The digital revolution affects all aspects of society, from education to healthcare. "
        "Cybersecurity becomes a critical issue in the era of the Internet of Things. "
        "Renewable energy sources gradually replace traditional fossil fuels. Quantum "
        "computers promise a revolution in computing technology, offering incredible "
        "processing power. Nanotechnology opens new horizons in materials science and "
        "medicine. Virtual reality and augmented reality change the ways we interact "
        "with digital information. Autonomous vehicles become a reality on our roads. "
        "Blockchain technology creates new paradigms for financial transactions. Space "
        "tourism gradually becomes available to wealthy individuals. Global warming "
        "requires urgent action from all humanity. Robotics and automation transform "
        "industrial production. 3D printing revolutionizes manufacturing processes "
        "across industries. Biotechnology offers innovative solutions for agriculture. "
        "In this rapidly changing world, it is important to remain curious and adaptable. "
        "Education becomes a lifelong process in the age of information explosion. "
        "Interdisciplinary research leads to breakthrough discoveries. International "
        "collaboration is necessary to solve global challenges. The future of humanity "
        "depends on our today's decisions and innovations.";

    char pattern[] = "revolution";

    printf("Размер текста: %zu байт\n", strlen(text));
    printf("Длина подстроки: %zu символов\n\n", strlen(pattern));
    
    clock_t start, end;
    double cpu_time_used;
    
    start = clock();  
    SearchResult brute_result = brute_force_search(text, pattern);
    end = clock();    
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    print_results_summary(pattern, brute_result, "Метод прямого поиска", cpu_time_used);
    
    start = clock();
    SearchResult rk_result = rabin_karp_search(text, pattern);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    print_results_summary(pattern, rk_result, "Метод Рабина-Карпа", cpu_time_used);
    
    printf("=== СРАВНЕНИЕ ЭФФЕКТИВНОСТИ ===\n");
    printf("Метод перебора: %d сравнений\n", brute_result.comparisons);
    printf("Метод Рабина-Карпа: %d сравнений\n", rk_result.comparisons);
    
    if (brute_result.comparisons > rk_result.comparisons) {
        double efficiency_ratio = (double)brute_result.comparisons / rk_result.comparisons;
        printf("Коэффициент эффективности: %.2f:1\n", efficiency_ratio);
        printf("Метод Рабина-Карпа эффективнее\n");
    } else if (rk_result.comparisons > brute_result.comparisons) {
        printf("Метод перебора эффективнее (редкий случай)\n");
    } else {
        printf("Методы одинаково эффективны\n");
    }
    
    analyze_pattern_length_impact(text);
    
    return 0;
}

/*
gcc -o string_search string_search.c -lm
./string_search
*/

/*
Функция analyze_pattern_length_impact() вызывается в конце main

Она создает свой собственный массив patterns[] с разными словами

Для каждого слова из этого массива запускается поиск

Результаты выводятся в таблицу
*/