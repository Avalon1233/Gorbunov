#pragma once
#include <string>
#include <vector>
#include <memory>
#include <pqxx/pqxx>

class DatabaseConnection {
private:
    std::unique_ptr<pqxx::connection> conn;
    std::string connectionString;
    bool isConnected;

public:
    DatabaseConnection();
    ~DatabaseConnection();

    // Подключение к БД
    bool connect(const std::string& host, const std::string& port, 
                 const std::string& database, const std::string& user, 
                 const std::string& password);
    
    bool isConnectedToDatabase() const;
    void disconnect();

    // Выполнение запросов
    pqxx::result executeQuery(const std::string& query);
    bool executeUpdate(const std::string& query);

    // Инициализация таблиц
    bool initializeTables();

    // Получение соединения
    pqxx::connection* getConnection();
};
