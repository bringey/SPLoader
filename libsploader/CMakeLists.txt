cmake_minimum_required(VERSION 3.10.0)
project(libsploader C)

project_template()

sources(LIBSPLOADER_SRC ${CONFIG_DIR}/libsploader/sources)

add_definitions(
    -DSPL_OPT_MBR
    -DSPL_OPT_GPT
)

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