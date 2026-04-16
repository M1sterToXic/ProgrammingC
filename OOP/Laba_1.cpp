#include<iostream>
#include<string>
using namespace std;

class EntertainmentVenue {
private:
    string venueType;    
    int capacity;        
    string name;         

public:
    EntertainmentVenue(string type, int cap, string n) {
        venueType = type;
        capacity = cap;
        name = n;
    }

    void outputTo() {
        cout << "Название: " << name << endl;
        cout << "Тип учреждения: " << venueType << endl;
        cout << "Вместимость: " << capacity << " человек" << endl;
        cout << "----------------------------------------" << endl;
    }

    string getVenueType() {
        return venueType;
    }

    int getCapacity() {
        return capacity;
    }

    string getName() {
        return name;
    }

    void setVenueType(string type) {
        venueType = type;
    }

    void setCapacity(int cap) {
        capacity = cap;
    }

    void setName(string n) {
        name = n;
    }
};

int main() {
    EntertainmentVenue theater("Театр", 500, "Большой театр");
    EntertainmentVenue *exhibition = new EntertainmentVenue("Выставка", 200, "Галерея искусств");

    cout << "=== ИСХОДНЫЕ ДАННЫЕ ===" << endl;
    theater.outputTo();
    exhibition->outputTo();

    cout << "Тип первого учреждения: " << theater.getVenueType() << endl;
    cout << "Вместимость второго учреждения: " << exhibition->getCapacity() << " человек" << endl;
    cout << "Название первого учреждения: " << theater.getName() << endl << endl;

    // Работа методов изменения значений
    theater.setCapacity(550);
    exhibition->setVenueType("Музей-выставка");
    theater.setName("Государственный академический театр");

    cout << "=== ДАННЫЕ ПОСЛЕ ИЗМЕНЕНИЙ ===" << endl;
    theater.outputTo();
    exhibition->outputTo();

    // Дополнительные изменения
    exhibition->setCapacity(250);
    cout << "=== ФИНАЛЬНЫЕ ДАННЫЕ ===" << endl;
    theater.outputTo();
    exhibition->outputTo();

    delete exhibition;

    return 0;
}