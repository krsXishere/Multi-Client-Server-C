cmake_minimum_required(VERSION 3.10)
project(server_multi_client C)

# Menambahkan submodule jansson
add_subdirectory(extern/jansson)

# Menambahkan direktori include untuk jansson
include_directories(extern/jansson/src)

# Tentukan standar C yang digunakan
set(CMAKE_C_STANDARD 11)

# Tentukan executable
add_executable(server_multi_client main.c
        body/server.c
        header/server.h
)

find_package(CURL REQUIRED)

target_link_libraries(server_multi_client PRIVATE CURL::libcurl)

# Link library jansson ke proyek
target_link_libraries(server_multi_client PRIVATE jansson)
