cmake_minimum_required(VERSION 3.10)
project(PASJANS_PP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Wskazujemy folder z nagłówkami
include_directories(include)

# Pliki źródłowe gry
set(SOURCES
    src/PASJANS_PP.cpp
    src/funkcje.cpp
)

# Pliki testowe (jeśli potrzebujesz)
set(TEST_SOURCES
    tests/test.cpp
    src/funkcje.cpp
)

# Tworzymy główny plik wykonywalny
add_executable(pasjans ${SOURCES})

# Tworzymy plik wykonywalny do testów
add_executable(pasjans_test ${TEST_SOURCES})
