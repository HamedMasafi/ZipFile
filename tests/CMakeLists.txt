cmake_minimum_required(VERSION 3.5)

project(zipfile_test LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
 set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -O2")
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Test REQUIRED)

add_executable(${PROJECT_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/basic/tst_zipfiletest.h
  ${CMAKE_CURRENT_SOURCE_DIR}/basic/tst_zipfiletest.cpp
)
target_link_libraries(${PROJECT_NAME} PUBLIC
  utility::zipfile
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Test
)
target_compile_definitions(${PROJECT_NAME} PUBLIC
  PATH="${CMAKE_CURRENT_BINARY_DIR}"
)
