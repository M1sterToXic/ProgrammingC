#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>

using namespace std;

class Bank {
private:
    string name;
    double creditAmount;
    double interestRate;

public:
    Bank() : creditAmount(0), interestRate(0) {}
    
    Bank(string n, double amount, double rate) {
        name = n;
        creditAmount = amount;
        interestRate = rate;
    }

    double getAnnualInterest() const {
        return creditAmount * interestRate / 100.0;
    }

    double getCreditAmount() const {
        return creditAmount;
    }

    void display() const {
        cout << name << " - Кредиты: " << creditAmount << " млн, Ставка: " 
             << interestRate << "%, Проценты: " << getAnnualInterest() << " млн" << endl;
    }

    void writeToFile(ofstream& file) const {
        file << name << " " << creditAmount << " " << interestRate << endl;
    }

    void readFromFile(ifstream& file) {
        file >> name >> creditAmount >> interestRate;
    }
};

class BankList {
private:
    list<Bank> banks;
    string filename;

public:
    BankList(string file) {
        filename = file;
    }

    void loadFromFile(int count) {
        ifstream file(filename);
        if (!file) {
            cout << "Файл не найден!" << endl;
            return;
        }

        banks.clear();
        for (int i = 0; i < count; i++) {
            Bank bank;
            bank.readFromFile(file);
            if (file) {
                banks.push_back(bank);
            }
        }
        file.close();
    }

    void displayAll() {
        cout << "\nВсе банки:" << endl;
        for (const auto& bank : banks) {
            bank.display();
        }
    }

    list<Bank> getRichBanks() {
        list<Bank> richBanks;
        
        for (const auto& bank : banks) {
            if (bank.getCreditAmount() > 100) {
                richBanks.push_back(bank);
            }
        }
        
        richBanks.sort([](const Bank& a, const Bank& b) {
            return a.getCreditAmount() < b.getCreditAmount();
        });
        
        return richBanks;
    }

    void displayList(const list<Bank>& bankList, string title) {
        cout << "\n" << title << ":" << endl;
        if (bankList.empty()) {
            cout << "Нет банков" << endl;
        } else {
            for (const auto& bank : bankList) {
                bank.display();
            }
        }
    }

    void addBank(const Bank& newBank) {
        banks.push_back(newBank);
        
        ofstream file(filename, ios::app);
        if (file) {
            file << endl; 
            newBank.writeToFile(file);
            file.close();
            cout << "Банк добавлен в файл" << endl;
        }
    }

    Bank createBank() {
        string name;
        double amount, rate;

        cout << "Введите название банка: ";
        cin.ignore();
        getline(cin, name);
        
        cout << "Введите сумму кредитов (млн): ";
        cin >> amount;
        
        cout << "Введите процентную ставку: ";
        cin >> rate;

        return Bank(name, amount, rate);
    }
};

int main() {
    int N;
    cout << "Сколько банков загрузить из файла? ";
    cin >> N;

    BankList bankManager("bank.txt");
    bankManager.loadFromFile(N);
    bankManager.displayAll();

    list<Bank> richBanks = bankManager.getRichBanks();
    bankManager.displayList(richBanks, "Банки с кредитами > 100 млн");

    char choice;
    cout << "\nДобавить новый банк? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        Bank newBank = bankManager.createBank();
        bankManager.addBank(newBank);

        if (newBank.getCreditAmount() > 100) {
            richBanks = bankManager.getRichBanks();
            bankManager.displayList(richBanks, "Обновленный список богатых банков");
        }
    }

    return 0;
}