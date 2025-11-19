#include "DatabaseConnection.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace {
    constexpr SQLSMALLINT ACCESS_DRIVER_OPTION = SQL_DRIVER_NOPROMPT;
}

DatabaseConnection::DatabaseConnection()
    : envHandle(SQL_NULL_HENV),
      dbcHandle(SQL_NULL_HDBC),
      isConnected(false) {}

DatabaseConnection::~DatabaseConnection() {
    disconnect();
}

bool DatabaseConnection::connect(const std::string& databaseFilePath, const std::string& password) {
    disconnect();

    SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &envHandle);
    if (!SQL_SUCCEEDED(ret)) {
        std::cout << "Не удалось создать окружение ODBC.\n";
        return false;
    }

    ret = SQLSetEnvAttr(envHandle, SQL_ATTR_ODBC_VERSION, reinterpret_cast<void*>(SQL_OV_ODBC3), 0);
    if (!SQL_SUCCEEDED(ret)) {
        std::cout << "Не удалось настроить окружение ODBC.\n";
        disconnect();
        return false;
    }

    ret = SQLAllocHandle(SQL_HANDLE_DBC, envHandle, &dbcHandle);
    if (!SQL_SUCCEEDED(ret)) {
        std::cout << "Не удалось создать соединение ODBC.\n";
        disconnect();
        return false;
    }

    std::ostringstream connStr;
    connStr << "Driver={Microsoft Access Driver (*.mdb, *.accdb)};"
            << "DBQ=" << databaseFilePath << ";";
    if (!password.empty()) {
        connStr << "PWD=" << password << ";";
    }

    std::string conn = connStr.str();
    SQLCHAR outConnStr[1024];
    SQLSMALLINT outConnLen = 0;

    ret = SQLDriverConnectA(
        dbcHandle,
        nullptr,
        reinterpret_cast<SQLCHAR*>(conn.data()),
        SQL_NTS,
        outConnStr,
        sizeof(outConnStr),
        &outConnLen,
        ACCESS_DRIVER_OPTION);

    if (SQL_SUCCEEDED(ret)) {
        isConnected = true;
        databasePath = databaseFilePath;
        std::cout << "Успешное подключение к Microsoft Access!\n";
        return true;
    }

    logOdbcError("Ошибка подключения к Microsoft Access", dbcHandle, SQL_HANDLE_DBC);
    disconnect();
    return false;
}

bool DatabaseConnection::isConnectedToDatabase() const {
    return isConnected && dbcHandle != SQL_NULL_HDBC;
}

void DatabaseConnection::disconnect() {
    if (dbcHandle != SQL_NULL_HDBC) {
        SQLDisconnect(dbcHandle);
        SQLFreeHandle(SQL_HANDLE_DBC, dbcHandle);
        dbcHandle = SQL_NULL_HDBC;
    }

    if (envHandle != SQL_NULL_HENV) {
        SQLFreeHandle(SQL_HANDLE_ENV, envHandle);
        envHandle = SQL_NULL_HENV;
    }

    if (isConnected) {
        std::cout << "Отключение от БД.\n";
    }

    isConnected = false;
    databasePath.clear();
}

DatabaseConnection::QueryResult DatabaseConnection::executeQuery(const std::string& query) {
    if (!isConnectedToDatabase()) {
        throw std::runtime_error("Нет подключения к БД!");
    }

    SQLHSTMT stmt = SQL_NULL_HSTMT;
    SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, dbcHandle, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        throw std::runtime_error("Не удалось подготовить выражение.");
    }

    ret = SQLExecDirectA(stmt, reinterpret_cast<SQLCHAR*>(const_cast<char*>(query.c_str())), SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        logOdbcError("Ошибка выполнения запроса", stmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        throw std::runtime_error("Ошибка выполнения запроса.");
    }

    SQLSMALLINT columnCount = 0;
    SQLNumResultCols(stmt, &columnCount);

    std::vector<std::string> columnNames(columnCount);
    for (SQLSMALLINT i = 1; i <= columnCount; ++i) {
        SQLCHAR colName[128] = {0};
        SQLSMALLINT nameLength = 0;
        SQLSMALLINT dataType = 0;
        SQLULEN colSize = 0;
        SQLSMALLINT decimalDigits = 0;
        SQLSMALLINT nullable = 0;

        SQLDescribeColA(stmt, i, colName, sizeof(colName), &nameLength, &dataType, &colSize, &decimalDigits, &nullable);
        columnNames[i - 1] = normalizeColumnName(reinterpret_cast<char*>(colName));
    }

    QueryResult rows;
    const SQLSMALLINT bufferSize = 1024;
    SQLRETURN fetchRet = SQL_NO_DATA;

    while ((fetchRet = SQLFetch(stmt)) != SQL_NO_DATA) {
        if (!SQL_SUCCEEDED(fetchRet)) {
            logOdbcError("Ошибка чтения данных", stmt, SQL_HANDLE_STMT);
            break;
        }

        Row row;
        for (SQLSMALLINT i = 1; i <= columnCount; ++i) {
            char buffer[bufferSize] = {0};
            SQLLEN indicator = 0;

            ret = SQLGetData(stmt, i, SQL_C_CHAR, buffer, bufferSize, &indicator);
            if (SQL_SUCCEEDED(ret)) {
                if (indicator == SQL_NULL_DATA) {
                    row[columnNames[i - 1]] = "";
                } else {
                    row[columnNames[i - 1]] = buffer;
                }
            } else {
                logOdbcError("Ошибка получения значения столбца", stmt, SQL_HANDLE_STMT);
                row[columnNames[i - 1]] = "";
            }
        }

        rows.push_back(std::move(row));
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    return rows;
}

bool DatabaseConnection::executeUpdate(const std::string& query) {
    if (!isConnectedToDatabase()) {
        std::cout << "Нет подключения к БД!\n";
        return false;
    }

    SQLHSTMT stmt = SQL_NULL_HSTMT;
    SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, dbcHandle, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        std::cout << "Не удалось подготовить выражение для обновления.\n";
        return false;
    }

    ret = SQLExecDirectA(stmt, reinterpret_cast<SQLCHAR*>(const_cast<char*>(query.c_str())), SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        logOdbcError("Ошибка выполнения обновления", stmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return false;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    return true;
}

bool DatabaseConnection::initializeTables() {
    if (!isConnectedToDatabase()) {
        std::cout << "Нет подключения к БД!\n";
        return false;
    }

    if (tableExists("employees")) {
        std::cout << "Таблица employees уже существует.\n";
        return true;
    }

    std::string createTableQuery = R"(
        CREATE TABLE employees (
            id AUTOINCREMENT PRIMARY KEY,
            full_name TEXT(255) NOT NULL,
            workshop TEXT(255) NOT NULL,
            salary DOUBLE NOT NULL,
            birth_year INTEGER NOT NULL,
            hire_date TEXT(10) NOT NULL,
            marital_status TEXT(50) NOT NULL,
            gender TEXT(1) NOT NULL,
            children_count INTEGER NOT NULL,
            illness_date TEXT(10),
            recovery_date TEXT(10),
            bulletin_pay_percent DOUBLE NOT NULL,
            average_earnings DOUBLE NOT NULL
        )
    )";

    if (executeUpdate(createTableQuery)) {
        std::cout << "Таблица employees создана успешно.\n";
        return true;
    }

    std::cout << "Ошибка создания таблицы employees.\n";
    return false;
}

std::string DatabaseConnection::normalizeColumnName(const std::string& name) {
    std::string normalized = name;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return normalized;
}

void DatabaseConnection::logOdbcError(const std::string& message, SQLHANDLE handle, SQLSMALLINT type) const {
    std::cout << message << "\n";

    SQLSMALLINT record = 1;
    SQLINTEGER nativeError = 0;
    SQLCHAR sqlState[6] = {0};
    SQLCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH] = {0};
    SQLSMALLINT textLength = 0;

    while (SQLGetDiagRecA(type, handle, record, sqlState, &nativeError, errorMsg,
                          sizeof(errorMsg), &textLength) != SQL_NO_DATA) {
        std::cout << "SQLSTATE=" << sqlState
                  << " NativeError=" << nativeError
                  << " Message=" << errorMsg << "\n";
        ++record;
    }
}

bool DatabaseConnection::tableExists(const std::string& tableName) {
    SQLHSTMT stmt = SQL_NULL_HSTMT;
    SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, dbcHandle, &stmt);
    if (!SQL_SUCCEEDED(ret)) {
        return false;
    }

    SQLCHAR tableType[] = "TABLE";
    ret = SQLTablesA(stmt,
                     nullptr, 0,
                     nullptr, 0,
                     reinterpret_cast<SQLCHAR*>(const_cast<char*>(tableName.c_str())), SQL_NTS,
                     tableType, SQL_NTS);

    if (!SQL_SUCCEEDED(ret)) {
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return false;
    }

    SQLRETURN fetchRet = SQLFetch(stmt);
    bool exists = SQL_SUCCEEDED(fetchRet);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    return exists;
}
