#include <iostream>
#include <stack>
#include <vector>

using namespace std;

class StackProcessor {
private:
    stack<int> data;
    
public:
    void push(int value) {
        data.push(value);
    }
    
    bool empty() const {
        return data.empty();
    }
    
    size_t size() const {
        return data.size();
    }
    
    double calculateAverage() const {
        if (data.empty()) return 0.0;
        
        stack<int> temp = data;
        int sum = 0;
        int count = 0;
        
        while (!temp.empty()) {
            sum += temp.top();
            temp.pop();
            count++;
        }
        
        return static_cast<double>(sum) / count;
    }
    
    StackProcessor removeAboveAverage(double average) const {
        StackProcessor result;
        stack<int> temp = data;
        
        stack<int> reversed;
        while (!temp.empty()) {
            reversed.push(temp.top());
            temp.pop();
        }
        
        while (!reversed.empty()) {
            int current = reversed.top();
            if (current <= average) {
                result.push(current);
            }
            reversed.pop();
        }
        
        return result;
    }
    

    void print() const {
        if (data.empty()) {
            cout << "Стек пуст!";
            return;
        }
        
        stack<int> temp = data;
        
        while (!temp.empty()) {
            cout << temp.top() << " ";
            temp.pop();
        }
    }
};

class InputHandler {
public:
    static StackProcessor inputNumbers() {
        StackProcessor stack;
        int number;
        
        cout << "Введите целые числа (для завершения введите 0):" << endl;
        
        while (true) {
            cin >> number;
            if (number == 0) {
                break;
            }
            stack.push(number);
        }
        
        return stack;
    }
};

class Program {
public:
    void run() {
        StackProcessor originalStack = InputHandler::inputNumbers();
        
        if (originalStack.empty()) {
            cout << "Стек пуст!" << endl;
            return;
        }
        
        double average = originalStack.calculateAverage();
        
        cout << "Исходный стек: ";
        originalStack.print();
        cout << endl;
        
        cout << "Среднее арифметическое: " << average << endl;
        
        StackProcessor filteredStack = originalStack.removeAboveAverage(average);
        
        cout << "Стек после удаления чисел > " << average << ": ";
        filteredStack.print();
        cout << endl;
    }
};

int main() {
    Program program;
    program.run();
    return 0;
}