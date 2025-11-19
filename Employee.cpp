#include "Employee.h"
#include <clocale>
#include <locale>
#include <cctype>

Employee::Employee() 
    : id(-1), fullName(""), workshop(""), salary(0), birthYear(0), hireDate(""),
      maritalStatus(""), gender(' '), childrenCount(0), illnessDate(""),
      recoveryDate(""), bulletinPayPercent(0), averageEarnings(0) {}

Employee::Employee(const std::string& name, const std::string& shop, double sal, int birth,
                   const std::string& hire, const std::string& marital, char gen, int children,
                   const std::string& illness, const std::string& recovery, double bulletinPay, double avgEarn,
                   int employeeId)
    : id(employeeId), fullName(name), workshop(shop), salary(sal), birthYear(birth), hireDate(hire),
      maritalStatus(marital), gender(gen), childrenCount(children), illnessDate(illness),
      recoveryDate(recovery), bulletinPayPercent(bulletinPay), averageEarnings(avgEarn) {}

// Геттеры
int Employee::getId() const { return id; }
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
void Employee::setId(int newId) { id = newId; }
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

namespace {
    std::string trim(const std::string& str) {
        const std::string whitespace = " \t\n\r";
        size_t start = str.find_first_not_of(whitespace);
        if (start == std::string::npos) return "";
        size_t end = str.find_last_not_of(whitespace);
        return str.substr(start, end - start + 1);
    }
}

char Employee::normalizeGenderInput(const std::string& input) {
    std::string value = trim(input);
    if (value.empty()) {
        return ' ';
    }

    unsigned char first = static_cast<unsigned char>(value[0]);
    unsigned char second = value.size() > 1 ? static_cast<unsigned char>(value[1]) : 0;

    // UTF-8 sequences
    if (first == 0xD0 && (second == 0x9C || second == 0xBC)) { // М/м
        return 'M';
    }
    if ((first == 0xD0 && (second == 0x96 || second == 0xB6)) ||  // Ж/ж
        (first == 0xD0 && (second == 0x81 || second == 0xB1))) {   // Ё как альтернатива
        return 'F';
    }

    // CP1251 single-byte
    if (first == 0xCC || first == 0xEC) { // М/м
        return 'M';
    }
    if (first == 0xC6 || first == 0xE6) { // Ж/ж
        return 'F';
    }

    char ascii = static_cast<char>(std::toupper(static_cast<unsigned char>(first)));
    if (ascii == 'M') return 'M';
    if (ascii == 'F') return 'F';

    return ' ';
}

std::string Employee::genderToText(char gender) {
    switch (gender) {
        case 'M': return "М";
        case 'F': return "Ж";
        default:
            if (gender == ' ' || gender == '\0') {
                return "-";
            }
            return std::string(1, gender);
    }
}

void Employee::display() const {
    std::cout << "\n=== Информация о сотруднике ===\n";
    if (id > 0) {
        std::cout << "ID (БД): " << id << "\n";
    }
    std::cout << "ФИО: " << fullName << "\n";
    std::cout << "Цех: " << workshop << "\n";
    std::cout << "Зарплата: " << salary << " руб.\n";
    std::cout << "Год рождения: " << birthYear << "\n";
    std::cout << "Дата поступления: " << hireDate << "\n";
    std::cout << "Семейное положение: " << maritalStatus << "\n";
    std::cout << "Пол: " << genderToText(gender) << "\n";
    std::cout << "Количество детей: " << childrenCount << "\n";
    std::cout << "Дата заболевания: " << illnessDate << "\n";
    std::cout << "Дата выздоровления: " << recoveryDate << "\n";
    std::cout << "Оплата по бюллетеню: " << bulletinPayPercent << "%\n";
    std::cout << "Средний заработок: " << averageEarnings << " руб.\n";
    std::cout << "================================\n";
}
