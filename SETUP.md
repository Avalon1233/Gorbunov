# Установка и компиляция проекта с PostgreSQL

## Требования
- C++ 17 или выше
- PostgreSQL 10+
- CMake 3.10+
- Компилятор (Visual Studio, g++, clang)

## Установка зависимостей

### Windows (Visual Studio)

1. **Установите PostgreSQL:**
   - Скачайте установщик с https://www.postgresql.org/download/windows/
   - Установите PostgreSQL с полной поддержкой разработки

2. **Установите libpqxx (C++ драйвер для PostgreSQL):**
   ```bash
   # Через vcpkg (рекомендуется)
   vcpkg install libpqxx:x64-windows
   
   # Или скачайте с https://github.com/jtv/libpqxx
   ```

3. **Установите CMake:**
   - Скачайте с https://cmake.org/download/

### Linux (Ubuntu/Debian)

```bash
# Установка PostgreSQL
sudo apt-get install postgresql postgresql-contrib libpq-dev

# Установка libpqxx
sudo apt-get install libpqxx-dev

# Установка CMake
sudo apt-get install cmake
```

### macOS

```bash
# Установка через Homebrew
brew install postgresql libpqxx cmake
```

## Компиляция

### Способ 1: С использованием CMake (рекомендуется)

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
    EmployeeDatabase.cpp \
    DatabaseConnection.cpp \
    -I/usr/include/pqxx \
    -I/usr/include/postgresql \
    -lpqxx -lpq
```

### Способ 3: Visual Studio (MSVC)

```bash
cl /std:c++17 /EHsc ^
    ConsoleApplication1.cpp ^
    Employee.cpp ^
    EmployeeDatabase.cpp ^
    DatabaseConnection.cpp ^
    /I"C:\Program Files\PostgreSQL\15\include" ^
    /link "C:\Program Files\PostgreSQL\15\lib\libpq.lib"
```

## Подготовка базы данных

1. **Создайте базу данных:**
   ```sql
   CREATE DATABASE employees_db;
   ```

2. **Подключитесь к БД:**
   ```bash
   psql -U postgres -d employees_db
   ```

3. **При первом запуске приложения:**
   - Выберите пункт "7. Подключение к БД PostgreSQL"
   - Введите параметры подключения:
     - Хост: localhost
     - Порт: 5432
     - Имя БД: employees_db
     - Пользователь: postgres
     - Пароль: (ваш пароль)

## Использование

После компиляции и запуска приложения:

1. Выберите "7. Подключение к БД PostgreSQL" для подключения
2. Добавляйте, редактируйте и удаляйте сотрудников
3. Все изменения будут автоматически сохраняться в PostgreSQL
4. При перезапуске приложения данные будут загружены из БД

## Решение проблем

### Ошибка: "Cannot find PostgreSQL"
- Убедитесь, что PostgreSQL установлен
- Проверьте переменные окружения PATH
- На Windows добавьте путь к PostgreSQL в PATH

### Ошибка: "libpqxx not found"
- Переустановите libpqxx
- Проверьте пути включения в CMakeLists.txt

### Ошибка подключения к БД
- Проверьте, что PostgreSQL запущен
- Проверьте параметры подключения
- Убедитесь, что база данных существует
- Проверьте права доступа пользователя

## Структура проекта

```
Gorbunov/
├── CMakeLists.txt              # Конфигурация CMake
├── SETUP.md                    # Этот файл
├── Employee.h / .cpp           # Класс сотрудника
├── EmployeeDatabase.h / .cpp   # Управление БД (в памяти + PostgreSQL)
├── DatabaseConnection.h / .cpp # Подключение к PostgreSQL
└── ConsoleApplication1.cpp     # Главное приложение
```
