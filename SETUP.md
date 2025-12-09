# Установка и компиляция проекта

## Требования
- C++ 17 или выше
- CMake 3.10+
- Компилятор (Visual Studio, g++, clang)

## Компиляция

### Способ 1: С использованием CMake

```bash
# Создайте директорию для сборки
mkdir build
cd build

# Сгенерируйте файлы сборки
cmake ..

# Скомпилируйте проект
cmake --build . --config Release

# Запустите приложение
./employee_app  # Linux/macOS
employee_app.exe  # Windows
```

### Способ 2: Прямая компиляция (g++)

```bash
g++ -std=c++17 -o employee_app \
    ConsoleApplication1.cpp \
    Employee.cpp \
    EmployeeDatabase.cpp
```

### Способ 3: Visual Studio (MSVC)

```bash
cl /std:c++17 /EHsc ^
    ConsoleApplication1.cpp ^
    Employee.cpp ^
    EmployeeDatabase.cpp
```

## Структура проекта

```
Gorbunov/
├── CMakeLists.txt
├── SETUP.md
├── Employee.h / .cpp
├── EmployeeDatabase.h / .cpp
└── ConsoleApplication1.cpp
```
=======
