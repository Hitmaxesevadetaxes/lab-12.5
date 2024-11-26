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
    Worker workers[10]; // Գ������� ������� ���������� ��� ��������
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
        std::cerr << "������� �������� ����� ��� ������.\n";
        return;
    }
    file.write((char*)companies, sizeof(Company) * size);
    file.close();
    std::cout << "��� �������� � ����.\n";
}

int readFromFile(const char* filename, Company*& companies) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "������� �������� ����� ��� �������.\n";
        return 0;
    }

    file.seekg(0, std::ios::end);
    int size = file.tellg() / sizeof(Company);
    file.seekg(0, std::ios::beg);

    companies = new Company[size];
    file.read((char*)companies, sizeof(Company) * size);

    if (file.fail()) {
        std::cerr << "������� ������� �����.\n";
        delete[] companies;
        companies = nullptr;
        return 0;
    }

    file.close();
    return size;
}

void addCompany(const char* filename) {
    Company newCompany;
    std::cout << "������ ����� ������: ";
    std::cin.getline(newCompany.name, 50);

    std::cout << "������ ����� ������: ";
    std::cin.getline(newCompany.position.title, 50);

    std::cout << "������ ������� �������: ";
    std::cin >> newCompany.position.vacancies;
    std::cin.ignore();

    std::cout << "������ ������: ";
    std::cin.getline(newCompany.position.requirements, 200);

    std::cout << "������ ������� ����������: ";
    std::cin >> newCompany.numWorkers;
    std::cin.ignore();

    for (int i = 0; i < newCompany.numWorkers; ++i) {
        std::cout << "������ ������� ����������: ";
        std::cin.getline(newCompany.workers[i].surname, 50);

        std::cout << "������ �������� ����������: ";
        std::cin >> newCompany.workers[i].salary;
        std::cin.ignore();
    }

    Company* companies = nullptr;
    int size = readFromFile(filename, companies);

    // �������� ����� ������ �������
    Company* updatedCompanies = new Company[size + 1];
    for (int i = 0; i < size; ++i) {
        updatedCompanies[i] = companies[i];
    }
    updatedCompanies[size] = newCompany;

    writeToFile(filename, updatedCompanies, size + 1);

    delete[] companies;
    delete[] updatedCompanies;
    std::cout << "������� ������ ������.\n";
}

void appendToFile(const char* filename, const Company* newCompanies, int newSize) {
    std::ofstream file(filename, std::ios::binary | std::ios::app);
    if (!file) {
        std::cerr << "������� �������� ����� ��� ����������.\n";
        return;
    }
    file.write((char*)newCompanies, sizeof(Company) * newSize);
    file.close();
    std::cout << "��� ������ ��������� � ����.\n";
}

void deleteFromFile(const char* filename, const char* companyName) {
    Company* companies = nullptr;
    int size = readFromFile(filename, companies);

    Company updatedCompanies[100]; // ���������� �������� 100 �������
    int updatedCount = 0;

    for (int i = 0; i < size; ++i) {
        if (strcmp(companies[i].name, companyName) != 0) {
            updatedCompanies[updatedCount] = companies[i];
            updatedCount++;
        }
    }

    // �������� ��������� ������ ����� � ����
    writeToFile(filename, updatedCompanies, updatedCount);

    delete[] companies;

    std::cout << "������� \"" << companyName << "\" �������� (���� ��������).\n";
}

int main() {
    const char* filename = "companies.dat";
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Company companies[2] = {
        {"������� A", {"���������", 5, "������ C++"}, {{"��������", 1500}}, 1},
        {"������� B", {"���������", 10, "����� ������ 2 ����"}, {{"��������", 2000}}, 1}
    };

    writeToFile(filename, companies, 2);

    while (true) {
        std::cout << "\n����:\n";
        std::cout << "1. ��������� ����� � ��������� ������� �������\n";
        std::cout << "2. ������� ������ �� ���������\n";
        std::cout << "3. �������� ����� ����� ��������� �������\n";
        std::cout << "4. ��������� ����\n";
        std::cout << "5. �������� �������\n";
        std::cout << "6. ������ ���� �������\n";
        std::cout << "7. �����\n";
        std::cout << "��� ����: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            char position[50];
            std::cout << "������ ����� ������: ";
            std::cin.getline(position, 50);
            findMaxVacancies(filename, position);
        }
        else if (choice == 2) {
            char position[50];
            std::cout << "������ ����� ������: ";
            std::cin.getline(position, 50);
            printRequirements(filename, position);
        }
        else if (choice == 3) {
            createPositionSummary(filename);
        }
        else if (choice == 4) {
            Company newCompanies[1] = {
                {"������� C", {"�������", 7, "��������� ��������"}, {{"���������", 1800}}, 1}
            };
            appendToFile(filename, newCompanies, 1);
        }
        else if (choice == 5) {
            char companyName[50];
            std::cout << "������ ����� ������: ";
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
            std::cout << "������� ����. ��������� �� ���.\n";
        }
    }

    return 0;
}
