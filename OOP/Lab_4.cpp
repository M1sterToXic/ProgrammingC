#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

class EntertainmentVenue {
private:
    string venueType;    
    int capacity;        
    string name;         

public:
    EntertainmentVenue() {
        venueType = "Неизвестно";
        capacity = 0;
        name = "Без названия";
    }

    EntertainmentVenue(string type, int cap, string n) {
        venueType = type;
        capacity = cap;
        name = n;
    }

    void outputTo() const {
        cout << "Название: " << name << endl;
        cout << "Тип учреждения: " << venueType << endl;
        cout << "Вместимость: " << capacity << " человек" << endl;
        cout << "----------------------------------------" << endl;
    }

    string getVenueType() const {
        return venueType;
    }

    int getCapacity() const {
        return capacity;
    }

    string getName() const {
        return name;
    }

    bool operator<(const EntertainmentVenue& other) const {
        return capacity < other.capacity;
    }
};

class VectorManager {
private:
    vector<EntertainmentVenue> originalVector;
    vector<EntertainmentVenue> newVector;

public:
    void createOriginalVector() {
        int size;
        cout << "Введите размер исходного вектора: ";
        cin >> size;
        cin.ignore();

        for (int i = 0; i < size; i++) {
            string type, name;
            int capacity;

            cout << "\nОбъект " << i + 1 << ":" << endl;
            cout << "Введите тип учреждения: ";
            getline(cin, type);
            cout << "Введите название: ";
            getline(cin, name);
            cout << "Введите вместимость: ";
            cin >> capacity;
            cin.ignore();

            EntertainmentVenue venue(type, capacity, name);
            originalVector.push_back(venue);
        }
    }

    void createNewVector() {
        for (int i = 0; i < originalVector.size(); i++) {
            if (originalVector[i].getCapacity() > 100) {
                newVector.push_back(originalVector[i]);
            }
        }
        cout << "\nНовый вектор создан. Размер: " << newVector.size() << endl;
    }

    void checkEmpty() {
        if (newVector.empty()) {
            cout << "Новый вектор ПУСТ!" << endl;
        } else {
            cout << "Новый вектор НЕ ПУСТ." << endl;
        }
    }

    void sortNewVector() {
        if (!newVector.empty()) {
            sort(newVector.begin(), newVector.end());
            cout << "Новый вектор отсортирован по вместимости." << endl;
        }
    }

    void insertToNewVector() {
        if (newVector.empty()) {
            cout << "Невозможно вставить - вектор пуст!" << endl;
            return;
        }

        string type, name;
        int capacity;

        cout << "\nВведите данные для нового объекта:" << endl;
        cout << "Введите тип учреждения: ";
        getline(cin, type);
        cout << "Введите название: ";
        getline(cin, name);
        cout << "Введите вместимость: ";
        cin >> capacity;
        cin.ignore();

        EntertainmentVenue newVenue(type, capacity, name);

        vector<EntertainmentVenue>::iterator it;
        for (it = newVector.begin(); it != newVector.end(); it++) {
            if (newVenue.getCapacity() < it->getCapacity()) {
                break;
            }
        }

        newVector.insert(it, newVenue);
        cout << "Новый объект вставлен в вектор." << endl;
    }

    void printVector(const vector<EntertainmentVenue>& vec, string name) {
        cout << "\n=== " << name << " ===" << endl;
        cout << "Размер: " << vec.size() << " объектов" << endl;
        
        if (vec.empty()) {
            cout << "Вектор пуст!" << endl;
            return;
        }

        for (int i = 0; i < vec.size(); i++) {
            cout << "Объект " << i + 1 << ":" << endl;
            vec[i].outputTo();
        }
    }

    void run() {
        cout << "=== ЛАБОРАТОРНАЯ РАБОТА №4 ===" << endl;
        
        createOriginalVector();
        printVector(originalVector, "ИСХОДНЫЙ ВЕКТОР");
        
        createNewVector();
        checkEmpty();
        
        printVector(newVector, "НОВЫЙ ВЕКТОР ДО СОРТИРОВКИ");
        
        sortNewVector();
        printVector(newVector, "НОВЫЙ ВЕКТОР ПОСЛЕ СОРТИРОВКИ");
        
        insertToNewVector();
        printVector(newVector, "НОВЫЙ ВЕКТОР ПОСЛЕ ВСТАВКИ");
        
        cout << "=== ПРОГРАММА ЗАВЕРШЕНА ===" << endl;
    }
};

int main() {
    VectorManager manager;
    manager.run();
    return 0;
}