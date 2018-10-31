cmake_minimum_required(VERSION 3.10.0)
project(tools C)

set(TOOLS_CFLAGS -std=gnu11)
project_template()

sources(MKIMAGE_SRC ${CONFIG_DIR}/mkimage.sources)
sources(MKIMAGE2_SRC ${CONFIG_DIR}/mkimage2.sources)
sources(MKBIN_SRC ${CONFIG_DIR}/mkbin.sources)
sources(COMMON_SRC ${CONFIG_DIR}/common.sources)

#
# TARGETS
#
set(MKBIN "mkbin")
set(MKIMAGE "mkimage")
set(MKIMAGE2 "mkimage2")
set(COMMON "common")

add_library(
    ${COMMON}
    OBJECT ${COMMON_SRC}
)

add_executable(
    ${MKBIN}
    ${MKBIN_SRC}
    $<TARGET_OBJECTS:${COMMON}>
)
target_compile_options(
    ${MKBIN}
    PUBLIC ${TOOLS_CFLAGS} ${WERROR} ${TOOLS_WARNFLAGS}
)
target_link_libraries(${MKBIN} ${LIBSPLOADER})

add_executable(
    ${MKIMAGE}
    ${MKIMAGE_SRC}
)
target_compile_options(
    ${MKIMAGE}
    PUBLIC ${TOOLS_CFLAGS} ${WERROR} ${TOOLS_WARNFLAGS}
)

add_executable(
    ${MKIMAGE2}
    ${MKIMAGE2_SRC}
    $<TARGET_OBJECTS:${COMMON}>
)
target_compile_options(
    ${MKIMAGE2}
    PUBLIC ${TOOLS_CFLAGS} ${WERROR} ${TOOLS_WARNFLAGS}
)

set(MKIMAGE ${CMAKE_CURRENT_BINARY_DIR}/${MKIMAGE} CACHE INTERNAL "" FORCE)
set(MKIMAGE2 ${CMAKE_CURRENT_BINARY_DIR}/${MKIMAGE2} CACHE INTERNAL "" FORCE)
set(MKBIN ${CMAKE_CURRENT_BINARY_DIR}/${MKBIN} CACHE INTERNAL "" FORCE)
