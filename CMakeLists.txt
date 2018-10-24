cmake_minimum_required(VERSION 3.10.0)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL ERROR "in-source builds are not allowed")
endif()


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

    if (GCC_AR_NOTFOUND)
        message(FATAL ERROR "could not locate ${TARGET_PREFIX}-ar")
    endif (GCC_AR_NOTFOUND)
    if (GCC_AS_NOTFOUND)
        message(FATAL ERROR "could not locate ${TARGET_PREFIX}-as")
    endif (GCC_AS_NOTFOUND)
    if (GCC_CC_NOTFOUND)
        message(FATAL ERROR "could not locate ${TARGET_PREFIX}-gcc")
    endif (GCC_CC_NOTFOUND)
    if (GCC_CPP_NOTFOUND)
        message(FATAL ERROR "could not locate ${TARGET_PREFIX}-cpp")
    endif (GCC_CPP_NOTFOUND)
    if (GCC_NM_NOTFOUND)
        message(FATAL ERROR "could not locate ${TARGET_PREFIX}-nm")
    endif (GCC_NM_NOTFOUND)
    if (GCC_LD_NOTFOUND)
        message(FATAL ERROR "could not locate ${TARGET_PREFIX}-ld")
    endif (GCC_LD_NOTFOUND)
    if (GCC_OBJCOPY_NOTFOUND)
        message(FATAL ERROR "could not locate ${TARGET_PREFIX}-objcopy")
    endif (GCC_OBJCOPY_NOTFOUND)
    if (GCC_OBJDUMP_NOTFOUND)
        message(FATAL ERROR "could not locate ${TARGET_PREFIX}-objdump")
    endif (GCC_OBJDUMP_NOTFOUND)
    if (GCC_RANLIB_NOTFOUND)
        message(FATAL ERROR "could not locate ${TARGET_PREFIX}-ranlib")
    endif (GCC_RANLIB_NOTFOUND)

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


option(ENABLE_WERROR "Treat compiler warnings as errors" ON)
set(WARNFLAGS -Wall -Wextra -pedantic CACHE STRING "Default GCC Warning enable flags")

set(VERSION_FILE "VERSION")
file(READ ${VERSION_FILE} VERSION)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${VERSION_FILE})

function(prepend outputVar prefix)
    set(list "")
    foreach(i ${ARGN})
        list(APPEND list "${prefix}${i}")
    endforeach(i)
    set(${outputVar} "${list}" PARENT_SCOPE)
endfunction(prepend)

macro(project_template)

    set(CONFIG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/config")
    set(CMAKE_MODULE_PATH ${CONFIG_DIR})

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

add_subdirectory(libsploader)
add_subdirectory(libsploaderk)
add_subdirectory(loader)
add_subdirectory(tools)

custom_targets()

configure_file(qemu ${CMAKE_BINARY_DIR}/qemu COPYONLY)
