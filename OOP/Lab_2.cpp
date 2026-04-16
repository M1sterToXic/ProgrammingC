#include <iostream>
#include <cmath>
using namespace std;

class Triangle {
protected:
    double a, b, c; 
public:
    Triangle(double aa, double bb, double cc) {
        a = aa; b = bb; c = cc;
    }

    // Проверка существования
    bool exists() {
        return (a + b > c && a + c > b && b + c > a);
    }

    // Периметр
    double perimeter() {
        return a + b + c;
    }

    // Площадь (по формуле Герона)
    double area() {
        double p = perimeter() / 2.0;
        return sqrt(p * (p - a) * (p - b) * (p - c));
    }

    virtual void outputTo() {
        cout << "Треугольник: стороны = " << a << ", " << b << ", " << c << "\n";
        if (!exists()) {
            cout << "Треугольник не существует!\n";
            return;
        }
        cout << "Периметр = " << perimeter() << "\n";
        cout << "Площадь = " << area() << "\n";
    }
};

class IsoscelesTriangle : public Triangle {
public:
    IsoscelesTriangle(double aa, double bb, double cc) : Triangle(aa, bb, cc) {}

    // Проверка на равнобедренность
    bool isIsosceles() {
        return (a == b || a == c || b == c);
    }

    void outputTo() override {
        cout << "Равнобедренный треугольник: стороны = " << a << ", " << b << ", " << c << "\n";
        if (!exists()) {
            cout << "Треугольник не существует!\n";
            return;
        }
        if (isIsosceles())
            cout << "Да, этот треугольник равнобедренный.\n";
        else
            cout << "Ошибка: треугольник не равнобедренный!\n";

        cout << "Периметр = " << perimeter() << "\n";
        cout << "Площадь = " << area() << "\n";
    }
};

int main() {

    Triangle t1(3, 4, 5);
    Triangle t2(6, 7, 8);
    IsoscelesTriangle it1(5, 5, 6);
    IsoscelesTriangle it2(4, 4, 3);

    Triangle* arr[4] = { &t1, &t2, &it1, &it2 };

    cout << "=== Вывод информации о всех треугольниках ===\n";
    for (int i = 0; i < 4; i++) {
        arr[i]->outputTo();
        cout << "--------------------------\n";
    }

    // Средняя площадь для всех треугольников
    double sum = 0;
    int count = 0;
    for (int i = 0; i < 4; i++) {
        if (arr[i]->exists()) {
            sum += arr[i]->area();
            count++;
        }
    }
    cout << "Средняя площадь всех треугольников = " << (sum / count) << "\n";

    // Поиск равнобедренного с минимальной площадью
    IsoscelesTriangle* minIso = nullptr;
    for (int i = 0; i < 4; i++) {
        IsoscelesTriangle* iso = dynamic_cast<IsoscelesTriangle*>(arr[i]);
        if (iso && iso->exists() && iso->isIsosceles()) {
            if (!minIso || iso->area() < minIso->area()) {
                minIso = iso;
            }
        }
    }

    if (minIso) {
        cout << "\nРавнобедренный треугольник с минимальной площадью:\n";
        minIso->outputTo();
    }

    return 0;
}
