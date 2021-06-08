
MINIZIP_PATH=$$PWD/3rdparty/minizip
ZLIB_PATH=$$PWD/3rdparty/zlib

INCLUDEPATH += $$PWD/src $$MINIZIP_PATH $$ZLIB_PATH

SOURCES += \
    $$MINIZIP_PATH/ioapi.c \
    $$MINIZIP_PATH/ioapi_buf.c \
    $$MINIZIP_PATH/ioapi_mem.c \
    $$MINIZIP_PATH/unzip.c \
    $$MINIZIP_PATH/zip.c \
    $$PWD/src/zipfile.cpp

HEADERS += \
    $$MINIZIP_PATH/crypt.h \
    $$MINIZIP_PATH/ioapi.h \
    $$MINIZIP_PATH/ioapi_buf.h \
    $$MINIZIP_PATH/ioapi_mem.h \
    $$MINIZIP_PATH/unzip.h \
    $$MINIZIP_PATH/zip.h \
    $$PWD/src/zipfile.h \
    $$PWD/src/zipfile_global.h

win32: {
    HEADERS += $$MINIZIP_PATH/iowin32.h
    SOURCES += $$MINIZIP_PATH/iowin32.c
}

SOURCES += \
    $$ZLIB_PATH/adler32.c \
    $$ZLIB_PATH/compress.c \
    $$ZLIB_PATH/crc32.c \
    $$ZLIB_PATH/deflate.c \
    $$ZLIB_PATH/gzclose.c \
    $$ZLIB_PATH/gzlib.c \
    $$ZLIB_PATH/gzread.c \
    $$ZLIB_PATH/gzwrite.c \
    $$ZLIB_PATH/infback.c \
    $$ZLIB_PATH/inffast.c \
    $$ZLIB_PATH/inflate.c \
    $$ZLIB_PATH/inftrees.c \
    $$ZLIB_PATH/trees.c \
    $$ZLIB_PATH/uncompr.c \
    $$ZLIB_PATH/zutil.c

HEADERS += \
    $$ZLIB_PATH/crc32.h \
    $$ZLIB_PATH/deflate.h \
    $$ZLIB_PATH/gzguts.h \
    $$ZLIB_PATH/inffast.h \
    $$ZLIB_PATH/inffixed.h \
    $$ZLIB_PATH/inflate.h \
    $$ZLIB_PATH/inftrees.h \
    $$ZLIB_PATH/trees.h \
    $$ZLIB_PATH/zconf.h \
    $$ZLIB_PATH/zlib.h \
    $$ZLIB_PATH/zutil.h
