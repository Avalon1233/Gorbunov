#include "EmployeeDatabase.h"
#include <iostream>
#include <iomanip>
#include <clocale>
#include <sstream>
#include <limits>
#include <utility>

namespace {
    std::string escapeSql(const std::string& value) {
        std::string result;
        result.reserve(value.size() + 4);
        for (char ch : value) {
            if (ch == '\'') {
                result += "''";
            } else {
                result += ch;
            }
        }
        return result;
    }

    std::string quote(const std::string& value) {
        return "'" + escapeSql(value) + "'";
    }

    std::string getValue(const DatabaseConnection::Row& row, const std::string& column) {
        auto it = row.find(column);
        if (it == row.end()) {
            return "";
        }
        return it->second;
    }

    int toInt(const std::string& value) {
        try {
            return value.empty() ? 0 : std::stoi(value);
        } catch (...) {
            return 0;
        }
    }

    double toDouble(const std::string& value) {
        try {
            return value.empty() ? 0.0 : std::stod(value);
        } catch (...) {
            return 0.0;
        }
    }
}
void EmployeeDatabase::addEmployee(const Employee& emp) {
    employees.push_back(emp);
}

bool EmployeeDatabase::removeEmployee(int index) {
    if (index < 0 || index >= static_cast<int>(employees.size())) {
        return false;
    }

    int employeeId = employees[index].getId();
    employees.erase(employees.begin() + index);

    if (employeeId > 0 && dbConn && dbConn->isConnectedToDatabase()) {
        if (!deleteFromDatabase(employeeId)) {
            std::cout << "Удаление из БД завершилось с ошибкой.\n";
        } else {
            std::cout << "Запись удалена из БД.\n";
        }
    }

    return true;
}

bool EmployeeDatabase::editEmployee(int index) {
    if (index < 0 || index >= employees.size()) {
        std::cout << "Некорректный индекс!\n";
        return false;
    }

    Employee& emp = employees[index];
    int choice;
    std::string input;
    double doubleInput;
    int intInput;
    std::string genderInput;

    std::cout << "\nВыберите поле для редактирования:\n";
    std::cout << "1. ФИО\n2. Цех\n3. Зарплата\n4. Год рождения\n";
    std::cout << "5. Дата поступления\n6. Семейное положение\n7. Пол\n";
    std::cout << "8. Количество детей\n9. Дата заболевания\n10. Дата выздоровления\n";
    std::cout << "11. Оплата по бюллетеню\n12. Средний заработок\n";
    std::cout << "0. Отмена\n";
    std::cout << "Выбор: ";
    std::cin >> choice;

    if (!std::cin) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Некорректный ввод!\n";
        return false;
    }

    bool modified = false;

    auto ignoreLine = []() {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    };

    switch (choice) {
        case 1:
            std::cout << "Введите ФИО: ";
            ignoreLine();
            std::getline(std::cin, input);
            emp.setFullName(input);
            modified = true;
            break;
        case 2:
            std::cout << "Введите цех: ";
            ignoreLine();
            std::getline(std::cin, input);
            emp.setWorkshop(input);
            modified = true;
            break;
        case 3:
            std::cout << "Введите зарплату: ";
            std::cin >> doubleInput;
            emp.setSalary(doubleInput);
            modified = true;
            break;
        case 4:
            std::cout << "Введите год рождения: ";
            std::cin >> intInput;
            emp.setBirthYear(intInput);
            modified = true;
            break;
        case 5:
            std::cout << "Введите дату поступления (ДД.ММ.ГГГГ): ";
            ignoreLine();
            std::getline(std::cin, input);
            emp.setHireDate(input);
            modified = true;
            break;
        case 6:
            std::cout << "Введите семейное положение: ";
            ignoreLine();
            std::getline(std::cin, input);
            emp.setMaritalStatus(input);
            modified = true;
            break;
        case 7: {
            std::cout << "Введите пол (М/Ж): ";
            ignoreLine();
            std::getline(std::cin, genderInput);
            char normalized = Employee::normalizeGenderInput(genderInput);
            if (normalized == ' ') {
                std::cout << "Пол не изменен. Допустимы значения М/Ж.\n";
                return false;
            }
            emp.setGender(normalized);
            modified = true;
            break;
        }
        case 8:
            std::cout << "Введите количество детей: ";
            std::cin >> intInput;
            emp.setChildrenCount(intInput);
            modified = true;
            break;
        case 9:
            std::cout << "Введите дату заболевания (ДД.ММ.ГГГГ): ";
            ignoreLine();
            std::getline(std::cin, input);
            emp.setIllnessDate(input);
            modified = true;
            break;
        case 10:
            std::cout << "Введите дату выздоровления (ДД.ММ.ГГГГ): ";
            ignoreLine();
            std::getline(std::cin, input);
            emp.setRecoveryDate(input);
            modified = true;
            break;
        case 11:
            std::cout << "Введите оплату по бюллетеню (%): ";
            std::cin >> doubleInput;
            emp.setBulletinPayPercent(doubleInput);
            modified = true;
            break;
        case 12:
            std::cout << "Введите средний заработок: ";
            std::cin >> doubleInput;
            emp.setAverageEarnings(doubleInput);
            modified = true;
            break;
        case 0:
            std::cout << "Редактирование отменено.\n";
            return false;
        default:
            std::cout << "Некорректный выбор!\n";
            return false;
    }

    if (!modified) {
        return false;
    }

    std::cout << "Сотрудник успешно отредактирован!\n";

    if (dbConn && dbConn->isConnectedToDatabase()) {
        if (emp.getId() > 0) {
            if (updateInDatabase(emp.getId(), emp)) {
                std::cout << "Изменения сохранены в БД.\n";
            } else {
                std::cout << "Не удалось обновить запись в БД.\n";
            }
        } else {
            if (saveToDatabase(emp)) {
                std::cout << "Запись сохранена в БД.\n";
            } else {
                std::cout << "Не удалось сохранить запись в БД.\n";
            }
        }
    }

    return true;
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

// Работа с БД
void EmployeeDatabase::setDatabaseConnection(std::shared_ptr<DatabaseConnection> conn) {
    dbConn = std::move(conn);
}

bool EmployeeDatabase::loadFromDatabase() {
    if (!dbConn || !dbConn->isConnectedToDatabase()) {
        std::cout << "Нет подключения к БД!\n";
        return false;
    }

    try {
        employees.clear();
        auto rows = dbConn->executeQuery(
            "SELECT id, full_name, workshop, salary, birth_year, hire_date, "
            "marital_status, gender, children_count, illness_date, recovery_date, "
            "bulletin_pay_percent, average_earnings FROM employees ORDER BY id");

        for (const auto& row : rows) {
            std::string genderStr = getValue(row, "gender");
            char gender = Employee::normalizeGenderInput(genderStr);

            Employee emp(
                getValue(row, "full_name"),
                getValue(row, "workshop"),
                toDouble(getValue(row, "salary")),
                toInt(getValue(row, "birth_year")),
                getValue(row, "hire_date"),
                getValue(row, "marital_status"),
                gender,
                toInt(getValue(row, "children_count")),
                getValue(row, "illness_date"),
                getValue(row, "recovery_date"),
                toDouble(getValue(row, "bulletin_pay_percent")),
                toDouble(getValue(row, "average_earnings")),
                toInt(getValue(row, "id"))
            );

            employees.push_back(emp);
        }

        std::cout << "Данные загружены из БД. Всего сотрудников: " << employees.size() << "\n";
        return true;
    } catch (const std::exception& e) {
        std::cout << "Ошибка загрузки данных: " << e.what() << "\n";
        return false;
    }
}

bool EmployeeDatabase::saveToDatabase(Employee& emp) {
    if (!dbConn || !dbConn->isConnectedToDatabase()) {
        std::cout << "Нет подключения к БД!\n";
        return false;
    }

    try {
        auto quoteOrNull = [](const std::string& value) {
            return value.empty() ? std::string("NULL") : quote(value);
        };

        std::string gender(1, emp.getGender() == '\0' ? ' ' : emp.getGender());

        std::ostringstream query;
        query << "INSERT INTO employees (full_name, workshop, salary, birth_year, "
              << "hire_date, marital_status, gender, children_count, illness_date, "
              << "recovery_date, bulletin_pay_percent, average_earnings) VALUES ("
              << quote(emp.getFullName()) << ", "
              << quote(emp.getWorkshop()) << ", "
              << emp.getSalary() << ", "
              << emp.getBirthYear() << ", "
              << quote(emp.getHireDate()) << ", "
              << quote(emp.getMaritalStatus()) << ", "
              << quote(gender) << ", "
              << emp.getChildrenCount() << ", "
              << quoteOrNull(emp.getIllnessDate()) << ", "
              << quoteOrNull(emp.getRecoveryDate()) << ", "
              << emp.getBulletinPayPercent() << ", "
              << emp.getAverageEarnings() << ")";

        if (!dbConn->executeUpdate(query.str())) {
            return false;
        }

        auto rows = dbConn->executeQuery("SELECT @@IDENTITY AS new_id");
        if (!rows.empty()) {
            int newId = toInt(getValue(rows[0], "new_id"));
            if (newId > 0) {
                emp.setId(newId);
            }
        }

        return true;
    } catch (const std::exception& e) {
        std::cout << "Ошибка сохранения данных: " << e.what() << "\n";
        return false;
    }
}

bool EmployeeDatabase::deleteFromDatabase(int id) {
    if (!dbConn || !dbConn->isConnectedToDatabase()) {
        std::cout << "Нет подключения к БД!\n";
        return false;
    }

    try {
        std::ostringstream query;
        query << "DELETE FROM employees WHERE id = " << id;
        return dbConn->executeUpdate(query.str());
    } catch (const std::exception& e) {
        std::cout << "Ошибка удаления данных: " << e.what() << "\n";
        return false;
    }
}

bool EmployeeDatabase::updateInDatabase(int id, const Employee& emp) {
    if (!dbConn || !dbConn->isConnectedToDatabase()) {
        std::cout << "Нет подключения к БД!\n";
        return false;
    }

    try {
        auto quoteOrNull = [](const std::string& value) {
            return value.empty() ? std::string("NULL") : quote(value);
        };
        std::string gender(1, emp.getGender() == '\0' ? ' ' : emp.getGender());

        std::ostringstream query;
        query << "UPDATE employees SET "
              << "full_name = " << quote(emp.getFullName()) << ", "
              << "workshop = " << quote(emp.getWorkshop()) << ", "
              << "salary = " << emp.getSalary() << ", "
              << "birth_year = " << emp.getBirthYear() << ", "
              << "hire_date = " << quote(emp.getHireDate()) << ", "
              << "marital_status = " << quote(emp.getMaritalStatus()) << ", "
              << "gender = " << quote(gender) << ", "
              << "children_count = " << emp.getChildrenCount() << ", "
              << "illness_date = " << quoteOrNull(emp.getIllnessDate()) << ", "
              << "recovery_date = " << quoteOrNull(emp.getRecoveryDate()) << ", "
              << "bulletin_pay_percent = " << emp.getBulletinPayPercent() << ", "
              << "average_earnings = " << emp.getAverageEarnings() << " "
              << "WHERE id = " << id;

        return dbConn->executeUpdate(query.str());
    } catch (const std::exception& e) {
        std::cout << "Ошибка обновления данных: " << e.what() << "\n";
        return false;
    }
}
