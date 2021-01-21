project(btree)
cmake_minimum_required(VERSION 3.14)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set(CMAKE_CXX_FLAGS
        "-g -std=c++11"
)
add_executable(btree_test btree_test.cc)
target_link_libraries(btree_test gtest)