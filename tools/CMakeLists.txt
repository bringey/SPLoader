cmake_minimum_required(VERSION 3.10.0)
project(tools C)

project_template()

include_directories(
    SYSTEM "${CMAKE_SOURCE_DIR}/include"
)

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
