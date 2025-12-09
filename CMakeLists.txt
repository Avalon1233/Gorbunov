cmake_minimum_required(VERSION 3.10)
project(EmployeeManagementSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск пакетов
# Проект не использует внешние драйверы базы данных — in-memory режим

# Исходные файлы
set(SOURCES
    ConsoleApplication1.cpp
    Employee.cpp
    EmployeeDatabase.cpp
)

# Создание исполняемого файла
add_executable(employee_app ${SOURCES})

# Без дополнительных библиотек
# Для Windows с MSVC
if(MSVC)
    set_target_properties(employee_app PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
endif()
