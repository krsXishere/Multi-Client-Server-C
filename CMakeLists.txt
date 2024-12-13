cmake_minimum_required(VERSION 3.10)
project(server_multi_client C)

set(CMAKE_C_STANDARD 11)

add_executable(server_multi_client main.c
        body/server.c
        header/server.h
)

find_package(CURL REQUIRED)

target_link_libraries(server_multi_client PRIVATE CURL::libcurl)
