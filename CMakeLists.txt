cmake_minimum_required(VERSION 3.10)
project(EmployeeManagementSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск пакетов
## Удалена зависимость от PostgreSQL

# Исходные файлы
set(SOURCES
    ConsoleApplication1.cpp
    Employee.cpp
    EmployeeDatabase.cpp
)

# Создание исполняемого файла
add_executable(employee_app ${SOURCES})

# Для Windows с MSVC
if(MSVC)
    set_target_properties(employee_app PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
endif()
