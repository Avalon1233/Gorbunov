#include "EmployeeDatabase.h"
#include <iostream>
#include <iomanip>
#include <clocale>

void EmployeeDatabase::addEmployee(const Employee& emp) {
    employees.push_back(emp);
}

void EmployeeDatabase::removeEmployee(int index) {
    if (index >= 0 && index < employees.size()) {
        employees.erase(employees.begin() + index);
    }
}

void EmployeeDatabase::editEmployee(int index) {
    if (index < 0 || index >= employees.size()) {
        std::cout << "Некорректный индекс!\n";
        return;
    }

    Employee& emp = employees[index];
    int choice;
    std::string input;
    double doubleInput;
    int intInput;
    char charInput;

    std::cout << "\nВыберите поле для редактирования:\n";
    std::cout << "1. ФИО\n2. Цех\n3. Зарплата\n4. Год рождения\n";
    std::cout << "5. Дата поступления\n6. Семейное положение\n7. Пол\n";
    std::cout << "8. Количество детей\n9. Дата заболевания\n10. Дата выздоровления\n";
    std::cout << "11. Оплата по бюллетеню\n12. Средний заработок\n";
    std::cout << "0. Отмена\n";
    std::cout << "Выбор: ";
    std::cin >> choice;
    std::cin.ignore();

    switch (choice) {
        case 1:
            std::cout << "Введите ФИО: ";
            std::getline(std::cin, input);
            emp.setFullName(input);
            break;
        case 2:
            std::cout << "Введите цех: ";
            std::getline(std::cin, input);
            emp.setWorkshop(input);
            break;
        case 3:
            std::cout << "Введите зарплату: ";
            std::cin >> doubleInput;
            emp.setSalary(doubleInput);
            break;
        case 4:
            std::cout << "Введите год рождения: ";
            std::cin >> intInput;
            emp.setBirthYear(intInput);
            break;
        case 5:
            std::cout << "Введите дату поступления (ДД.ММ.ГГГГ): ";
            std::cin.ignore();
            std::getline(std::cin, input);
            emp.setHireDate(input);
            break;
        case 6:
            std::cout << "Введите семейное положение: ";
            std::getline(std::cin, input);
            emp.setMaritalStatus(input);
            break;
        case 7:
            std::cout << "Введите пол (М/Ж): ";
            std::cin >> charInput;
            emp.setGender(charInput);
            break;
        case 8:
            std::cout << "Введите количество детей: ";
            std::cin >> intInput;
            emp.setChildrenCount(intInput);
            break;
        case 9:
            std::cout << "Введите дату заболевания (ДД.ММ.ГГГГ): ";
            std::cin.ignore();
            std::getline(std::cin, input);
            emp.setIllnessDate(input);
            break;
        case 10:
            std::cout << "Введите дату выздоровления (ДД.ММ.ГГГГ): ";
            std::cin.ignore();
            std::getline(std::cin, input);
            emp.setRecoveryDate(input);
            break;
        case 11:
            std::cout << "Введите оплату по бюллетеню (%): ";
            std::cin >> doubleInput;
            emp.setBulletinPayPercent(doubleInput);
            break;
        case 12:
            std::cout << "Введите средний заработок: ";
            std::cin >> doubleInput;
            emp.setAverageEarnings(doubleInput);
            break;
        case 0:
            std::cout << "Редактирование отменено.\n";
            break;
        default:
            std::cout << "Некорректный выбор!\n";
    }
}

void EmployeeDatabase::displayAll() const {
    if (employees.empty()) {
        std::cout << "База данных пуста!\n";
        return;
    }

    std::cout << "\n=== Все сотрудники ===\n";
    std::cout << std::left << std::setw(3) << "№" << std::setw(25) << "ФИО" 
              << std::setw(15) << "Цех" << std::setw(12) << "Зарплата" 
              << std::setw(10) << "Год рожд." << "\n";
    std::cout << std::string(65, '-') << "\n";

    for (size_t i = 0; i < employees.size(); ++i) {
        std::cout << std::left << std::setw(3) << (i + 1) 
                  << std::setw(25) << employees[i].getFullName()
                  << std::setw(15) << employees[i].getWorkshop()
                  << std::setw(12) << employees[i].getSalary()
                  << std::setw(10) << employees[i].getBirthYear() << "\n";
    }
    std::cout << std::string(65, '-') << "\n";
}

void EmployeeDatabase::displayEmployee(int index) const {
    if (index >= 0 && index < employees.size()) {
        employees[index].display();
    } else {
        std::cout << "Некорректный индекс!\n";
    }
}

// Сортировка
void EmployeeDatabase::sortByName() {
    std::sort(employees.begin(), employees.end(),
              [](const Employee& a, const Employee& b) {
                  return a.getFullName() < b.getFullName();
              });
    std::cout << "Сотрудники отсортированы по ФИО.\n";
}

void EmployeeDatabase::sortBySalary() {
    std::sort(employees.begin(), employees.end(),
              [](const Employee& a, const Employee& b) {
                  return a.getSalary() > b.getSalary();
              });
    std::cout << "Сотрудники отсортированы по зарплате (убывание).\n";
}

void EmployeeDatabase::sortByWorkshop() {
    std::sort(employees.begin(), employees.end(),
              [](const Employee& a, const Employee& b) {
                  return a.getWorkshop() < b.getWorkshop();
              });
    std::cout << "Сотрудники отсортированы по цеху.\n";
}

void EmployeeDatabase::sortByBirthYear() {
    std::sort(employees.begin(), employees.end(),
              [](const Employee& a, const Employee& b) {
                  return a.getBirthYear() > b.getBirthYear();
              });
    std::cout << "Сотрудники отсортированы по году рождения.\n";
}

void EmployeeDatabase::sortByHireDate() {
    std::sort(employees.begin(), employees.end(),
              [](const Employee& a, const Employee& b) {
                  return a.getHireDate() < b.getHireDate();
              });
    std::cout << "Сотрудники отсортированы по дате поступления.\n";
}

// Фильтрация
std::vector<int> EmployeeDatabase::filterByWorkshop(const std::string& workshop) const {
    std::vector<int> result;
    for (size_t i = 0; i < employees.size(); ++i) {
        if (employees[i].getWorkshop() == workshop) {
            result.push_back(i);
        }
    }
    return result;
}

std::vector<int> EmployeeDatabase::filterByMaritalStatus(const std::string& status) const {
    std::vector<int> result;
    for (size_t i = 0; i < employees.size(); ++i) {
        if (employees[i].getMaritalStatus() == status) {
            result.push_back(i);
        }
    }
    return result;
}

std::vector<int> EmployeeDatabase::filterByGender(char gender) const {
    std::vector<int> result;
    for (size_t i = 0; i < employees.size(); ++i) {
        if (employees[i].getGender() == gender) {
            result.push_back(i);
        }
    }
    return result;
}

std::vector<int> EmployeeDatabase::filterByChildrenCount(int count) const {
    std::vector<int> result;
    for (size_t i = 0; i < employees.size(); ++i) {
        if (employees[i].getChildrenCount() == count) {
            result.push_back(i);
        }
    }
    return result;
}

std::vector<int> EmployeeDatabase::filterBySalaryRange(double minSal, double maxSal) const {
    std::vector<int> result;
    for (size_t i = 0; i < employees.size(); ++i) {
        double sal = employees[i].getSalary();
        if (sal >= minSal && sal <= maxSal) {
            result.push_back(i);
        }
    }
    return result;
}

// Поиск
int EmployeeDatabase::findByName(const std::string& name) const {
    for (size_t i = 0; i < employees.size(); ++i) {
        if (employees[i].getFullName() == name) {
            return i;
        }
    }
    return -1;
}

std::vector<int> EmployeeDatabase::findByWorkshop(const std::string& workshop) const {
    return filterByWorkshop(workshop);
}

// Агрегирующие запросы
double EmployeeDatabase::getTotalSalary() const {
    double total = 0;
    for (const auto& emp : employees) {
        total += emp.getSalary();
    }
    return total;
}

double EmployeeDatabase::getAverageSalary() const {
    if (employees.empty()) return 0;
    return getTotalSalary() / employees.size();
}

double EmployeeDatabase::getMaxSalary() const {
    if (employees.empty()) return 0;
    double maxSal = employees[0].getSalary();
    for (const auto& emp : employees) {
        if (emp.getSalary() > maxSal) {
            maxSal = emp.getSalary();
        }
    }
    return maxSal;
}

double EmployeeDatabase::getMinSalary() const {
    if (employees.empty()) return 0;
    double minSal = employees[0].getSalary();
    for (const auto& emp : employees) {
        if (emp.getSalary() < minSal) {
            minSal = emp.getSalary();
        }
    }
    return minSal;
}

int EmployeeDatabase::getTotalEmployees() const {
    return employees.size();
}

double EmployeeDatabase::getAverageSalaryByWorkshop(const std::string& workshop) const {
    auto indices = filterByWorkshop(workshop);
    if (indices.empty()) return 0;

    double total = 0;
    for (int idx : indices) {
        total += employees[idx].getSalary();
    }
    return total / indices.size();
}

int EmployeeDatabase::getEmployeeCountByWorkshop(const std::string& workshop) const {
    return filterByWorkshop(workshop).size();
}

double EmployeeDatabase::getTotalBulletinPayByWorkshop(const std::string& workshop) const {
    auto indices = filterByWorkshop(workshop);
    double total = 0;
    for (int idx : indices) {
        total += employees[idx].getBulletinPayPercent();
    }
    return total;
}

int EmployeeDatabase::getAverageChildrenCount() const {
    if (employees.empty()) return 0;
    int total = 0;
    for (const auto& emp : employees) {
        total += emp.getChildrenCount();
    }
    return total / employees.size();
}

// Утилиты
bool EmployeeDatabase::isEmpty() const {
    return employees.empty();
}

void EmployeeDatabase::clear() {
    employees.clear();
}

Employee& EmployeeDatabase::getEmployee(int index) {
    return employees[index];
}

const Employee& EmployeeDatabase::getEmployeeConst(int index) const {
    return employees[index];
}
