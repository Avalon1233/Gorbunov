#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <string>
#include <vector>
#include <unordered_map>
#include <sql.h>
#include <sqlext.h>

class DatabaseConnection {
public:
    using Row = std::unordered_map<std::string, std::string>;
    using QueryResult = std::vector<Row>;

private:
    SQLHENV envHandle;
    SQLHDBC dbcHandle;
    bool isConnected;
    std::string databasePath;

    static std::string normalizeColumnName(const std::string& name);
    void logOdbcError(const std::string& message, SQLHANDLE handle, SQLSMALLINT type) const;
    bool tableExists(const std::string& tableName);

public:
    DatabaseConnection();
    ~DatabaseConnection();

    bool connect(const std::string& databaseFilePath, const std::string& password = "");
    bool isConnectedToDatabase() const;
    void disconnect();

    QueryResult executeQuery(const std::string& query);
    bool executeUpdate(const std::string& query);

    bool initializeTables();
};
