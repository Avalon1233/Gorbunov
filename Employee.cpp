#include "Employee.h"

Employee::Employee() 
    : fullName(""), workshop(""), salary(0), birthYear(0), hireDate(""),
      maritalStatus(""), gender(' '), childrenCount(0), illnessDate(""),
      recoveryDate(""), bulletinPayPercent(0), averageEarnings(0) {}

Employee::Employee(const std::string& name, const std::string& shop, double sal, int birth,
                   const std::string& hire, const std::string& marital, char gen, int children,
                   const std::string& illness, const std::string& recovery, double bulletinPay, double avgEarn)
    : fullName(name), workshop(shop), salary(sal), birthYear(birth), hireDate(hire),
      maritalStatus(marital), gender(gen), childrenCount(children), illnessDate(illness),
      recoveryDate(recovery), bulletinPayPercent(bulletinPay), averageEarnings(avgEarn) {}

// Геттеры
std::string Employee::getFullName() const { return fullName; }
std::string Employee::getWorkshop() const { return workshop; }
double Employee::getSalary() const { return salary; }
int Employee::getBirthYear() const { return birthYear; }
std::string Employee::getHireDate() const { return hireDate; }
std::string Employee::getMaritalStatus() const { return maritalStatus; }
char Employee::getGender() const { return gender; }
int Employee::getChildrenCount() const { return childrenCount; }
std::string Employee::getIllnessDate() const { return illnessDate; }
std::string Employee::getRecoveryDate() const { return recoveryDate; }
double Employee::getBulletinPayPercent() const { return bulletinPayPercent; }
double Employee::getAverageEarnings() const { return averageEarnings; }

// Сеттеры
void Employee::setFullName(const std::string& name) { fullName = name; }
void Employee::setWorkshop(const std::string& shop) { workshop = shop; }
void Employee::setSalary(double sal) { salary = sal; }
void Employee::setBirthYear(int birth) { birthYear = birth; }
void Employee::setHireDate(const std::string& hire) { hireDate = hire; }
void Employee::setMaritalStatus(const std::string& marital) { maritalStatus = marital; }
void Employee::setGender(char gen) { gender = gen; }
void Employee::setChildrenCount(int children) { childrenCount = children; }
void Employee::setIllnessDate(const std::string& illness) { illnessDate = illness; }
void Employee::setRecoveryDate(const std::string& recovery) { recoveryDate = recovery; }
void Employee::setBulletinPayPercent(double bulletinPay) { bulletinPayPercent = bulletinPay; }
void Employee::setAverageEarnings(double avgEarn) { averageEarnings = avgEarn; }

void Employee::display() const {
    std::cout << "\n=== Информация о сотруднике ===\n";
    std::cout << "ФИО: " << fullName << "\n";
    std::cout << "Цех: " << workshop << "\n";
    std::cout << "Зарплата: " << salary << " руб.\n";
    std::cout << "Год рождения: " << birthYear << "\n";
    std::cout << "Дата поступления: " << hireDate << "\n";
    std::cout << "Семейное положение: " << maritalStatus << "\n";
    std::cout << "Пол: " << gender << "\n";
    std::cout << "Количество детей: " << childrenCount << "\n";
    std::cout << "Дата заболевания: " << illnessDate << "\n";
    std::cout << "Дата выздоровления: " << recoveryDate << "\n";
    std::cout << "Оплата по бюллетеню: " << bulletinPayPercent << "%\n";
    std::cout << "Средний заработок: " << averageEarnings << " руб.\n";
    std::cout << "================================\n";
}
