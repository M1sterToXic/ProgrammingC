#include <iostream>
#include <stack>
using namespace std;

class NumberStack {
private:
    stack<int> mainStack;
    stack<int> evenStack;

public:
    void addNumber(int number) {
        mainStack.push(number);
        cout << "Число " << number << " добавлено в стек" << endl;
    }

    void createEvenStack() {
        stack<int> tempStack = mainStack;
        
        while (!evenStack.empty()) {
            evenStack.pop();
        }
        
        while (!tempStack.empty()) {
            int num = tempStack.top();
            if (num % 2 == 0) {
                evenStack.push(num);
            }
            tempStack.pop();
        }
        
        cout << "Стек четных чисел создан" << endl;
    }

    void displayMainStack() {
        if (mainStack.empty()) {
            cout << "Основной стек пуст!" << endl;
            return;
        }
        
        cout << "=== ОСНОВНОЙ СТЕК ===" << endl;
        stack<int> tempStack = mainStack;
        int counter = 1;
        
        while (!tempStack.empty()) {
            cout << counter << ". " << tempStack.top() << endl;
            tempStack.pop();
            counter++;
        }
    }

    void displayEvenStack() {
        if (evenStack.empty()) {
            cout << "Стек четных чисел пуст!" << endl;
            return;
        }
        
        cout << "=== СТЕК ЧЕТНЫХ ЧИСЕЛ ===" << endl;
        stack<int> tempStack = evenStack;
        int counter = 1;
        
        while (!tempStack.empty()) {
            cout << counter << ". " << tempStack.top() << endl;
            tempStack.pop();
            counter++;
        }
    }

    bool isMainStackEmpty() {
        return mainStack.empty();
    }
};

class Menu {
private:
    NumberStack stackManager;

    void displayMenu() {
        cout << "\n=== МЕНЮ ===" << endl;
        cout << "1. Добавить число в стек" << endl;
        cout << "2. Показать основной стек" << endl;
        cout << "3. Создать стек четных чисел" << endl;
        cout << "4. Показать стек четных чисел" << endl;
        cout << "5. Выполнить все операции (добавить числа -> показать -> создать четные -> показать)" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите действие: ";
    }

    void handleAddNumber() {
        int number;
        cout << "Введите целое число: ";
        cin >> number;
        stackManager.addNumber(number);
    }

    void handleCreateEvenStack() {
        if (stackManager.isMainStackEmpty()) {
            cout << "Основной стек пуст! Сначала добавьте числа." << endl;
            return;
        }
        stackManager.createEvenStack();
    }

    void handleAllOperations() {
        cout << "\n--- Добавление чисел в стек ---" << endl;
        int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        for (int i = 0; i < 10; i++) {
            stackManager.addNumber(numbers[i]);
        }
        
        cout << "\n--- Показ основного стека ---" << endl;
        stackManager.displayMainStack();
        
        cout << "\n--- Создание стека четных чисел ---" << endl;
        stackManager.createEvenStack();
        
        cout << "\n--- Показ стека четных чисел ---" << endl;
        stackManager.displayEvenStack();
    }

public:
    void run() {
        int choice;
        
        do {
            displayMenu();
            cin >> choice;
            
            switch(choice) {
                case 1:
                    handleAddNumber();
                    break;
                    
                case 2:
                    stackManager.displayMainStack();
                    break;
                    
                case 3:
                    handleCreateEvenStack();
                    break;
                    
                case 4:
                    stackManager.displayEvenStack();
                    break;
                    
                case 5:
                    handleAllOperations();
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