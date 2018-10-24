cmake_minimum_required(VERSION 3.10.0)
project(libsploaderk C)

project_template()

target_toolchain()

set(SOURCES "${CMAKE_SOURCE_DIR}/libsploader/config/sources")
file(STRINGS ${SOURCES} LIBSPLOADERK_SRC)
prepend(LIBSPLOADERK_SRC "${CMAKE_SOURCE_DIR}/libsploader/" ${LIBSPLOADERK_SRC})
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${SOURCES})

set(LIBSPLOADERK "sploaderk" CACHE INTERNAL "" FORCE)

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