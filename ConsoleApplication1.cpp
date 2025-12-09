#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <locale>
#include <limits>
#include <clocale>
#include <memory>
#define NOMINMAX
#include <windows.h>
#include "Employee.h"
#include "EmployeeDatabase.h"

using namespace std;

EmployeeDatabase db;

void clearInputLine() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void displayMainMenu() {
    cout << "\n========== ГЛАВНОЕ МЕНЮ ==========" << "\n";
    cout << "1. Управление сотрудниками\n";
    cout << "2. Сортировка\n";
    cout << "3. Фильтрация\n";
    cout << "4. Поиск\n";
    cout << "5. Агрегирующие запросы\n";
    cout << "6. Просмотр всех сотрудников\n";
    cout << "0. Выход\n";
    cout << "==================================\n";
    cout << "Выбор: ";
}

void displayManagementMenu() {
    cout << "\n===== УПРАВЛЕНИЕ СОТРУДНИКАМИ =====\n";
    cout << "1. Добавить сотрудника\n";
    cout << "2. Удалить сотрудника\n";
    cout << "3. Редактировать сотрудника\n";
    cout << "4. Просмотреть сотрудника\n";
    cout << "0. Назад в главное меню\n";
    cout << "====================================\n";
    cout << "Выбор: ";
}

void displaySortMenu() {
    cout << "\n========== СОРТИРОВКА ==========" << "\n";
    cout << "1. По ФИО (А-Я)\n";
    cout << "2. По зарплате (убывание)\n";
    cout << "3. По цеху\n";
    cout << "4. По году рождения\n";
    cout << "5. По дате поступления\n";
    cout << "0. Назад в главное меню\n";
    cout << "================================\n";
    cout << "Выбор: ";
}

void displayFilterMenu() {
    cout << "\n========== ФИЛЬТРАЦИЯ ==========" << "\n";
    cout << "1. По цеху\n";
    cout << "2. По семейному положению\n";
    cout << "3. По полу\n";
    cout << "4. По количеству детей\n";
    cout << "5. По диапазону зарплаты\n";
    cout << "0. Назад в главное меню\n";
    cout << "================================\n";
    cout << "Выбор: ";
}

void displaySearchMenu() {
    cout << "\n========== ПОИСК ==========" << "\n";
    cout << "1. По ФИО\n";
    cout << "2. По цеху\n";
    cout << "0. Назад в главное меню\n";
    cout << "==========================\n";
    cout << "Выбор: ";
}

void displayAggregateMenu() {
    cout << "\n===== АГРЕГИРУЮЩИЕ ЗАПРОСЫ =====\n";
    cout << "1. Общая зарплата всех сотрудников\n";
    cout << "2. Средняя зарплата\n";
    cout << "3. Максимальная зарплата\n";
    cout << "4. Минимальная зарплата\n";
    cout << "5. Количество сотрудников\n";
    cout << "6. Средняя зарплата по цеху\n";
    cout << "7. Количество сотрудников по цеху\n";
    cout << "8. Средний возраст\n";
    cout << "9. Среднее количество детей\n";
    cout << "0. Назад в главное меню\n";
    cout << "=================================\n";
    cout << "Выбор: ";
}

void addEmployee() {
    string name, workshop, hireDate, maritalStatus, illnessDate, recoveryDate;
    double salary, bulletinPayPercent, averageEarnings;
    int birthYear, childrenCount;
    char gender;

    cout << "\n=== Добавление нового сотрудника ===\n";
    cout << "ФИО: ";
    cin.ignore(10000, '\n');
    getline(cin, name);

    cout << "Цех: ";
    getline(cin, workshop);

    cout << "Зарплата: ";
    cin >> salary;
    cin.ignore(10000, '\n');

    cout << "Год рождения: ";
    cin >> birthYear;
    cin.ignore(10000, '\n');

    cout << "Дата поступления (ДД.ММ.ГГГГ): ";
    getline(cin, hireDate);

    cout << "Семейное положение: ";
    getline(cin, maritalStatus);

    cout << "Пол (М/Ж): ";
    cin >> gender;
    cin.ignore(10000, '\n');

    cout << "Количество детей: ";
    cin >> childrenCount;
    cin.ignore(10000, '\n');

    cout << "Дата заболевания (ДД.ММ.ГГГГ): ";
    getline(cin, illnessDate);

    cout << "Дата выздоровления (ДД.ММ.ГГГГ): ";
    getline(cin, recoveryDate);

    cout << "Оплата по бюллетеню (%): ";
    cin >> bulletinPayPercent;
    cin.ignore(10000, '\n');

    cout << "Средний заработок: ";
    cin >> averageEarnings;
    cin.ignore(10000, '\n');

    Employee emp(name, workshop, salary, birthYear, hireDate, maritalStatus,
                 gender, childrenCount, illnessDate, recoveryDate, bulletinPayPercent, averageEarnings);
    db.addEmployee(emp);
    cout << "Сотрудник успешно добавлен!\n";
}

void removeEmployee() {
    if (db.isEmpty()) {
        cout << "База данных пуста!\n";
        return;
    }

    db.displayAll();
    cout << "Введите номер сотрудника для удаления: ";
    int index;
    cin >> index;
    clearInputLine();
    index--;

    if (index >= 0 && index < db.getTotalEmployees() && db.removeEmployee(index)) {
        cout << "Сотрудник успешно удален!\n";
    } else {
        cout << "Некорректный номер!\n";
    }
}

void editEmployee() {
    if (db.isEmpty()) {
        cout << "База данных пуста!\n";
        return;
    }

    db.displayAll();
    cout << "Введите номер сотрудника для редактирования: ";
    int index;
    cin >> index;
    clearInputLine();
    index--;

    if (index >= 0 && index < db.getTotalEmployees()) {
        if (!db.editEmployee(index)) {
            cout << "Редактирование не выполнено.\n";
        }
    } else {
        cout << "Некорректный номер!\n";
    }
}

void viewEmployee() {
    if (db.isEmpty()) {
        cout << "База данных пуста!\n";
        return;
    }

    db.displayAll();
    cout << "Введите номер сотрудника для просмотра: ";
    int index;
    cin >> index;
    clearInputLine();
    index--;

    if (index >= 0 && index < db.getTotalEmployees()) {
        db.displayEmployee(index);
    } else {
        cout << "Некорректный номер!\n";
    }
}

void managementMenu() {
    int choice;
    while (true) {
        displayManagementMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                addEmployee();
                break;
            case 2:
                removeEmployee();
                break;
            case 3:
                editEmployee();
                break;
            case 4:
                viewEmployee();
                break;
            case 0:
                return;
            default:
                cout << "Некорректный выбор!\n";
        }
    }
}

void sortMenu() {
    int choice;
    while (true) {
        displaySortMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                db.sortByName();
                db.displayAll();
                break;
            case 2:
                db.sortBySalary();
                db.displayAll();
                break;
            case 3:
                db.sortByWorkshop();
                db.displayAll();
                break;
            case 4:
                db.sortByBirthYear();
                db.displayAll();
                break;
            case 5:
                db.sortByHireDate();
                db.displayAll();
                break;
            case 0:
                return;
            default:
                cout << "Некорректный выбор!\n";
        }
    }
}

void filterMenu() {
    int choice;
    while (true) {
        displayFilterMenu();
        cin >> choice;
        clearInputLine();

        switch (choice) {
            case 1: {
                string workshop;
                cout << "Введите цех: ";
                getline(cin, workshop);
                auto indices = db.filterByWorkshop(workshop);
                if (indices.empty()) {
                    cout << "Сотрудников в этом цехе не найдено!\n";
                } else {
                    cout << "\n=== Результаты фильтрации ===\n";
                    for (int idx : indices) {
                        db.displayEmployee(idx);
                    }
                }
                break;
            }
            case 2: {
                string status;
                cout << "Введите семейное положение: ";
                getline(cin, status);
                auto indices = db.filterByMaritalStatus(status);
                if (indices.empty()) {
                    cout << "Сотрудников с таким статусом не найдено!\n";
                } else {
                    cout << "\n=== Результаты фильтрации ===\n";
                    for (int idx : indices) {
                        db.displayEmployee(idx);
                    }
                }
                break;
            }
            case 3: {
                string genderInput;
                cout << "Введите пол (М/Ж): ";
                getline(cin, genderInput);
                char normalized = Employee::normalizeGenderInput(genderInput);
                if (normalized == ' ') {
                    cout << "Некорректное значение пола!\n";
                    break;
                }
                auto indices = db.filterByGender(normalized);
                if (indices.empty()) {
                    cout << "Сотрудников с таким полом не найдено!\n";
                } else {
                    cout << "\n=== Результаты фильтрации ===\n";
                    for (int idx : indices) {
                        db.displayEmployee(idx);
                    }
                }
                break;
            }
            case 4: {
                int count;
                cout << "Введите количество детей: ";
                cin >> count;
                clearInputLine();
                auto indices = db.filterByChildrenCount(count);
                if (indices.empty()) {
                    cout << "Сотрудников с таким количеством детей не найдено!\n";
                } else {
                    cout << "\n=== Результаты фильтрации ===\n";
                    for (int idx : indices) {
                        db.displayEmployee(idx);
                    }
                }
                break;
            }
            case 5: {
                double minSal, maxSal;
                cout << "Введите минимальную зарплату: ";
                cin >> minSal;
                cout << "Введите максимальную зарплату: ";
                cin >> maxSal;
                clearInputLine();
                auto indices = db.filterBySalaryRange(minSal, maxSal);
                if (indices.empty()) {
                    cout << "Сотрудников в этом диапазоне зарплаты не найдено!\n";
                } else {
                    cout << "\n=== Результаты фильтрации ===\n";
                    for (int idx : indices) {
                        db.displayEmployee(idx);
                    }
                }
                break;
            }
            case 0:
                return;
            default:
                cout << "Некорректный выбор!\n";
        }
    }
}

void searchMenu() {
    int choice;
    while (true) {
        displaySearchMenu();
        cin >> choice;
        clearInputLine();

        switch (choice) {
            case 1: {
                string name;
                cout << "Введите ФИО: ";
                getline(cin, name);
                int idx = db.findByName(name);
                if (idx == -1) {
                    cout << "Сотрудник не найден!\n";
                } else {
                    cout << "\n=== Результат поиска ===\n";
                    db.displayEmployee(idx);
                }
                break;
            }
            case 2: {
                string workshop;
                cout << "Введите цех: ";
                getline(cin, workshop);
                auto indices = db.findByWorkshop(workshop);
                if (indices.empty()) {
                    cout << "Сотрудников в этом цехе не найдено!\n";
                } else {
                    cout << "\n=== Результаты поиска ===\n";
                    for (int idx : indices) {
                        db.displayEmployee(idx);
                    }
                }
                break;
            }
            case 0:
                return;
            default:
                cout << "Некорректный выбор!\n";
        }
    }
}

void aggregateMenu() {
    int choice;
    while (true) {
        displayAggregateMenu();
        cin >> choice;
        clearInputLine();

        switch (choice) {
            case 1:
                cout << "Общая зарплата всех сотрудников: " << db.getTotalSalary() << " руб.\n";
                break;
            case 2:
                cout << "Средняя зарплата: " << db.getAverageSalary() << " руб.\n";
                break;
            case 3:
                cout << "Максимальная зарплата: " << db.getMaxSalary() << " руб.\n";
                break;
            case 4:
                cout << "Минимальная зарплата: " << db.getMinSalary() << " руб.\n";
                break;
            case 5:
                cout << "Количество сотрудников: " << db.getTotalEmployees() << "\n";
                break;
            case 6: {
                string workshop;
                cout << "Введите цех: ";
                getline(cin, workshop);
                cout << "Средняя зарплата в цехе " << workshop << ": " 
                     << db.getAverageSalaryByWorkshop(workshop) << " руб.\n";
                break;
            }
            case 7: {
                string workshop;
                cout << "Введите цех: ";
                getline(cin, workshop);
                cout << "Количество сотрудников в цехе " << workshop << ": " 
                     << db.getEmployeeCountByWorkshop(workshop) << "\n";
                break;
            }
            case 8: {
                int totalEmployees = db.getTotalEmployees();
                if (totalEmployees == 0) {
                    cout << "База данных пуста!\n";
                } else {
                    int totalAge = 0;
                    for (int i = 0; i < totalEmployees; ++i) {
                        totalAge += (2025 - db.getEmployeeConst(i).getBirthYear());
                    }
                    cout << "Средний возраст: " << (totalAge / totalEmployees) << " лет\n";
                }
                break;
            }
            case 9:
                cout << "Среднее количество детей: " << db.getAverageChildrenCount() << "\n";
                break;
            case 0:
                return;
            default:
                cout << "Некорректный выбор!\n";
        }
    }
}

int main() {
    // Установка кодовой страницы консоли на UTF-8 (для Windows)
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    
    // Установка локали для работы с русским текстом
    setlocale(LC_ALL, ".UTF8");
    
    int choice;

    cout << "========================================\n";
    cout << "  АСУ 'Заработная плата сотрудников'\n";
    cout << "========================================\n";

    while (true) {
        displayMainMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                managementMenu();
                break;
            case 2:
                sortMenu();
                break;
            case 3:
                filterMenu();
                break;
            case 4:
                searchMenu();
                break;
            case 5:
                aggregateMenu();
                break;
            case 6:
                db.displayAll();
                break;
            case 0:
                cout << "До свидания!\n";
                return 0;
            default:
                cout << "Некорректный выбор!\n";
        }
    }

    return 0;
}
