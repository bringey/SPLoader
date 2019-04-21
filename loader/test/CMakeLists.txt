project("test_loader" C)

sources(LOADER_STUBS_SRC ${CONFIG_DIR}/loader/test/loaderstubs.sources)
set(LOADER_STUBS_LIB "loaderstubs")

add_library(${LOADER_STUBS_LIB} STATIC ${LOADER_STUBS_SRC})

link_libraries(${LOADER_STUBS_LIB})

set(LOADER-TEST_CFLAGS -std=gnu11)
project_template()
test_template("loader")

# any complex tests that cannot be created by the template should go here
