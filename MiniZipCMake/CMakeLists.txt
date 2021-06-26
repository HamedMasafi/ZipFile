cmake_minimum_required(VERSION 3.5)

project(MiniZipLibrary LANGUAGES C)

set(MINIZIP_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/minizip)
set(ZLIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/zlib)

file(GLOB MINIZIP_SOURCE_FILES 
  ${MINIZIP_PATH}/ioapi.c
  ${MINIZIP_PATH}/ioapi_buf.c
  ${MINIZIP_PATH}/ioapi_mem.c
  ${MINIZIP_PATH}/unzip.c
  ${MINIZIP_PATH}/zip.c
)
file(GLOB MINIZIP_HEADER_FILES 
  ${MINIZIP_PATH}/crypt.h
  ${MINIZIP_PATH}/ioapi.h
  ${MINIZIP_PATH}/ioapi_buf.h
  ${MINIZIP_PATH}/ioapi_mem.h
  ${MINIZIP_PATH}/unzip.h
  ${MINIZIP_PATH}/zip.h
)
file(GLOB ZLIB_SOURCE_FILES
  ${ZLIB_PATH}/adler32.c
  ${ZLIB_PATH}/compress.c
  ${ZLIB_PATH}/crc32.c
  ${ZLIB_PATH}/deflate.c
  ${ZLIB_PATH}/gzclose.c
  ${ZLIB_PATH}/gzlib.c
  ${ZLIB_PATH}/gzread.c
  ${ZLIB_PATH}/gzwrite.c
  ${ZLIB_PATH}/infback.c
  ${ZLIB_PATH}/inffast.c
  ${ZLIB_PATH}/inflate.c
  ${ZLIB_PATH}/inftrees.c
  ${ZLIB_PATH}/trees.c
  ${ZLIB_PATH}/uncompr.c
  ${ZLIB_PATH}/zutil.c
)
file(GLOB ZLIB_HEADER_FILES
  ${ZLIB_PATH}/crc32.h
  ${ZLIB_PATH}/deflate.h
  ${ZLIB_PATH}/gzguts.h
  ${ZLIB_PATH}/inffast.h
  ${ZLIB_PATH}/inffixed.h
  ${ZLIB_PATH}/inflate.h
  ${ZLIB_PATH}/inftrees.h
  ${ZLIB_PATH}/trees.h
  ${ZLIB_PATH}/zconf.h
  ${ZLIB_PATH}/zlib.h
  ${ZLIB_PATH}/zutil.h
)

if(WIN32)
  list(APPEND MINIZIP_HEADER_FILES ${MINIZIP_PATH}/iowin32.h)
  list(APPEND MINIZIP_SOURCE_FILES ${MINIZIP_PATH}/iowin32.c)
endif(WIN32)

add_library(${PROJECT_NAME} SHARED
  ${ZLIB_SOURCE_FILES}
  ${ZLIB_HEADER_FILES}
  ${MINIZIP_HEADER_FILES}
  ${MINIZIP_SOURCE_FILES}
)

add_library(_helper::minizip::library ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
  ${MINIZIP_PATH}
  ${ZLIB_PATH}
)
