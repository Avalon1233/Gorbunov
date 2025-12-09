#pragma once
#include "Employee.h"
#include <vector>
#include <algorithm>
#include <clocale>
#include <memory>

class EmployeeDatabase {
private:
    std::vector<Employee> employees;

public:
    // Основные операции
    void addEmployee(const Employee& emp);
    bool removeEmployee(int index);
    bool editEmployee(int index);
    void displayAll() const;
    void displayEmployee(int index) const;

    // Сортировка
    void sortByName();
    void sortBySalary();
    void sortByWorkshop();
    void sortByBirthYear();
    void sortByHireDate();

    // Фильтрация
    std::vector<int> filterByWorkshop(const std::string& workshop) const;
    std::vector<int> filterByMaritalStatus(const std::string& status) const;
    std::vector<int> filterByGender(char gender) const;
    std::vector<int> filterByChildrenCount(int count) const;
    std::vector<int> filterBySalaryRange(double minSal, double maxSal) const;

    // Поиск
    int findByName(const std::string& name) const;
    std::vector<int> findByWorkshop(const std::string& workshop) const;

    // Агрегирующие запросы
    double getTotalSalary() const;
    double getAverageSalary() const;
    double getMaxSalary() const;
    double getMinSalary() const;
    int getTotalEmployees() const;
    double getAverageSalaryByWorkshop(const std::string& workshop) const;
    int getEmployeeCountByWorkshop(const std::string& workshop) const;
    double getTotalBulletinPayByWorkshop(const std::string& workshop) const;
    int getAverageChildrenCount() const;

    // Утилиты
    bool isEmpty() const;
    void clear();
    Employee& getEmployee(int index);
    const Employee& getEmployeeConst(int index) const;
};
