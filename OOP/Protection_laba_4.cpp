#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class Cinema {
private:
    string name, address, workTime;
    int capacity;

public:
    Cinema(string n = "", string addr = "", string time = "", int cap = 0) 
        : name(n), address(addr), workTime(time), capacity(cap) {}
    
    string getName() const { return name; }
    string getAddress() const { return address; }
    string getWorkTime() const { return workTime; }
    int getCapacity() const { return capacity; }
    
    void display() const {
        cout << "Название: " << name << "\nАдрес: " << address 
             << "\nВремя работы: " << workTime << "\nВместимость: " 
             << capacity << " чел.\n-----------------------------------\n";
    }
    
    void writeToFile(ofstream& file) const {
        file << name << '\n' << address << '\n' << workTime << '\n' << capacity << '\n';
    }
    
    bool readFromFile(ifstream& file) {
        if (!getline(file, name) || name.empty()) return false;
        if (!getline(file, address) || address.empty()) return false;
        if (!getline(file, workTime) || workTime.empty()) return false;
        
        string capStr;
        if (!getline(file, capStr) || capStr.empty()) return false;
        
        try {
            capacity = stoi(capStr);
        } catch (...) {
            return false; 
        }
        
        return capacity > 0; 
    }
    
    bool isLarge() const { return capacity > 200; }
};

bool compareByAddress(const Cinema& a, const Cinema& b) {
    return a.getAddress() < b.getAddress();
}

class CinemaManager {
private:
    vector<Cinema> allCinemas, largeCinemas;

    void updateLargeCinemas() {
        largeCinemas.clear();
        for (const auto& cinema : allCinemas) {
            if (cinema.isLarge()) {
                largeCinemas.push_back(cinema);
            }
        }
        sort(largeCinemas.begin(), largeCinemas.end(), compareByAddress);
    }

public:
    void loadFromFile(const string& filename, int K) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Ошибка открытия файла '" << filename << "'!\n";
            return;
        }
        
        int loaded = 0;
        while (loaded < K && !file.eof()) {
            Cinema cinema;
            if (cinema.readFromFile(file)) {
                allCinemas.push_back(cinema);
                loaded++;
            } else {
                break;
            }
        }
        file.close();
        
        updateLargeCinemas();
        cout << "Загружено кинотеатров: " << allCinemas.size() << endl;
    }
    
    void addNewCinema() {
        string name, address, workTime;
        int capacity;
        
        cout << "\nВведите данные нового кинотеатра:\nНазвание: ";
        getline(cin, name);
        cout << "Адрес: ";
        getline(cin, address);
        cout << "Время работы: ";
        getline(cin, workTime);
        cout << "Вместимость: ";
        cin >> capacity;
        cin.ignore();
        
        allCinemas.emplace_back(name, address, workTime, capacity);
        updateLargeCinemas();
        
        ofstream file("apteka.txt", ios::app);
        if (file.is_open()) {
            if (file.tellp() > 0) file << "\n";
            allCinemas.back().writeToFile(file);
            cout << "Данные успешно добавлены в файл!\n";
        } else {
            cout << "Ошибка записи в файл!\n";
        }
    }
    
    void displayAll() const {
        if (allCinemas.empty()) {
            cout << "\nНет данных о кинотеатрах.\n";
            return;
        }
        cout << "\nВсе кинотеатры (" << allCinemas.size() << "):\n";
        for (size_t i = 0; i < allCinemas.size(); i++) {
            cout << "[" << i + 1 << "] ";
            allCinemas[i].display();
        }
    }
    
    void displayLarge() const {
        if (largeCinemas.empty()) {
            cout << "\nНет кинотеатров с вместимостью > 200 человек.\n";
            return;
        }
        cout << "\nКинотеатры с вместимостью > 200 человек (" << largeCinemas.size() << "):\n";
        for (size_t i = 0; i < largeCinemas.size(); i++) {
            cout << "[" << i + 1 << "] ";
            largeCinemas[i].display();
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    
    CinemaManager manager;
    int K;
    
    cout << "Введите количество кинотеатров K: ";
    cin >> K;
    cin.ignore();
    
    manager.loadFromFile("apteka.txt", K);
    manager.displayAll();
    manager.displayLarge();
    
    char choice;
    cout << "Хотите добавить новый кинотеатр? (y/n): ";
    cin >> choice;
    cin.ignore();
    
    if (choice == 'y' || choice == 'Y') {
        manager.addNewCinema();
        manager.displayLarge();
    }
    
    cout << "\nПрограмма завершена.\n";
    return 0;
}