cmake_minimum_required(VERSION 3.10.0)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL ERROR "in-source builds are not allowed")
endif()

set(CONFIG_DIR "${CMAKE_SOURCE_DIR}/config")
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

project(sploader)

if(NOT DEFINED TARGET)
    set(TARGET "i386-pc")
endif(NOT DEFINED TARGET)

include(${TARGET})

# find the GCC cross-compiler for ${TARGET}
find_program(GCC_AR         ${TARGET_PREFIX}-ar)
find_program(GCC_AS         ${TARGET_PREFIX}-as)
find_program(GCC_CC         ${TARGET_PREFIX}-gcc)
find_program(GCC_CPP        ${TARGET_PREFIX}-cpp)
find_program(GCC_NM         ${TARGET_PREFIX}-nm)
find_program(GCC_LD         ${TARGET_PREFIX}-ld)
find_program(GCC_OBJCOPY    ${TARGET_PREFIX}-objcopy)
find_program(GCC_OBJDUMP    ${TARGET_PREFIX}-objdump)
find_program(GCC_RANLIB     ${TARGET_PREFIX}-ranlib)

function(target_toolchain)

    if (GCC_AR STREQUAL "GCC_AR-NOTFOUND")
        message(FATAL_ERROR "could not locate ${TARGET_PREFIX}-ar")
    endif ()
    if (GCC_AS STREQUAL "GCC_AS-NOTFOUND")
        message(FATAL_ERROR "could not locate ${TARGET_PREFIX}-as")
    endif ()
    if (GCC_CC STREQUAL "GCC_CC-NOTFOUND")
        message(FATAL_ERROR "could not locate ${TARGET_PREFIX}-gcc")
    endif ()
    if (GCC_CPP STREQUAL "GCC_CPP-NOTFOUND")
        message(FATAL_ERROR "could not locate ${TARGET_PREFIX}-cpp")
    endif ()
    if (GCC_NM STREQUAL "GCC_NM-NOTFOUND")
        message(FATAL_ERROR "could not locate ${TARGET_PREFIX}-nm")
    endif ()
    if (GCC_LD STREQUAL "GCC_LD-NOTFOUND")
        message(FATAL_ERROR "could not locate ${TARGET_PREFIX}-ld")
    endif ()
    if (GCC_OBJCOPY STREQUAL "GCC_OBJCOPY-NOTFOUND")
        message(FATAL_ERROR "could not locate ${TARGET_PREFIX}-objcopy")
    endif ()
    if (GCC_OBJDUMP STREQUAL "GCC_OBJDUMP-NOTFOUND")
        message(FATAL_ERROR "could not locate ${TARGET_PREFIX}-objdump")
    endif ()
    if (GCC_RANLIB STREQUAL "GCC_RANLIB-NOTFOUND")
        message(FATAL_ERROR "could not locate ${TARGET_PREFIX}-ranlib")
    endif ()

    set(CMAKE_AR ${GCC_AR} PARENT_SCOPE)
    set(CMAKE_RANLIB ${GCC_RANLIB} PARENT_SCOPE)
    set(CMAKE_C_COMPILER ${GCC_CC} PARENT_SCOPE)
    set(CMAKE_ASM_COMPILER ${GCC_CC} PARENT_SCOPE)

endfunction(target_toolchain)

string(REPLACE "-" ";" TARGET_LIST ${TARGET})
list(GET TARGET_LIST 0 TARGET_ISA)
list(GET TARGET_LIST 1 TARGET_PLATFORM)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()


option(ENABLE_TESTS "Enable unit testing" ON)
option(ENABLE_WERROR "Treat compiler warnings as errors" ON)
set(WARNFLAGS -Wall -Wextra -pedantic CACHE STRING "Default GCC Warning enable flags")

set(VERSION_FILE "VERSION")
file(READ ${VERSION_FILE} VERSION)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${VERSION_FILE})
set(PROJECT_VERSION ${VERSION})

function(prepend outputVar prefix)
    set(list "")
    foreach(i ${ARGN})
        list(APPEND list "${prefix}${i}")
    endforeach(i)
    set(${outputVar} "${list}" PARENT_SCOPE)
endfunction(prepend)

macro(project_template)

    set(testlist_src "${CONFIG_DIR}/${PROJECT_NAME}/testlist")
    if (EXISTS ${testlist_src})
        # this will fail for test projects (test_template handles it)
        sources(TESTLIST ${testlist_src})
        foreach (test ${TESTLIST})
            set(test_target "test_${PROJECT_NAME}_${test}")
            add_test(
                NAME ${test_target}
                COMMAND ${test_target}
            )
        endforeach ()
    endif()
    string(TOUPPER ${PROJECT_NAME} PREFIX)
    option(${PREFIX}_WERROR "WERROR override for ${PROJECT_NAME} project" ${ENABLE_WERROR})
    set(${PREFIX}_WARNFLAGS ${${PREFIX}_WARNFLAGS} ${WARNFLAGS} CACHE STRING "Warning flags for ${PROJECT_NAME}")
    set(${PREFIX}_CFLAGS ${${PREFIX}_CFLAGS} CACHE STRING "C compiler flags for ${PROJECT_NAME}")

    if (${PREFIX}_WERROR)
        set(WERROR -Werror)
    else()
        set(WERROR "")
    endif()

    unset(PREFIX)

endmacro(project_template)

function(sources var filename)

    file(STRINGS ${filename} SRC)
    set(${var} ${SRC} PARENT_SCOPE)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${filename})

endfunction(sources)

include_directories(
    SYSTEM "${CMAKE_SOURCE_DIR}/include"
    SYSTEM "${CMAKE_BINARY_DIR}/include"
)

if (ENABLE_TESTS)
    find_library(CMOCKA_LIB cmocka)
    if (CMOCKA_LIB STREQUAL "CMOCKA_LIB-NOTFOUND")
        message(FATAL_ERROR "libcmocka was not found")
    endif ()

    function(add_test_exe project testname)
        set(test_target "test_${project}_${testname}")
        set(test_srcfile "${CONFIG_DIR}/${project}/test/test_${testname}.sources")
        if (EXISTS ${test_srcfile})
            sources(test_src ${test_srcfile})
        else ()
            # no source file for this test exists, so it's a simple test
            # simple tests consist of a single source file
            set(test_src "test_${testname}.c")
        endif ()
        add_executable(
            ${test_target}
            ${test_src}
        )
        string(TOUPPER ${test_target} targetvar)
        set(${targetvar} ${test_target} CACHE INTERNAL "" FORCE PARENT_SCOPE)
    endfunction(add_test_exe)

    macro(test_template project)
        include_directories(
            "${CMAKE_SOURCE_DIR}/${project}"
        )
        link_libraries(
            ${CMOCKA_LIB}
        )
        set(testlist_src "${CONFIG_DIR}/${project}/testlist")
        if (EXISTS ${testlist_src})
            sources(TESTLIST ${testlist_src})
            set(p ${project})
            foreach (test ${TESTLIST})
                add_test_exe("${project}" "${test}")
            endforeach ()
            unset(p)
        endif ()
    endmacro(test_template)


    enable_testing()
    add_subdirectory(loader/test)
endif (ENABLE_TESTS)

add_subdirectory(libsploader)
add_subdirectory(libsploaderk)
add_subdirectory(loader)
add_subdirectory(tools)

custom_targets()

configure_file(qemu ${CMAKE_BINARY_DIR}/qemu COPYONLY)
configure_file(include/version.h.in ${CMAKE_BINARY_DIR}/include/version.h)
