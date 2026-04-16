#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

// Определяем класс данных
class CoalBrigade {
public:
    int number;
    double price;
    double days[5];
    double total;
    
    void showBrigade() {
        cout << "|  " << number << "  |";
        cout << fixed << setprecision(1) << setw(6) << price << " |";
        
        for (int j = 0; j < 5; j++) {
            cout << setw(5) << days[j] << " |";
        }
        
        cout << setw(8) << setprecision(2) << total << " |" << endl;
    }
};

// Класс для чтения файлов
class BinaryReader {
public:
    int count;
    
    bool readBinaryFile(CoalBrigade* &brigades) {
        ifstream in("coal_data.bin", ios::binary);
        if (!in) {
            cout << "Ошибка открытия файла!" << endl;
            return false;
        }
        
        in.read((char*)&count, sizeof(count));
        brigades = new CoalBrigade[count];
        
        for (int i = 0; i < count; i++) {
            in.read((char*)&brigades[i], sizeof(CoalBrigade));
        }
        in.close();
        
        cout << "Прочитано бригад: " << count << endl;
        return true;
    }
};

// Дочерний класс для вывода на экран
class CoalManager : public BinaryReader {
public:
    void displayTable() {
        cout << "\nВЕДОМОСТЬ ДОБЫЧИ УГЛЯ" << endl;
        cout << "-------------------------------------------------------------" << endl;
        cout << "| Бр. | Цена  |  Пн  |  Вт  |  Ср  |  Чт  |  Пт  |  СУММА  |" << endl;
        cout << "-------------------------------------------------------------" << endl;
    }
    
    void findMinMax(CoalBrigade* brigades) {
        int max_br = brigades[0].number;
        double max_sum = brigades[0].total;
        int min_br = brigades[0].number;
        double min_sum = brigades[0].total;
        
        for (int i = 1; i < count; i++) {
            if (brigades[i].total > max_sum) {
                max_sum = brigades[i].total;
                max_br = brigades[i].number;
            }
            if (brigades[i].total < min_sum) {
                min_sum = brigades[i].total;
                min_br = brigades[i].number;
            }
        }
        
        cout << "Максимальная сумма у бригады " << max_br << ": " << max_sum << " руб." << endl;
        cout << "Минимальная сумма у бригады " << min_br << ": " << min_sum << " руб." << endl;
    }
    
    void showStatistics(CoalBrigade* brigades) {
        double all_total = 0;
        double all_tons = 0;
        
        for (int i = 0; i < count; i++) {
            all_total += brigades[i].total;
            for(int j = 0; j < 5; j++) {
                all_tons += brigades[i].days[j];
            }
        }
        
        cout << "\nИТОГИ:" << endl;
        cout << "Всего бригад: " << count << endl;
        cout << "Общая добыча: " << all_tons << " тонн" << endl;
        cout << "Общая сумма: " << all_total << " руб." << endl;
    }
};

int main() {
    cout << "Программа 2: Обработка бинарного файла" << endl;
    
    CoalManager manager;
    CoalBrigade* brigades;
    
    if (manager.readBinaryFile(brigades)) {
        manager.displayTable();
        
        for (int i = 0; i < manager.count; i++) {
            brigades[i].showBrigade();
        }
        cout << "-------------------------------------------------------------" << endl;
        
        manager.showStatistics(brigades);
        manager.findMinMax(brigades);
        
        delete[] brigades;
    }
    
    return 0;
}