project(test C)

set(TEST_CFLAGS -std=gnu11)
project_template()

# add the entire project directory to the include path
# allow the test programs to include c source files
include_directories("${CMAKE_SOURCE_DIR}")

sources(LOADER_STUBS_SRC ${CONFIG_DIR}/loader/loaderstubs.sources)
set(LOADER_STUBS_LIB "loaderstubs")

add_library(${LOADER_STUBS_LIB} STATIC ${LOADER_STUBS_SRC})

# loader tests

function(add_loader_test testname)
    set(test_target "loader_${testname}")
    sources(test_src ${CONFIG_DIR}/loader/${testname}.sources)
    add_executable(
        ${test_target}
        ${test_src}
    )
    target_link_libraries(
        ${test_target}
        ${LOADER_STUBS_LIB}
        ${CMOCKA_LIB}
    )
    add_test(
        NAME "${test_target}"
        COMMAND ${test_target}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    # export our created variables to the parent scope
    string(TOUPPER ${testname} testname_upper)
    set(LOADER_${testname_upper} ${test_target} PARENT_SCOPE)
endfunction(add_loader_test)

sources(LOADER_TESTS ${CONFIG_DIR}/loader_tests)
foreach(testname ${LOADER_TESTS})
    add_loader_test(${testname})
endforeach()

