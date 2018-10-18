cmake_minimum_required(VERSION 3.10.0)
project(loader C)
enable_language(ASM)

# use the target toolchain instead of the host's
target_toolchain()

include_directories(
    SYSTEM "${CMAKE_SOURCE_DIR}/include"
    "include"
)

include(CheckCCompilerFlag)

# compiler checking
check_c_compiler_flag(-std=gnu11 HAVE_GNU11)
if (NOT HAVE_GNU11)
    message(FATAL_ERROR "Compiler {CMAKE_C_COMPILER} does not support C11 standard (with GNU extensions)")
endif (NOT HAVE_GNU11)

set(CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/config)
set(CMAKE_MODULE_PATH ${CONFIG_DIR})

# target isa/platform configuration
include(${TARGET_ISA}/config OPTIONAL)
include(${TARGET_ISA}/${TARGET_PLATFORM}/config)

# get our source files
# these files contain a list of all source files to compile, one file per line
# this is used instead of globbing, for various reasons.
# ./config/<isa>/sources             : ISA-specific source files
# ./config/<isa>/<platform>/sources  : Platform-specific source files
# ./config/sources                   : cross-platform source files

set(ISA_SOURCES ${CONFIG_DIR}/${TARGET_ISA}/sources)
set(PLATFORM_SOURCES ${CONFIG_DIR}/${TARGET_ISA}/${TARGET_PLATFORM}/sources)
set(SOURCES ${CONFIG_DIR}/sources)

file(STRINGS ${ISA_SOURCES} LOADER_ISA_SRC)
file(STRINGS ${PLATFORM_SOURCES} LOADER_PLATFORM_SRC)
file(STRINGS ${SOURCES} LOADER_SRC)

# these will allow the build system to automatically reconfigure cmake if any
# of the sources lists get modified.
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${ISA_SOURCES})
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${PLATFORM_SOURCES})
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${SOURCES})

#set_property(DIRECTORY)


if(OPT_DISK_GPT)
    add_definitions(-DOPT_DISK_GPT)
endif(OPT_DISK_GPT)

if(OPT_DISK_MBR)
   add_definitions(-DOPT_DISK_MBR)
endif(OPT_DISK_MBR)

# compiler flags
option(LOADER_WERROR "WERROR override for loader project" ${ENABLE_WERROR})
set(LOADER_WARNFLAGS ${WARNFLAGS} CACHE STRING "GCC Warning enable flags for loader project")
set(LOADER_CFLAGS "-fdiagnostics-color=always -std=gnu11 -ffreestanding" CACHE STRING "C compiler flags for loader")
if(LOADER_WERROR)
    set(WERROR "-Werror")
else(LOADER_WERROR)
    set(WERROR "")
endif(LOADER_WERROR)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${LOADER_CFLAGS} ${WERROR} ${LOADER_WARNFLAGS}")
#set(CMAKE_EXE_LINKER_FLAGS "")

# pass on the version string to version.c
set_source_files_properties(src/version.c 
    PROPERTIES
    COMPILE_DEFINITIONS
    "VERSION=\"${VERSION}\""
)

# targets
set(LOADER_ELF loader.elf)  # final loader executable as an ELF binary
set(LOADER_BIN loader.bin)  # final loader executable as a flat binary


add_executable(
    ${LOADER_ELF}
    ${LOADER_ENTRY}
    ${LOADER_PLATFORM_SRC}
    ${LOADER_ISA_SRC}
    ${LOADER_SRC}
    ${LOADER_LD_SCRIPT}
)
set_target_properties(
    ${LOADER_ELF}
    PROPERTIES
    LINK_FLAGS "-T ${LOADER_LD_SCRIPT} -nostdlib"
)

add_custom_command(
    OUTPUT ${LOADER_BIN}
    COMMAND ${GCC_LD} -o ${LOADER_BIN} -T ${LOADER_LD_SCRIPT} -s --oformat binary ${LOADER_ELF}
    COMMENT "Linking flat binary loader.bin"
    DEPENDS ${LOADER_ELF}
)

add_custom_target(loader ALL
    DEPENDS ${LOADER_BIN} ${BOOTSTRAP_BIN}
)
