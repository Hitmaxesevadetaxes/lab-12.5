#define _crt_secure_no_warnings
#include <iostream>
#include <fstream>
#include <cstring>
#include <Windows.h>

struct Worker {
    char surname[50];
    double salary;
};

struct Position {
    char title[50];
    int vacancies;
    char requirements[200];
};

struct Company {
    char name[50];
    Position position;
    Worker workers[10]; // Фіксована кількість працівників для простоти
    int numWorkers;
};

struct PositionSummary {
    char title[50];
    double avgSalary;
};
bool fileExists(const char* filename) {
    std::ifstream file(filename);
    return file.is_open();
}

int writeToFile(const char* filename, Company* companies, int size) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Помилка відкриття файлу для запису.\n";
        return;
    }
    file.write((char*)companies, sizeof(Company) * size);
    file.close();
    std::cout << "Дані записано у файл.\n";
}

int readFromFile(const char* filename, Company*& companies) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Помилка відкриття файлу для читання.\n";
        return 0;
    }

    file.seekg(0, std::ios::end);
    int size = file.tellg() / sizeof(Company);
    file.seekg(0, std::ios::beg);

    companies = new Company[size];
    file.read((char*)companies, sizeof(Company) * size);

    if (file.fail()) {
        std::cerr << "Помилка читання файлу.\n";
        delete[] companies;
        companies = nullptr;
        return 0;
    }

    file.close();
    return size;
}

void addCompany(const char* filename) {
    Company newCompany;
    std::cout << "Введіть назву компанії: ";
    std::cin.getline(newCompany.name, 50);

    std::cout << "Введіть назву посади: ";
    std::cin.getline(newCompany.position.title, 50);

    std::cout << "Введіть кількість вакансій: ";
    std::cin >> newCompany.position.vacancies;
    std::cin.ignore();

    std::cout << "Введіть вимоги: ";
    std::cin.getline(newCompany.position.requirements, 200);

    std::cout << "Введіть кількість працівників: ";
    std::cin >> newCompany.numWorkers;
    std::cin.ignore();

    for (int i = 0; i < newCompany.numWorkers; ++i) {
        std::cout << "Введіть прізвище працівника: ";
        std::cin.getline(newCompany.workers[i].surname, 50);

        std::cout << "Введіть зарплату працівника: ";
        std::cin >> newCompany.workers[i].salary;
        std::cin.ignore();
    }

    Company* companies = nullptr;
    int size = readFromFile(filename, companies);

    // Збільшуємо розмір масиву компаній
    Company* updatedCompanies = new Company[size + 1];
    for (int i = 0; i < size; ++i) {
        updatedCompanies[i] = companies[i];
    }
    updatedCompanies[size] = newCompany;

    writeToFile(filename, updatedCompanies, size + 1);

    delete[] companies;
    delete[] updatedCompanies;
    std::cout << "Компанія успішно додана.\n";
}

void appendToFile(const char* filename, const Company* newCompanies, int newSize) {
    std::ofstream file(filename, std::ios::binary | std::ios::app);
    if (!file) {
        std::cerr << "Помилка відкриття файлу для доповнення.\n";
        return;
    }
    file.write((char*)newCompanies, sizeof(Company) * newSize);
    file.close();
    std::cout << "Дані успішно доповнено у файл.\n";
}

void deleteFromFile(const char* filename, const char* companyName) {
    Company* companies = nullptr;
    int size = readFromFile(filename, companies);

    Company updatedCompanies[100]; // Припустимо максимум 100 компаній
    int updatedCount = 0;

    for (int i = 0; i < size; ++i) {
        if (strcmp(companies[i].name, companyName) != 0) {
            updatedCompanies[updatedCount] = companies[i];
            updatedCount++;
        }
    }

    // Записуємо оновлений список назад у файл
    writeToFile(filename, updatedCompanies, updatedCount);

    delete[] companies;

    std::cout << "Компанію \"" << companyName << "\" видалено (якщо існувала).\n";
}

int main() {
    const char* filename = "companies.dat";
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Company companies[2] = {
        {"Компанія A", {"Програміст", 5, "Знання C++"}, {{"Іваненко", 1500}}, 1},
        {"Компанія B", {"Програміст", 10, "Досвід роботи 2 роки"}, {{"Петренко", 2000}}, 1}
    };

    writeToFile(filename, companies, 2);

    while (true) {
        std::cout << "\nМеню:\n";
        std::cout << "1. Визначити фірму з найбільшою кількістю вакансій\n";
        std::cout << "2. Вивести вимоги до кандидатів\n";
        std::cout << "3. Створити новий масив унікальних вакансій\n";
        std::cout << "4. Доповнити файл\n";
        std::cout << "5. Видалити компанію\n";
        std::cout << "6. Додати нову компанію\n";
        std::cout << "7. Вихід\n";
        std::cout << "Ваш вибір: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            char position[50];
            std::cout << "Введіть назву посади: ";
            std::cin.getline(position, 50);
            findMaxVacancies(filename, position);
        }
        else if (choice == 2) {
            char position[50];
            std::cout << "Введіть назву посади: ";
            std::cin.getline(position, 50);
            printRequirements(filename, position);
        }
        else if (choice == 3) {
            createPositionSummary(filename);
        }
        else if (choice == 4) {
            Company newCompanies[1] = {
                {"Компанія C", {"Аналітик", 7, "Аналітичне мислення"}, {{"Сидоренко", 1800}}, 1}
            };
            appendToFile(filename, newCompanies, 1);
        }
        else if (choice == 5) {
            char companyName[50];
            std::cout << "Введіть назву компанії: ";
            std::cin.getline(companyName, 50);
            deleteFromFile(filename, companyName);
        }
        else if (choice == 6) {
            addCompany(filename);
        }
        else if (choice == 7) {
            break;
        }
        else {
            std::cout << "Невірний вибір. Спробуйте ще раз.\n";
        }
    }

    return 0;
}
