#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <locale>
#include <clocale>
#include <memory>
#include <filesystem>
#include <limits>
#include "Employee.h"
#include "EmployeeDatabase.h"
#include "DatabaseConnection.h"

using namespace std;
namespace fs = std::filesystem;

EmployeeDatabase db;
shared_ptr<DatabaseConnection> dbConnection;

void clearInputLine() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void initializeDatabase() {
    cout << "\n===== ПОДКЛЮЧЕНИЕ К БАЗЕ ДАННЫХ =====\n";
    cout << "Работа с Microsoft Access (.mdb/.accdb)\n";

    clearInputLine();

    string dbPath;
    cout << "Укажите полный путь к файлу БД: ";
    getline(cin, dbPath);

    if (dbPath.empty()) {
        cout << "Путь к файлу не может быть пустым!\n";
        return;
    }

    if (!fs::exists(dbPath)) {
        cout << "Файл БД не найден: " << dbPath << "\n";
        return;
    }

    string password;
    cout << "Пароль (если отсутствует, оставьте пустым): ";
    getline(cin, password);

    dbConnection = make_shared<DatabaseConnection>();
    
    if (dbConnection->connect(dbPath, password)) {
        if (dbConnection->initializeTables()) {
            db.setDatabaseConnection(dbConnection);
            db.loadFromDatabase();
            cout << "Успешное подключение и инициализация БД Access!\n";
        } else {
            cout << "Ошибка инициализации таблиц!\n";
        }
    } else {
        cout << "Ошибка подключения к БД!\n";
        dbConnection = nullptr;
    }
}

void displayMainMenu() {
    cout << "\n========== ГЛАВНОЕ МЕНЮ ==========\n";
    cout << "1. Управление сотрудниками\n";
    cout << "2. Сортировка\n";
    cout << "3. Фильтрация\n";
    cout << "4. Поиск\n";
    cout << "5. Агрегирующие запросы\n";
    cout << "6. Просмотр всех сотрудников\n";
    cout << "7. Подключение к БД Microsoft Access\n";
    cout << "0. Выход\n";
    cout << "==================================\n";
    if (dbConnection && dbConnection->isConnectedToDatabase()) {
        cout << "[БД подключена]\n";
    }
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
    cout << "\n========== СОРТИРОВКА ==========\n";
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
    cout << "\n========== ФИЛЬТРАЦИЯ ==========\n";
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
    cout << "\n========== ПОИСК ==========\n";
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
    clearInputLine();
    getline(cin, name);

    cout << "Цех: ";
    getline(cin, workshop);

    cout << "Зарплата: ";
    cin >> salary;
    clearInputLine();

    cout << "Год рождения: ";
    cin >> birthYear;
    clearInputLine();

    cout << "Дата поступления (ДД.ММ.ГГГГ): ";
    getline(cin, hireDate);

    cout << "Семейное положение: ";
    getline(cin, maritalStatus);

    cout << "Пол (М/Ж): ";
    cin >> gender;
    clearInputLine();

    cout << "Количество детей: ";
    cin >> childrenCount;
    clearInputLine();

    cout << "Дата заболевания (ДД.ММ.ГГГГ): ";
    getline(cin, illnessDate);

    cout << "Дата выздоровления (ДД.ММ.ГГГГ): ";
    getline(cin, recoveryDate);

    cout << "Оплата по бюллетеню (%): ";
    cin >> bulletinPayPercent;
    clearInputLine();

    cout << "Средний заработок: ";
    cin >> averageEarnings;
    clearInputLine();

    Employee emp(name, workshop, salary, birthYear, hireDate, maritalStatus, 
                 gender, childrenCount, illnessDate, recoveryDate, bulletinPayPercent, averageEarnings);
    db.addEmployee(emp);
    int storedIndex = db.getTotalEmployees() - 1;
    Employee& storedEmployee = db.getEmployee(storedIndex);
    
    // Сохранение в БД если подключена
    if (dbConnection && dbConnection->isConnectedToDatabase()) {
        if (db.saveToDatabase(storedEmployee)) {
            cout << "Сотрудник успешно добавлен и сохранен в БД!\n";
        } else {
            cout << "Сотрудник добавлен, но ошибка при сохранении в БД!\n";
        }
    } else {
        cout << "Сотрудник успешно добавлен!\n";
    }
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
                char gender;
                cout << "Введите пол (М/Ж): ";
                cin >> gender;
                clearInputLine();
                auto indices = db.filterByGender(gender);
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
    setlocale(LC_ALL, "ru_RU.UTF-8");
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
            case 7:
                initializeDatabase();
                break;
            case 0:
                cout << "До свидания!\n";
                if (dbConnection && dbConnection->isConnectedToDatabase()) {
                    dbConnection->disconnect();
                }
                return 0;
            default:
                cout << "Некорректный выбор!\n";
        }
    }

    return 0;
}
