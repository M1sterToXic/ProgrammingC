#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Airport {
private:
    string name;
    string location;
    int flightCount;
    string status;

public:
    Airport() {
        name = "Неизвестно";
        location = "Неизвестно";
        flightCount = 0;
        status = "Неизвестно";
    }

    Airport(string n, string loc, int count, string stat) {
        name = n;
        location = loc;
        flightCount = count;
        status = stat;
    }

    void setName(string n) { name = n; }
    void setLocation(string loc) { location = loc; }
    void setFlightCount(int count) { flightCount = count; }
    void setStatus(string stat) { status = stat; }

    string getName() { return name; }
    string getLocation() { return location; }
    int getFlightCount() { return flightCount; }
    string getStatus() { return status; }

    void display() {
        cout << "Название: " << name << endl;
        cout << "Местоположение: " << location << endl;
        cout << "Количество рейсов: " << flightCount << endl;
        cout << "Статус: " << status << endl;
        cout << "------------------------" << endl;
    }

    static void createAirportsFromFile(Airport* airports, int N) {
        ifstream file("port.txt");
        if (!file) {
            cout << "Ошибка: файл port.txt не найден!" << endl;
            return;
        }
        
        string name, location, status;
        int flightCount;
        
        for (int i = 0; i < N; i++) {
            if (file >> name >> location >> flightCount >> status) {
                airports[i] = Airport(name, location, flightCount, status);
            } else {
                cout << "Ошибка: недостаточно данных в файле!" << endl;
                break;
            }
        }
        file.close();
    }
};

class Flight : public Airport {
private:
    string flightNumber;
    int capacity;

public:
    Flight() : Airport() {
        flightNumber = "Неизвестно";
        capacity = 0;
    }

    Flight(string n, string loc, int count, string stat, string fNum, int cap) 
        : Airport(n, loc, count, stat) {
        flightNumber = fNum;
        capacity = cap;
    }

    void setFlightNumber(string fNum) { flightNumber = fNum; }
    void setCapacity(int cap) { capacity = cap; }

    string getFlightNumber() { return flightNumber; }
    int getCapacity() { return capacity; }

    void display() {
        Airport::display();
        cout << "Номер рейса: " << flightNumber << endl;
        cout << "Вместимость: " << capacity << " мест" << endl;
        cout << "==========================" << endl;
    }
};

int main() {
    cout << "Введите количество аэропортов (N): ";
    int N;
    cin >> N;

    Airport* airports = new Airport[N];
    
    Airport::createAirportsFromFile(airports, N);
    
    cout << "\nСозданные аэропорты:" << endl;
    for (int i = 0; i < N; i++) {
        cout << "Аэропорт " << i + 1 << ":" << endl;
        airports[i].display();
    }

    delete[] airports;
    return 0;
}