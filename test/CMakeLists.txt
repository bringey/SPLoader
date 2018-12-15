project(test C)

project_template()

sources(LOADER_COMMON_SRC ${CONFIG_DIR}/loader/common.sources)
set(LOADER_COMMON "loader_common")

add_library(${LOADER_COMMON} STATIC ${LOADER_COMMON_SRC})

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
        ${LOADER_COMMON}
        ${CMOCKA_LIB}
    )
    add_test(
        NAME "loader_${test_target}"
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

