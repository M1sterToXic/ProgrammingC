#include <iostream>
#include <list>
#include <string>
using namespace std;

class HelpItem {
public:
    string term;
    string explanation;

    HelpItem() {
        term = "";
        explanation = "";
    }

    HelpItem(string t, string exp) {
        term = t;
        explanation = exp;
    }

    void show() {
        cout << "Term: " << term << endl;
        cout << "Explanation: " << explanation << endl;
        cout << "----------------------------------------" << endl;
    }
};

class HelpSystem {
private:
    list<HelpItem> items;

    bool termExists(string term) {
        for (auto it = items.begin(); it != items.end(); it++) {
            if (it->term == term) {
                return true;
            }
        }
        return false;
    }

public:
    void start() {
        if (items.empty()) {
            addTerm("program", "Set of instructions for computer execution");
            addTerm("algorithm", "Sequence of steps to solve a problem");
            addTerm("compiler", "Program that translates code into machine instructions");
            addTerm("function", "Block of code that performs a specific task");
            cout << "Help system created with initial terms." << endl;
        } else {
            cout << "Help system already contains terms." << endl;
        }
    }

    void addTerm(string term, string explanation) {
        if (termExists(term)) {
            cout << "Term '" << term << "' already exists!" << endl;
            return;
        }

        if (explanation.length() > 50) {
            explanation = explanation.substr(0, 50);
            cout << "Explanation truncated to 50 characters." << endl;
        }
        HelpItem newItem(term, explanation);
        items.push_back(newItem);
        cout << "Term '" << term << "' added." << endl;
    }

    void findTerm(string term) {
        bool found = false;
        for (auto it = items.begin(); it != items.end(); it++) {
            if (it->term == term) {
                cout << "Term found: " << endl;
                it->show();
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "Term '" << term << "' not found." << endl;
        }
    }
};

class Menu {
private:
    HelpSystem help;

    string getInput(string message) {
        string input;
        cout << message;
        getline(cin, input);
        return input;
    }

public:
    void showMenu() {
        int choice;
        
        do {
            cout << endl;
            cout << "=== HELP SYSTEM ===" << endl;
            cout << "1. Create help system" << endl;
            cout << "2. Add term" << endl;
            cout << "3. Find term explanation" << endl;
            cout << "0. Exit" << endl;
            cout << "Choose: ";
            cin >> choice;
            cin.ignore();
            
            switch (choice) {
                case 1:
                    createSystem();
                    break;
                case 2:
                    addTerm();
                    break;
                case 3:
                    findTerm();
                    break;
                case 0:
                    cout << "Exit from program." << endl;
                    break;
                default:
                    cout << "Invalid choice!" << endl;
            }
        } while (choice != 0);
    }

private:
    void createSystem() {
        help.start();
    }

    void addTerm() {
        string term = getInput("Enter term: ");
        if (term.empty()) {
            cout << "Term cannot be empty." << endl;
            return;
        }
        string explanation = getInput("Enter explanation: ");
        if (explanation.empty()) {
            cout << "Explanation cannot be empty." << endl;
            return;
        }
        help.addTerm(term, explanation);
    }

    void findTerm() {
        string term = getInput("Enter term to search: ");
        help.findTerm(term);
    }
};

int main() {
    Menu menu;
    menu.showMenu();
    return 0;
}