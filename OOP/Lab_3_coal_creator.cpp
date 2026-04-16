#include <iostream>
#include <fstream>
using namespace std;

class FileReader {
public:
    int count;
    
    bool readFromFile() {
        ifstream in("coal_data.txt");
        if (!in) {
            cout << "Ошибка открытия файла!" << endl;
            return false;
        }
        
        in >> count;
        cout << "Найдено бригад: " << count << endl;
        in.close();
        return true;
    }
};

class CoalBrigade : public FileReader {
public:
    int number;
    double price;
    double days[5];
    double total;
    
    void calculateTotal() {
        double sum = 0;
        for(int i = 0; i < 5; i++) {
            sum += days[i];
        }
        total = sum * price;
    }
    
    void saveToBinary() {
        ifstream in("coal_data.txt");
        in >> count; 
        
        CoalBrigade* brigades = new CoalBrigade[count];
        
        for (int i = 0; i < count; i++) {
            in >> brigades[i].number >> brigades[i].price;
            for (int j = 0; j < 5; j++) {
                in >> brigades[i].days[j];
            }
            brigades[i].calculateTotal();
        }
        in.close();
        
        ofstream out("coal_data.bin", ios::binary);
        out.write((char*)&count, sizeof(count));
        for (int i = 0; i < count; i++) {
            out.write((char*)&brigades[i], sizeof(CoalBrigade));
        }
        out.close();
        
        cout << "Создано " << count << " записей" << endl;
        delete[] brigades;
    }
};

int main() {
    cout << "Программа 1: Создание бинарного файла" << endl;
    
    CoalBrigade manager;
    if (manager.readFromFile()) {
        manager.saveToBinary();
    }
    
    return 0;
}