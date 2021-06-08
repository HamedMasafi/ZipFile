cmake_minimum_required(VERSION 3.5)

project(zipfile LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
 set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -O2")
endif()

add_subdirectory(tests)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/MiniZipCMake)

add_library(${PROJECT_NAME} SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/src/zipfile.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/zipfile.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/zipfile_global.h
)
add_library(utility::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC
  ${MINIZIP_PATH}
  ${ZLIB_PATH}
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)
target_link_libraries(${PROJECT_NAME} PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  _helper::minizip::library
)
target_compile_definitions(${PROJECT_NAME} PUBLIC ZIP_LIB_LIBRARY)
