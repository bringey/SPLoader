cmake_minimum_required(VERSION 3.10.0)
project(libsploader C)

project_template()

include_directories(
    SYSTEM "${CMAKE_SOURCE_DIR}/include"
)

set(SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/config/sources")
file(STRINGS ${SOURCES} LIBSPLOADER_SRC)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${SOURCES})

set(LIBSPLOADER "sploader")

add_library(
    ${LIBSPLOADER}
    STATIC
    ${LIBSPLOADER_SRC}
)
target_compile_options(
    ${LIBSPLOADER}
    PUBLIC ${LIBSPLOADER_CFLAGS} ${WARNFLAGS} ${WERROR}
)