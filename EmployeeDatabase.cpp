#include "EmployeeDatabase.h"
#include <iostream>

void EmployeeDatabase::addEmployee(const Employee& emp) {
	employees.push_back(emp);
}

bool EmployeeDatabase::removeEmployee(int index) {
	if (index < 0 || index >= static_cast<int>(employees.size())) return false;
	employees.erase(employees.begin() + index);
	return true;
}

bool EmployeeDatabase::editEmployee(int index) {
	if (index < 0 || index >= static_cast<int>(employees.size())) return false;
	Employee& e = employees[index];

	std::string input;
	double dval;
	int ival;
	char cval;

	std::cout << "Редактирование сотрудника (оставьте пустым чтобы не менять)\n";
	std::cout << "Текущее ФИО: " << e.getFullName() << "\n";
	std::cout << "Новое ФИО: ";
	std::getline(std::cin, input);
	if (!input.empty()) e.setFullName(input);

	std::cout << "Текущий цех: " << e.getWorkshop() << "\n";
	std::cout << "Новый цех: ";
	std::getline(std::cin, input);
	if (!input.empty()) e.setWorkshop(input);

	std::cout << "Текущая зарплата: " << e.getSalary() << "\n";
	std::cout << "Новая зарплата: ";
	if (std::getline(std::cin, input) && !input.empty()) {
		try { dval = std::stod(input); e.setSalary(dval); } catch(...) {}
	}

	std::cout << "Год рождения (текущий: " << e.getBirthYear() << ") : ";
	if (std::getline(std::cin, input) && !input.empty()) {
		try { ival = std::stoi(input); e.setBirthYear(ival); } catch(...) {}
	}

	return true;
}

void EmployeeDatabase::displayAll() const {
	if (employees.empty()) {
		std::cout << "База данных пуста!\n";
		return;
	}
	for (size_t i = 0; i < employees.size(); ++i) {
		std::cout << (i + 1) << ". ";
		employees[i].display();
	}
}

void EmployeeDatabase::displayEmployee(int index) const {
	if (index < 0 || index >= static_cast<int>(employees.size())) {
		std::cout << "Некорректный индекс сотрудника\n";
		return;
	}
	employees[index].display();
}

void EmployeeDatabase::sortByName() {
	std::sort(employees.begin(), employees.end(), [](const Employee& a, const Employee& b){
		return a.getFullName() < b.getFullName();
	});
}

void EmployeeDatabase::sortBySalary() {
	std::sort(employees.begin(), employees.end(), [](const Employee& a, const Employee& b){
		return a.getSalary() > b.getSalary();
	});
}

void EmployeeDatabase::sortByWorkshop() {
	std::sort(employees.begin(), employees.end(), [](const Employee& a, const Employee& b){
		return a.getWorkshop() < b.getWorkshop();
	});
}

void EmployeeDatabase::sortByBirthYear() {
	std::sort(employees.begin(), employees.end(), [](const Employee& a, const Employee& b){
		return a.getBirthYear() < b.getBirthYear();
	});
}

void EmployeeDatabase::sortByHireDate() {
	std::sort(employees.begin(), employees.end(), [](const Employee& a, const Employee& b){
		return a.getHireDate() < b.getHireDate();
	});
}

std::vector<int> EmployeeDatabase::filterByWorkshop(const std::string& workshop) const {
	std::vector<int> res;
	for (size_t i = 0; i < employees.size(); ++i) {
		if (employees[i].getWorkshop() == workshop) res.push_back(static_cast<int>(i));
	}
	return res;
}

std::vector<int> EmployeeDatabase::filterByMaritalStatus(const std::string& status) const {
	std::vector<int> res;
	for (size_t i = 0; i < employees.size(); ++i) {
		if (employees[i].getMaritalStatus() == status) res.push_back(static_cast<int>(i));
	}
	return res;
}

std::vector<int> EmployeeDatabase::filterByGender(char gender) const {
	std::vector<int> res;
	for (size_t i = 0; i < employees.size(); ++i) {
		if (employees[i].getGender() == gender) res.push_back(static_cast<int>(i));
	}
	return res;
}

std::vector<int> EmployeeDatabase::filterByChildrenCount(int count) const {
	std::vector<int> res;
	for (size_t i = 0; i < employees.size(); ++i) {
		if (employees[i].getChildrenCount() == count) res.push_back(static_cast<int>(i));
	}
	return res;
}

std::vector<int> EmployeeDatabase::filterBySalaryRange(double minSal, double maxSal) const {
	std::vector<int> res;
	for (size_t i = 0; i < employees.size(); ++i) {
		double s = employees[i].getSalary();
		if (s >= minSal && s <= maxSal) res.push_back(static_cast<int>(i));
	}
	return res;
}

int EmployeeDatabase::findByName(const std::string& name) const {
	for (size_t i = 0; i < employees.size(); ++i) {
		if (employees[i].getFullName() == name) return static_cast<int>(i);
	}
	return -1;
}

std::vector<int> EmployeeDatabase::findByWorkshop(const std::string& workshop) const {
	return filterByWorkshop(workshop);
}

double EmployeeDatabase::getTotalSalary() const {
	double total = 0;
	for (const auto& e : employees) total += e.getSalary();
	return total;
}

double EmployeeDatabase::getAverageSalary() const {
	if (employees.empty()) return 0.0;
	return getTotalSalary() / employees.size();
}

double EmployeeDatabase::getMaxSalary() const {
	if (employees.empty()) return 0.0;
	double m = employees[0].getSalary();
	for (const auto& e : employees) if (e.getSalary() > m) m = e.getSalary();
	return m;
}

double EmployeeDatabase::getMinSalary() const {
	if (employees.empty()) return 0.0;
	double m = employees[0].getSalary();
	for (const auto& e : employees) if (e.getSalary() < m) m = e.getSalary();
	return m;
}

int EmployeeDatabase::getTotalEmployees() const {
	return static_cast<int>(employees.size());
}

double EmployeeDatabase::getAverageSalaryByWorkshop(const std::string& workshop) const {
	double total = 0; int count = 0;
	for (const auto& e : employees) if (e.getWorkshop() == workshop) { total += e.getSalary(); ++count; }
	return count == 0 ? 0.0 : (total / count);
}

int EmployeeDatabase::getEmployeeCountByWorkshop(const std::string& workshop) const {
	int count = 0; for (const auto& e : employees) if (e.getWorkshop() == workshop) ++count; return count;
}

double EmployeeDatabase::getTotalBulletinPayByWorkshop(const std::string& workshop) const {
	double total = 0; for (const auto& e : employees) if (e.getWorkshop() == workshop) total += e.getBulletinPayPercent(); return total;
}

int EmployeeDatabase::getAverageChildrenCount() const {
	if (employees.empty()) return 0;
	int total = 0; for (const auto& e : employees) total += e.getChildrenCount(); return total / static_cast<int>(employees.size());
}

bool EmployeeDatabase::isEmpty() const {
	return employees.empty();
}

void EmployeeDatabase::clear() {
	employees.clear();
}

Employee& EmployeeDatabase::getEmployee(int index) {
	return employees.at(index);
}

const Employee& EmployeeDatabase::getEmployeeConst(int index) const {
	return employees.at(index);
}

