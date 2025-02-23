cmake_minimum_required(VERSION 3.10)
project(LAB06_osa)

# Configura o padrão C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Habilita o modo Debug
set(CMAKE_BUILD_TYPE Debug)

# Define o nome do executável
add_executable(btree
        src/main.cpp
        src/BTree.cpp
)

# Adiciona o diretório de headers
target_include_directories(btree PRIVATE ${CMAKE_SOURCE_DIR}/include)