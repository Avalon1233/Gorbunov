#include "DatabaseConnection.h"
#include <iostream>
#include <sstream>

DatabaseConnection::DatabaseConnection() : isConnected(false) {}

DatabaseConnection::~DatabaseConnection() {
    disconnect();
}

bool DatabaseConnection::connect(const std::string& host, const std::string& port,
                                 const std::string& database, const std::string& user,
                                 const std::string& password) {
    try {
        std::ostringstream connStr;
        connStr << "host=" << host << " port=" << port << " dbname=" << database
                << " user=" << user << " password=" << password;
        
        connectionString = connStr.str();
        conn = std::make_unique<pqxx::connection>(connectionString);
        
        if (conn->is_open()) {
            isConnected = true;
            std::cout << "Успешное подключение к PostgreSQL!\n";
            return true;
        } else {
            std::cout << "Ошибка подключения к БД!\n";
            isConnected = false;
            return false;
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка подключения: " << e.what() << "\n";
        isConnected = false;
        return false;
    }
}

bool DatabaseConnection::isConnectedToDatabase() const {
    return isConnected && conn && conn->is_open();
}

void DatabaseConnection::disconnect() {
    if (conn && conn->is_open()) {
        conn->disconnect();
        isConnected = false;
        std::cout << "Отключение от БД.\n";
    }
}

pqxx::result DatabaseConnection::executeQuery(const std::string& query) {
    if (!isConnectedToDatabase()) {
        throw std::runtime_error("Нет подключения к БД!");
    }
    
    try {
        pqxx::work txn(*conn);
        pqxx::result res = txn.exec(query);
        txn.commit();
        return res;
    } catch (const std::exception& e) {
        std::cout << "Ошибка выполнения запроса: " << e.what() << "\n";
        throw;
    }
}

bool DatabaseConnection::executeUpdate(const std::string& query) {
    if (!isConnectedToDatabase()) {
        std::cout << "Нет подключения к БД!\n";
        return false;
    }
    
    try {
        pqxx::work txn(*conn);
        txn.exec(query);
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cout << "Ошибка выполнения обновления: " << e.what() << "\n";
        return false;
    }
}

bool DatabaseConnection::initializeTables() {
    if (!isConnectedToDatabase()) {
        std::cout << "Нет подключения к БД!\n";
        return false;
    }
    
    try {
        pqxx::work txn(*conn);
        
        // Создание таблицы employees
        std::string createTableQuery = R"(
            CREATE TABLE IF NOT EXISTS employees (
                id SERIAL PRIMARY KEY,
                full_name VARCHAR(255) NOT NULL,
                workshop VARCHAR(255) NOT NULL,
                salary DOUBLE PRECISION NOT NULL,
                birth_year INTEGER NOT NULL,
                hire_date VARCHAR(10) NOT NULL,
                marital_status VARCHAR(50) NOT NULL,
                gender CHAR(1) NOT NULL,
                children_count INTEGER NOT NULL,
                illness_date VARCHAR(10),
                recovery_date VARCHAR(10),
                bulletin_pay_percent DOUBLE PRECISION NOT NULL,
                average_earnings DOUBLE PRECISION NOT NULL
            )
        )";
        
        txn.exec(createTableQuery);
        txn.commit();
        
        std::cout << "Таблицы инициализированы успешно!\n";
        return true;
    } catch (const std::exception& e) {
        std::cout << "Ошибка инициализации таблиц: " << e.what() << "\n";
        return false;
    }
}

pqxx::connection* DatabaseConnection::getConnection() {
    return conn.get();
}
