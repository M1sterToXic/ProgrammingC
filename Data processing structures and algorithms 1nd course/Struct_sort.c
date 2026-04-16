#include <stdio.h>
#include <string.h>

#define NAME_LEN 50
#define ADDR_LEN 100
#define MAX_CONTACTS 20

struct Contact {
    char name[NAME_LEN];
    char lastname[NAME_LEN];
    char address[ADDR_LEN];
    int number;
};

int less_00(struct Contact a, struct Contact b) {
    int res = strcmp(b.lastname, a.lastname);
    return res ? res : strcmp(b.name, a.name);
}

int less_01(struct Contact a, struct Contact b) {
    int res = strcmp(a.lastname, b.lastname);
    return res ? res : strcmp(a.name, b.name);
}

int less_10(struct Contact a, struct Contact b) {
    int res = strcmp(b.address, a.address);
    return res ? res : b.number - a.number;
}

int less_11(struct Contact a, struct Contact b) {
    int res = strcmp(a.address, b.address);
    return res ? res : a.number - b.number;
}

void InsertSort(struct Contact contacts[], int size, int (*compare)(struct Contact, struct Contact)) {
    for (int i = 1; i < size; i++) {
        struct Contact temp = contacts[i];
        int j = i - 1;

        while (j >= 0 && compare(contacts[j], temp) > 0) {
            contacts[j + 1] = contacts[j];
            j--;
        }

        contacts[j + 1] = temp;
    }
}

void printTableHeader() {
    printf("+------------------------+------------------------+------------------------------+------------+\n");
    printf("| %-16s | %-15s | %-23s | %-5s |\n", "Фамилия", "Имя", "Адрес", "Телефон");
    printf("+------------------------+------------------------+------------------------------+------------+\n");
}

void printContactRow(struct Contact c) {
    printf("| %-16s | %-15s | %-24s | %-5d |\n", 
           c.lastname, c.name, c.address, c.number);
}

void printTableFooter() {
    printf("+------------------------+------------------------+------------------------------+------------+\n\n");
}

void printContacts(struct Contact contacts[], int size) {
    printTableHeader();
    for (int i = 0; i < size; i++) {
        printContactRow(contacts[i]);
    }
    printTableFooter();
}

int main() {
    struct Contact contacts[MAX_CONTACTS] = {
        {"Влад", "Погорелов", "ул. Ленина, 10", 893213},
        {"Никита", "Криволапов", "пр. Мира, 5", 123513},
        {"Артем", "Добромилов", "ул. Центральная, 3", 763421},
        {"Рома", "Петров", "пр. Мира, 5", 345635},
        {"Кирилл", "Крахмальный", "ул. Садовая, 15", 987123}
    };

    int size = 5;
    int mode;

    printf("Исходный справочник:\n");
    printContacts(contacts, size);

    printf("Выберите режим сортировки:\n");
    printf("00 - Фамилия+Имя по убыванию\n");
    printf("01 - Фамилия+Имя по возрастанию\n");
    printf("10 - Адрес+Номер по убыванию\n");
    printf("11 - Адрес+Номер по возрастанию\n");
    printf("Введите код режима (00-11): ");
    scanf("%d", &mode);

    switch(mode) {
        case 0:  InsertSort(contacts, size, less_00); break;
        case 1:  InsertSort(contacts, size, less_01); break;
        case 10: InsertSort(contacts, size, less_10); break;
        case 11: InsertSort(contacts, size, less_11); break;
        default: printf("Неверный режим сортировки!\n"); return 1;
    }

    printf("\nОтсортированный справочник:\n");
    printContacts(contacts, size);

    return 0;
}