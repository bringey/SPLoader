cmake_minimum_required(VERSION 3.10.0)
project(libsploader C)

project_template()

set(SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/config/sources")
file(STRINGS ${SOURCES} LIBSPLOADER_SRC)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${SOURCES})

set(LIBSPLOADER "sploader" CACHE INTERNAL "" FORCE)

add_library(
    ${LIBSPLOADER}
    STATIC
    ${LIBSPLOADER_SRC}
)
target_compile_options(
    ${LIBSPLOADER}
    PRIVATE ${LIBSPLOADER_CFLAGS} ${LIBSPLOADER_WARNFLAGS} ${WERROR}
)