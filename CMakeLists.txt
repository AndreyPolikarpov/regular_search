cmake_minimum_required(VERSION 3.10)

project(find_regual)

#нужно обновить Cmake что бы скачать gtest
#include(FetchContent)
#FetchContent_Declare(
#  googletest
  # Specify the commit you depend on and update it regularly.
#  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
#)
# For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)

# Basic compiler and cmake configurations
#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)
#set(CONCENTRATOR_BASE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/googletest
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#//To Do файлы завернуть в set что бы одно и тоже несколько раз не пеатать
add_executable(find_regual main.cpp 
              tree_regular.cpp
              storage_tnode.cpp
              tree_node.cpp
              search_tree.cpp
              )

add_executable(test_regular test.cpp
              tree_regular.cpp
              storage_tnode.cpp
              tree_node.cpp
              search_tree.cpp
              )

include_directories(${GTEST_INCLUDE_DIRS})

set_target_properties(find_regual test_regular PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON) 

target_include_directories(find_regual
    PRIVATE "${CMAKE_BINARY_DIR}")

set_target_properties(test_regular PROPERTIES
INCLUDE_DIRECTORIES ${gtest_SOURCE_DIR})

#target_link_libraries(ip_filter_cli PRIVATE ip_filter)
target_link_libraries(test_regular ${gtest_LIBRARIES} gtest gtest_main)

#target_compile_options(ip_filter_cli PRIVATE
#    -Wall -Wextra -pedantic -Werror  -Wno-unused-parameter) 
target_compile_options(find_regual PRIVATE
    -Wall -Wextra -pedantic -Werror  -Wno-unused-parameter)
target_compile_options(test_regular PRIVATE
    -Wall -Wextra -pedantic -Werror  -Wno-unused-parameter)