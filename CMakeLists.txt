cmake_minimum_required (VERSION 4.1.1)

project("Snake")

find_package(PThreads4W CONFIG REQUIRED)

add_executable(Snake src/main.c)

target_link_libraries(Snake PRIVATE PThreads4W::PThreads4W)

include(Warnings.cmake)

set_property(TARGET Snake PROPERTY C_STANDARD 99)
