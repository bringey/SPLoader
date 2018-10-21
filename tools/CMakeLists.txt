cmake_minimum_required(VERSION 3.10.0)
project(tools C)


include_directories(
    SYSTEM "${CMAKE_SOURCE_DIR}/include"
)

#
# CACHE Vars
#
option(TOOLS_WERROR "WERROR Override for tools project" ${ENABLE_WERROR})
set(TOOLS_CFLAGS "" CACHE STRING "C compiler flags for tools")

if (TOOLS_WERROR)
    set(WERROR -Werror)
else (TOOLS_WERROR)
    set(WERROR "")
endif (TOOLS_WERROR)

set(MKIMAGE "mkimage")
set(MKIMAGE_SRC "mkimage/mkimage.c")

add_executable(
    ${MKIMAGE}
    ${MKIMAGE_SRC}
)
target_compile_options(
    ${MKIMAGE}
    PUBLIC ${TOOLS_CFLAGS} ${WERROR}
)

set(MKIMAGE ${CMAKE_CURRENT_BINARY_DIR}/${MKIMAGE} CACHE INTERNAL "" FORCE)
