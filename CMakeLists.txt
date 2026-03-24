cmake_minimum_required(VERSION 3.10)
project(TIA_Game)

set(CMAKE_CXX_STANDARD 17)

# Adicione aqui o caminho para as bibliotecas se necessário
# Se você baixar o crow_all.h, ele facilita muito a vida
add_executable(TIA_Game main.cpp)

# Se estiver no Linux/Mac, precisa dessas threads:
find_package(Threads REQUIRED)
target_link_libraries(TIA_Game Threads::Threads)