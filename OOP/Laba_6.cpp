#include <iostream>
#include <string>
#include <queue>
using namespace std;

class Toy {
private:
    string name;
    double price;
public:
    Toy() : name(""), price(0) {}
    Toy(string n, double p) : name(n), price(p) {}
    
    string getName() const { return name; }
    double getPrice() const { return price; }
    
    void display() const {
        cout << "Игрушка: " << name << ", Цена: " << price << " руб." << endl;
    }
};

class ToyShop {
private:
    queue<Toy> toyQueue;
    
public:
    ToyShop() {
        addToy("Мяч", 500);
        addToy("Кукла", 1200);
        addToy("Машинка", 800);
    }
    
    void addToy(string name, double price) {
        toyQueue.push(Toy(name, price));
    }
    
    void displayAllToys() {
        if (toyQueue.empty()) {
            cout << "Очередь пуста!" << endl;
            return;
        }
        
        cout << "\n=== СПИСОК ИГРУШЕК ===" << endl;
        queue<Toy> tempQueue = toyQueue;
        
        int counter = 1;
        while (!tempQueue.empty()) {
            cout << counter << ". ";
            tempQueue.front().display();
            tempQueue.pop();
            counter++;
        }
    }
    
    void findToyPrice(string searchName) {
        if (toyQueue.empty()) {
            cout << "Очередь пуста!" << endl;
            return;
        }
        
        queue<Toy> tempQueue = toyQueue;
        bool found = false;
        
        while (!tempQueue.empty()) {
            if (tempQueue.front().getName() == searchName) {
                cout << "Найдена игрушка: ";
                tempQueue.front().display();
                found = true;
                break;
            }
            tempQueue.pop();
        }
        
        if (!found) {
            cout << "Игрушка с названием '" << searchName << "' не найдена!" << endl;
        }
    }
    
    void addNewShipment() {
        int count;
        cout << "Сколько игрушек поступило? ";
        cin >> count;
        cin.ignore();
        
        for (int i = 0; i < count; i++) {
            string name;
            double price;
            
            cout << "Игрушка " << (i + 1) << ":" << endl;
            cout << "Введите название: ";
            getline(cin, name);
            cout << "Введите стоимость: ";
            cin >> price;
            cin.ignore();
            
            addToy(name, price);
        }
        cout << "Все игрушки добавлены в очередь!" << endl;
    }
    
    void findMostExpensiveToy() {
        if (toyQueue.empty()) {
            cout << "Очередь пуста!" << endl;
            return;
        }
        
        queue<Toy> tempQueue = toyQueue;
        Toy mostExpensive = tempQueue.front();
        tempQueue.pop();
        
        while (!tempQueue.empty()) {
            if (tempQueue.front().getPrice() > mostExpensive.getPrice()) {
                mostExpensive = tempQueue.front();
            }
            tempQueue.pop();
        }
        
        cout << "Самая дорогая игрушка: ";
        mostExpensive.display();
    }
    
    bool isEmpty() const {
        return toyQueue.empty();
    }
};

class Menu {
private:
    ToyShop shop;
    
    void displayMenu() {
        cout << "\n=== МЕНЮ ===" << endl;
        cout << "1. Добавить новую игрушку" << endl;
        cout << "2. Показать все игрушки" << endl;
        cout << "3. Найти стоимость игрушки по названию" << endl;
        cout << "4. Добавить новое поступление игрушек" << endl;
        cout << "5. Найти самую дорогую игрушку" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите действие: ";
    }
    
    void handleAddToy() {
        string name;
        double price;
        
        cout << "Введите название игрушки: ";
        getline(cin, name);
        cout << "Введите стоимость игрушки: ";
        cin >> price;
        cin.ignore();
        
        shop.addToy(name, price);
        cout << "Игрушка добавлена в очередь!" << endl;
    }
    
    void handleFindToy() {
        string searchName;
        cout << "Введите название игрушки для поиска: ";
        getline(cin, searchName);
        shop.findToyPrice(searchName);
    }
    
public:
    void run() {
        int choice;
        
        do {
            displayMenu();
            cin >> choice;
            cin.ignore();
            
            switch(choice) {
                case 1:
                    handleAddToy();
                    break;
                    
                case 2:
                    shop.displayAllToys();
                    break;
                    
                case 3:
                    handleFindToy();
                    break;
                    
                case 4:
                    shop.addNewShipment();
                    break;
                    
                case 5:
                    shop.findMostExpensiveToy();
                    break;
                    
                case 0:
                    cout << "Выход из программы..." << endl;
                    break;
                    
                default:
                    cout << "Неверный выбор! Попробуйте снова." << endl;
            }
            
        } while (choice != 0);
    }
};

int main() {
    Menu menu;
    menu.run();
    return 0;
}