cmake_minimum_required(VERSION 3.22)
project(example_module)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
enable_testing()
#
#set(CMAKE_CXX_STANDARD 17)
#message(STATUS "CXX version: ${CMAKE_CXX_STANDARD}")
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR} ${CMAKE_PREFIX_PATH})

find_package(GTest REQUIRED)
find_package(SystemCLanguage REQUIRED)
add_compile_options(-fdiagnostics-color)

add_compile_options(-Werror -Wall -Wextra -Wvla -Wno-unused-parameter)

add_subdirectory(src)
add_subdirectory(test)

add_custom_target(ctest
    COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process -T test --no-compress-output
    USES_TERMINAL)
message(STATUS "Install directory: ${CMAKE_INSTALL_INCLUDEDIR}")

install(TARGETS example_module TYPE LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
install(DIRECTORY include/example_module DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
