cmake_minimum_required(VERSION 3.30)
project(server_multi_client C)

set(CMAKE_C_STANDARD 11)

# Tentukan path header dan library
include_directories(/opt/homebrew/Cellar/cjson/1.7.18/include)
link_directories(/opt/homebrew/Cellar/cjson/1.7.18/lib)

add_executable(server_multi_client main.c
        body/server.c
        header/server.h)

# Tautkan library cJSON
target_link_libraries(server_multi_client cjson)