cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(find_regual VERSION ${PROJECT_VERSION})

configure_file(version.h.in version.h)

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

#//To Do файлы завернуть в set что бы одно и тоже несколько раз не пеатать
set(SOURS_REGULAR  #main.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/source/tree_regular.cpp
${CMAKE_CURRENT_SOURCE_DIR}/source/storage_tnode.cpp
${CMAKE_CURRENT_SOURCE_DIR}/source/tree_node.cpp
${CMAKE_CURRENT_SOURCE_DIR}/source/search_tree.cpp
      )

set(INCLUDE_REGULAR  #main.cpp 
      ${CMAKE_CURRENT_SOURCE_DIR}/include/tree_regular.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/include/storage_tnode.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/include/tree_node.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/include/search_tree.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/include/tree_search_engine.hpp
)

add_executable(find_regual_cli main.cpp)
add_library(find_regual SHARED ${SOURS_REGULAR} ${INCLUDE_REGULAR})
add_executable(test_regular ${CMAKE_CURRENT_SOURCE_DIR}/source/test.cpp 
              )

set_target_properties(find_regual find_regual_cli  test_regular  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON) 

target_include_directories(find_regual
    PRIVATE "${CMAKE_BINARY_DIR}")

target_include_directories(find_regual 
    PRIVATE include source)
  

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER tree_search_engine.hpp)

target_include_directories(test_regular 
    PRIVATE include source)

set_target_properties(test_regular PROPERTIES
    INCLUDE_DIRECTORIES "${gtest_SOURCE_DIR}" 
    INCLUDE_DIRECTORIES  ${CMAKE_CURRENT_SOURCE_DIR}/include
    INCLUDE_DIRECTORIES  ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_link_libraries(find_regual_cli PRIVATE find_regual)
target_link_libraries(test_regular ${gtest_LIBRARIES} find_regual gtest gtest_main)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/googletest
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

target_compile_options(find_regual_cli PRIVATE
    -Wall -Wextra -pedantic -Werror  -Wno-unused-parameter) 
target_compile_options(find_regual PRIVATE
    -Wall -Wextra -pedantic -Werror  -Wno-unused-parameter)
target_compile_options(test_regular PRIVATE
    -Wall -Wextra -pedantic -Werror  -Wno-unused-parameter)

install(TARGETS find_regual_cli RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT examle@example.com)

include(CPack)

enable_testing()

add_test(test_regular test_regular)