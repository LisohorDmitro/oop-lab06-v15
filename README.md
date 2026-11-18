# Лабораторна робота №6 — варіант 15

Створення та використання динамічної бібліотеки C++

Варіант 15: клас Fraction із лабораторної №4 з полями long та unsigned short, трьома конструкторами, деструктором, Init(), Read(), Display(), toString(), арифметикою, порівняннями та перевантаженими +, -, *, ==. Створити DLL і окремий застосунок-клієнт, продемонструвати функціонал бібліотеки та сформувати звіт. Зв'язок №6 із №4 використано за попереднім уточненням користувача; надруковане в методичці посилання на №9 не переноситься до реалізації.

## Збирання і запуск у Windows

Потрібен GCC MinGW-w64 з підтримкою C++14. Додайте каталог `bin` компілятора до `PATH` або передайте його скрипту через параметр `-CompilerDirectory`.

```powershell
.\build.ps1
Get-Content .\input.txt | .\lab06.exe
```

Під час запуску `fraction.dll` має бути поруч із клієнтом, а каталог `bin` MinGW-w64 у `PATH`.

Команди збирання

```text
g++ -std=c++14 -O2 -Wall -Wextra -pedantic -DFRACTION_DLL -DFRACTION_EXPORTS -c fraction.cpp -o fraction.o
g++ -shared fraction.o -Wl,--out-implib,libfraction.dll.a -o fraction.dll
g++ -std=c++14 -O2 -Wall -Wextra -pedantic -DFRACTION_DLL main.cpp -L. -lfraction -o lab06.exe
```

У `input.txt` збережені тестові вхідні дані, а в `expected_output.txt` — вивід перевіреного запуску.
