cmake_minimum_required(VERSION 3.10.0)
project(tools C)

project_template()

sources(MKIMAGE_SRC ${CONFIG_DIR}/mkimage.sources)
sources(MKBIN_SRC ${CONFIG_DIR}/mkbin.sources)

#
# TARGETS
#
set(MKBIN "mkbin")
set(MKIMAGE "mkimage")

add_executable(
    ${MKBIN}
    ${MKBIN_SRC}
)
target_compile_options(
    ${MKBIN}
    PUBLIC ${TOOLS_CFLAGS} ${WERROR} ${TOOLS_WARNFLAGS}
)

add_executable(
    ${MKIMAGE}
    ${MKIMAGE_SRC}
)
target_compile_options(
    ${MKIMAGE}
    PUBLIC ${TOOLS_CFLAGS} ${WERROR} ${TOOLS_WARNFLAGS}
)

set(MKIMAGE ${CMAKE_CURRENT_BINARY_DIR}/${MKIMAGE} CACHE INTERNAL "" FORCE)
set(MKBIN ${CMAKE_CURRENT_BINARY_DIR}/${MKBIN} CACHE INTERNAL "" FORCE)
