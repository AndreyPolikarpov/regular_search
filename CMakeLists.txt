cmake_minimum_required(VERSION 3.20)

project(find_regual)

# Basic compiler and cmake configurations
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)
#set(CONCENTRATOR_BASE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(find_regual main.cpp tree_regular.cpp)