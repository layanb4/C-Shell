cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(shell src/shellmain.c src/shellfunc.c)
project(CMakeShellProject VERSION 1.0 DESCRIPTION "This is a CMake shell proj." LANGUAGES C CXX)
