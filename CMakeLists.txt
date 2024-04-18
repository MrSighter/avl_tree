cmake_minimum_required(VERSION 3.20.2)
project(avl_tree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_EXE_LINKER_FLAGS "-static")
add_executable(avl_tree main.cpp avl_tree_test.cpp)


#target_compile_options(avl_tree PRIVATE -O3 -march=native -s)