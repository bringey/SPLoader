project("loader-test" C)

set(LOADER-TEST_CFLAGS -std=gnu11)
project_template()

# allow loader's src directory as an include path
# for including c files into a test program
include_directories("../")

sources(LOADER_STUBS_SRC ${CONFIG_DIR}/loader/test/loaderstubs.sources)
set(LOADER_STUBS_LIB "loaderstubs")

add_library(${LOADER_STUBS_LIB} STATIC ${LOADER_STUBS_SRC})

# loader tests

function(add_loader_test testname)
    set(test_target "loader_${testname}")
    add_test_exe(${testname})
    target_link_libraries(${test_target} ${LOADER_STUBS_LIB})
    # export our created variables to the parent scope
    string(TOUPPER ${testname} testname_upper)
    set(LOADER_${testname_upper} ${test_target} PARENT_SCOPE)
endfunction(add_loader_test)

add_loader_test(test_disk)
add_loader_test(test_disk_gpt)
add_loader_test(test_disk_mbr)
add_loader_test(test_mem)
add_loader_test(test_printf)
add_loader_test(test_string)
