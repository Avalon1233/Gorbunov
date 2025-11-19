#pragma once
#include <string>
#include <iostream>
#include <ctime>
#include <clocale>

class Employee {
private:
    int id;                         // ID в БД (AUTOINCREMENT)
    std::string fullName;           // ФИО
    std::string workshop;           // Наименование цеха
    double salary;                  // Зарплата
    int birthYear;                  // Год рождения
    std::string hireDate;           // Дата поступления на работу (ДД.ММ.ГГГГ)
    std::string maritalStatus;      // Семейное положение
    char gender;                    // Пол (М/Ж)
    int childrenCount;              // Количество детей
    std::string illnessDate;        // Дата заболевания (ДД.ММ.ГГГГ)
    std::string recoveryDate;       // Дата выздоровления (ДД.ММ.ГГГГ)
    double bulletinPayPercent;      // Оплата по бюллетеню (%)
    double averageEarnings;         // Средний заработок

public:
    // Конструкторы
    Employee();
    Employee(const std::string& name, const std::string& shop, double sal, int birth,
             const std::string& hire, const std::string& marital, char gen, int children,
             const std::string& illness, const std::string& recovery, double bulletinPay, double avgEarn,
             int employeeId = -1);

    // Геттеры
    int getId() const;
    std::string getFullName() const;
    std::string getWorkshop() const;
    double getSalary() const;
    int getBirthYear() const;
    std::string getHireDate() const;
    std::string getMaritalStatus() const;
    char getGender() const;
    int getChildrenCount() const;
    std::string getIllnessDate() const;
    std::string getRecoveryDate() const;
    double getBulletinPayPercent() const;
    double getAverageEarnings() const;

    // Сеттеры
    void setId(int newId);
    void setFullName(const std::string& name);
    void setWorkshop(const std::string& shop);
    void setSalary(double sal);
    void setBirthYear(int birth);
    void setHireDate(const std::string& hire);
    void setMaritalStatus(const std::string& marital);
    void setGender(char gen);
    void setChildrenCount(int children);
    void setIllnessDate(const std::string& illness);
    void setRecoveryDate(const std::string& recovery);
    void setBulletinPayPercent(double bulletinPay);
    void setAverageEarnings(double avgEarn);

    // Утилиты по полу
    static char normalizeGenderInput(const std::string& input);
    static std::string genderToText(char gender);

    // Вывод информации
    void display() const;
};
