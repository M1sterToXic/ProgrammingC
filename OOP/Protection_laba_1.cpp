#include <iostream>
#include <cmath>
using namespace std;

class Circle {
private:
    double x0; 
    double y0; 
    double R;  

public:
    Circle() : x0(0), y0(0), R(1) {}
    
    Circle(double x, double y, double radius) : x0(x), y0(y), R(radius) {
        if (R <= 0) {
            cout << "Ошибка: радиус должен быть положительным! Установлен радиус = 1" << endl;
            R = 1;
        }
    }

    double getLength() const {
        return 2 * M_PI * R;
    }
    
    bool intersectsOX() const {
        return abs(y0) <= R;
    }
    
    bool intersectsOY() const {
        return abs(x0) <= R;
    }
    
    void outputInfo() const {
        cout << "Окружность: центр(" << x0 << ", " << y0 << "), радиус = " << R << endl;
        cout << "Длина окружности: " << getLength() << endl;
        cout << "Пересекает OX: " << (intersectsOX() ? "да" : "нет") << endl;
        cout << "Пересекает OY: " << (intersectsOY() ? "да" : "нет") << endl;
        cout << "----------------------------------------" << endl;
    }
};

int main() {
    int N;
    cout << "Введите количество окружностей: ";
    cin >> N;
    
    Circle* circles = new Circle[N];
    int k1 = 0; 
    int k2 = 0; 

    for (int i = 0; i < N; i++) {
        cout << "\nОкружность " << i + 1 << ":" << endl;
        double x, y, radius;
        cout << "Введите координату X центра: ";
        cin >> x;
        cout << "Введите координату Y центра: ";
        cin >> y;
        cout << "Введите радиус: ";
        cin >> radius;
        
        circles[i] = Circle(x, y, radius);
    }

    cout << "\n=== РЕЗУЛЬТАТЫ ===" << endl;
    
    for (int i = 0; i < N; i++) {
        cout << "\nОкружность " << i + 1 << ":" << endl;
        circles[i].outputInfo();
        if (circles[i].intersectsOX()) {
            k1++;
        }
        if (circles[i].intersectsOY()) {
            k2++;
        }
    }

    cout << "\n=== СВОДКА ===" << endl;
    for (int i = 0; i < N; i++) {
        cout << "Окружность " << i + 1 << ": ";
        cout << "длина = " << circles[i].getLength() << ", ";
        cout << "OX: " << (circles[i].intersectsOX() ? "да" : "нет") << ", ";
        cout << "OY: " << (circles[i].intersectsOY() ? "да" : "нет") << endl;
    }

    cout << "\nПересекают OX: " << k1 << " окружностей" << endl;
    cout << "Пересекают OY: " << k2 << " окружностей" << endl;

    delete[] circles;
    
    return 0;
}