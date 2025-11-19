# Установка и компиляция проекта с Microsoft Access

## Требования
- Windows 10/11
- C++ 17 или выше
- Microsoft Access Database Engine 2016 (ODBC-драйвер)
- CMake 3.10+
- Компилятор (Visual Studio/MSVC или MinGW g++)

## Установка зависимостей

### 1. Microsoft Access Database Engine
1. Скачайте `AccessDatabaseEngine_X64.exe` (или X86) с официального сайта Microsoft.
2. Установите драйвер. После установки в системе появится ODBC-драйвер `Microsoft Access Driver (*.mdb, *.accdb)`.
3. Перезагрузите компьютер (рекомендуется), чтобы драйвер стал доступен компилятору.

### 2. CMake
- Скачайте с https://cmake.org/download/ и установите.

### 3. Компилятор
- **Visual Studio**: установите рабочую нагрузку "Разработка классических приложений на C++".
- **MinGW-w64**: установите сборку с поддержкой C++17 и добавьте `g++.exe` в `PATH`.

## Компиляция

### Способ 1: CMake (рекомендуется)
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
./employee_app.exe   # Windows
```

### Способ 2: Прямая компиляция (MinGW g++)
```bash
g++ -std=c++17 -O2 -Wall ^
    ConsoleApplication1.cpp ^
    Employee.cpp ^
    EmployeeDatabase.cpp ^
    DatabaseConnection.cpp ^
    -lodbc32 ^
    -o employee_app.exe
```

### Способ 3: MSVC (cl)
```bash
cl /std:c++17 /EHsc ^
    ConsoleApplication1.cpp ^
    Employee.cpp ^
    EmployeeDatabase.cpp ^
    DatabaseConnection.cpp ^
    /link odbc32.lib
```

## Подготовка базы данных Access
1. Создайте пустой файл `employees.accdb` (можно в Microsoft Access: File → New → Blank Database).
2. Сохраните файл в удобной директории, например `C:\Data\employees.accdb`.
3. Если необходимо, установите пароль на файл (он будет вводиться при подключении).

## Подключение внутри приложения
1. Запустите программу и выберите пункт `7. Подключение к БД Microsoft Access`.
2. Укажите полный путь к файлу `.accdb`/`.mdb`.
3. Введите пароль (или оставьте поле пустым).
4. Приложение автоматически создаст таблицу `employees`, если она отсутствует.

## Решение проблем

### Ошибка: "Microsoft Access Driver (*.mdb, *.accdb) not found"
- Проверьте, установлен ли Access Database Engine.
- Убедитесь, что разрядность драйвера совпадает с компилятором (x86/x64).

### Ошибка: "Не удалось открыть файл БД"
- Проверьте путь к файлу и наличие доступа (файл не должен быть открыт в Access).
- Убедитесь, что у вас есть права записи в каталог с БД.

### Ошибка: "Operation must use an updateable query"
- Убедитесь, что файл БД расположен в каталоге с правом записи (не Program Files).

## Структура проекта

```
Gorbunov/
├── CMakeLists.txt              # Конфигурация CMake (ODBC)
├── SETUP.md                    # Этот файл
├── Employee.h / .cpp           # Класс сотрудника
├── EmployeeDatabase.h / .cpp   # Управление БД (в памяти + Access)
├── DatabaseConnection.h / .cpp # ODBC-подключение к Access
└── ConsoleApplication1.cpp     # Главное приложение
```
