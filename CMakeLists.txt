cmake_minimum_required(VERSION 3.10)
project(EmployeeManagementSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск пакетов
<<<<<<< HEAD
## Удалена зависимость от PostgreSQL
=======
find_package(ODBC REQUIRED)
>>>>>>> cc1f4313b492450e58ec61ab1618f987ad7e7338

# Исходные файлы
set(SOURCES
    ConsoleApplication1.cpp
    Employee.cpp
    EmployeeDatabase.cpp
)

# Создание исполняемого файла
add_executable(employee_app ${SOURCES})

<<<<<<< HEAD
=======
# Связывание с библиотеками ODBC
target_link_libraries(employee_app PRIVATE ODBC::ODBC)

>>>>>>> cc1f4313b492450e58ec61ab1618f987ad7e7338
# Для Windows с MSVC
if(MSVC)
    set_target_properties(employee_app PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
endif()
