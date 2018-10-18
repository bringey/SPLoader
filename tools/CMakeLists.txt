cmake_minimum_required(VERSION 3.0.0)
project(tools C)


include_directories(
    SYSTEM "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/loader/include"
)

set(MKIMAGE "mkimage")
set(MKIMAGE_SRC "mkimage/mkimage.c")

add_executable(
    ${MKIMAGE}
    ${MKIMAGE_SRC}
)
