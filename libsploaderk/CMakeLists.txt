cmake_minimum_required(VERSION 3.10.0)
project(libsploaderk C)

project_template()

target_toolchain()

sources(LIBSPLOADERK_SRC "${CONFIG_DIR}/libsploader/sources")
prepend(LIBSPLOADERK_SRC "${CMAKE_SOURCE_DIR}/libsploader/" ${LIBSPLOADERK_SRC})

set(LIBSPLOADERK "sploaderk" CACHE INTERNAL "" FORCE)

add_definitions(
    -DSPLOADERK
)

add_library(
    ${LIBSPLOADERK}
    STATIC
    ${LIBSPLOADERK_SRC}
)
target_compile_options(
    ${LIBSPLOADERK}
    PRIVATE -ffreestanding -std=gnu11 
        ${LIBSPLOADERK_CFLAGS}
        ${LIBSPLOADERK_WARNFLAGS}
        ${WERROR}
)