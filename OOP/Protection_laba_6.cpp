#include <iostream>
#include <string>
#include <queue>
using namespace std;

class Bus {
private:
    int busNumber;
    string driverName;
    int routeNumber;
public:
    Bus() : busNumber(0), driverName(""), routeNumber(0) {}
    Bus(int num, string name, int route) : busNumber(num), driverName(name), routeNumber(route) {}
    
    int getBusNumber() const { return busNumber; }
    string getDriverName() const { return driverName; }
    int getRouteNumber() const { return routeNumber; }
    
    void display() const {
        cout << "Автобус №" << busNumber << ", Водитель: " << driverName << ", Маршрут: " << routeNumber << endl;
    }
};

class BusPark {
private:
    queue<Bus> busesInPark;
    queue<Bus> busesOnRoute;
    
    Bus findBus(queue<Bus>& q, int busNumber) {
        queue<Bus> temp = q;
        while (!temp.empty()) {
            if (temp.front().getBusNumber() == busNumber) {
                return temp.front();
            }
            temp.pop();
        }
        return Bus();
    }
    
    bool removeBus(queue<Bus>& q, int busNumber) {
        queue<Bus> temp;
        bool found = false;
        
        while (!q.empty()) {
            if (q.front().getBusNumber() != busNumber) {
                temp.push(q.front());
            } else {
                found = true;
            }
            q.pop();
        }
        
        while (!temp.empty()) {
            q.push(temp.front());
            temp.pop();
        }
        
        return found;
    }
    
public:
    BusPark() {
        addBusToPark(101, "Иванов И.И.", 5);
        addBusToPark(102, "Петров П.П.", 12);
        addBusToPark(103, "Сидоров С.С.", 8);
        addBusToPark(104, "Козлов К.К.", 5);
    }
    
    void addBusToPark(int number, string driver, int route) {
        busesInPark.push(Bus(number, driver, route));
    }
    
    void busDeparture(int busNumber) {
        Bus bus = findBus(busesInPark, busNumber);
        if (bus.getBusNumber() == 0) {
            cout << "Автобус №" << busNumber << " не найден в парке!" << endl;
            return;
        }
        
        if (removeBus(busesInPark, busNumber)) {
            busesOnRoute.push(bus);
            cout << "Автобус №" << busNumber << " выехал на маршрут!" << endl;
        }
    }
    
    void busArrival(int busNumber) {
        Bus bus = findBus(busesOnRoute, busNumber);
        if (bus.getBusNumber() == 0) {
            cout << "Автобус №" << busNumber << " не найден на маршруте!" << endl;
            return;
        }
        
        if (removeBus(busesOnRoute, busNumber)) {
            busesInPark.push(bus);
            cout << "Автобус №" << busNumber << " вернулся в парк!" << endl;
        }
    }
    
    void showBusesInPark() {
        if (busesInPark.empty()) {
            cout << "В парке нет автобусов!" << endl;
            return;
        }
        
        cout << "\n=== АВТОБУСЫ В ПАРКЕ ===" << endl;
        queue<Bus> temp = busesInPark;
        int counter = 1;
        while (!temp.empty()) {
            cout << counter << ". ";
            temp.front().display();
            temp.pop();
            counter++;
        }
    }
    
    void showBusesOnRoute() {
        if (busesOnRoute.empty()) {
            cout << "На маршруте нет автобусов!" << endl;
            return;
        }
        
        cout << "\n=== АВТОБУСЫ НА МАРШРУТЕ ===" << endl;
        queue<Bus> temp = busesOnRoute;
        int counter = 1;
        while (!temp.empty()) {
            cout << counter << ". ";
            temp.front().display();
            temp.pop();
            counter++;
        }
    }
};

class Menu {
private:
    BusPark park;
    
    void displayMenu() {
        cout << "\n=== СИСТЕМА УПРАВЛЕНИЯ АВТОБУСНЫМ ПАРКОМ ===" << endl;
        cout << "1. Показать автобусы в парке" << endl;
        cout << "2. Показать автобусы на маршруте" << endl;
        cout << "3. Выезд автобуса из парка" << endl;
        cout << "4. Въезд автобуса в парк" << endl;
        cout << "5. Добавить новый автобус в парк" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите действие: ";
    }
    
    void handleBusDeparture() {
        int busNumber;
        cout << "Введите номер автобуса для выезда: ";
        cin >> busNumber;
        park.busDeparture(busNumber);
    }
    
    void handleBusArrival() {
        int busNumber;
        cout << "Введите номер автобуса для въезда: ";
        cin >> busNumber;
        park.busArrival(busNumber);
    }
    
    void handleAddBus() {
        int number, route;
        string driver;
        
        cout << "Введите номер автобуса: ";
        cin >> number;
        cin.ignore();
        cout << "Введите ФИО водителя: ";
        getline(cin, driver);
        cout << "Введите номер маршрута: ";
        cin >> route;
        
        park.addBusToPark(number, driver, route);
        cout << "Автобус добавлен в парк!" << endl;
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
                    park.showBusesInPark();
                    break;
                    
                case 2:
                    park.showBusesOnRoute();
                    break;
                    
                case 3:
                    handleBusDeparture();
                    break;
                    
                case 4:
                    handleBusArrival();
                    break;
                    
                case 5:
                    handleAddBus();
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