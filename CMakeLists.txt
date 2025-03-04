cmake_minimum_required(VERSION 3.5.0)
project(Battleship VERSION 1.0.0 LANGUAGES C CXX)

SET(CMAKE_CXX_STANDARD 23)

add_subdirectory(src)

enable_testing()
