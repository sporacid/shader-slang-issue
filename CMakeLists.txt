cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 23)

project(slang-issue)

add_executable(slang-issue src/main.cpp)

find_path(SPORE_SLANG_INCLUDE_DIRS slang.h)
target_include_directories(slang-issue PRIVATE ${SPORE_SLANG_INCLUDE_DIRS})

find_library(SPORE_SLANG_LIB_DIRS slang)
target_link_libraries(slang-issue PRIVATE ${SPORE_SLANG_LIB_DIRS})