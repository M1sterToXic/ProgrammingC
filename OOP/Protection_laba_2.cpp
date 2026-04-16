#include <iostream>
#include <string>
using namespace std;

// Базовый класс Магазины инструментов
class ToolStore {
protected:
    string name;
    string phone;
    string address;
    string workTime;

public:
    // Конструктор без параметров
    ToolStore() {
        name = "";
        phone = "";
        address = "";
        workTime = "";
    }

    // Конструктор с параметрами
    ToolStore(string n, string p, string a, string wt) {
        name = n;
        phone = p;
        address = a;
        workTime = wt;
    }

    // Метод для вывода всех характеристик
    virtual void displayInfo() {
        cout << "Название: " << name << endl;
        cout << "Телефон: " << phone << endl;
        cout << "Адрес: " << address << endl;
        cout << "Время работы: " << workTime << endl;
    }
};

// Класс-наследник Электроинструмент
class PowerTool : public ToolStore {
private:
    string purpose;  // назначение

public:
    // Конструктор без параметров
    PowerTool() : ToolStore() {
        purpose = "";
    }

    // Конструктор с параметрами
    PowerTool(string n, string p, string a, string wt, string pur) 
        : ToolStore(n, p, a, wt) {
        purpose = pur;
    }

    // Переопределение метода вывода
    void displayInfo() override {
        ToolStore::displayInfo();
        cout << "Назначение: " << purpose << endl;
    }
};

int main() {
    // Демонстрация базового класса
    cout << "=== Демонстрация класса ToolStore ===" << endl;
    
    ToolStore store1;
    ToolStore store2("Молоток и Гвоздь", "+7-495-987-65-43", 
                     "пр. Мира, 25", "8:00-20:00");

    store1.displayInfo();
    cout << endl;
    store2.displayInfo();

    cout << endl;

    // Демонстрация класса-наследника
    cout << "=== Демонстрация класса PowerTool ===" << endl;
    
    PowerTool pt1;
    PowerTool pt2("Бош Центр", "+7-495-111-22-33", "ул. Центральная, 5", 
                  "9:00-19:00", "Официальный дилер инструментов Bosch");

    pt1.displayInfo();
    cout << endl;
    pt2.displayInfo();

    cout << endl;

    cout << "=== Демонстрация полиморфизма ===" << endl;
    
    ToolStore* stores[4] = {&store1, &store2, &pt1, &pt2};
    
    for (int i = 0; i < 4; i++) {
        cout << "Магазин " << i + 1 << ":\n";
        stores[i]->displayInfo();
        cout << "--------------------------\n";
    }

    return 0;
}