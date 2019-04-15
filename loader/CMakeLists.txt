cmake_minimum_required(VERSION 3.10.0)
project(loader C)
enable_language(ASM)

# use the target toolchain instead of the host's
target_toolchain()

project_template()

list(APPEND CMAKE_MODULE_PATH "${CONFIG_DIR}/loader")

include(CheckCCompilerFlag)

# compiler checking
check_c_compiler_flag(-std=gnu11 HAVE_GNU11)
if (NOT HAVE_GNU11)
    message(FATAL_ERROR "Compiler ${CMAKE_C_COMPILER} does not support C11 standard (with GNU extensions)")
endif (NOT HAVE_GNU11)

# target isa/platform configuration
include(${TARGET_ISA}/config OPTIONAL)
include(${TARGET_ISA}/${TARGET_PLATFORM}/config)

# get our source files
# these files contain a list of all source files to compile, one file per line
# this is used instead of globbing, for various reasons.
# ./config/<isa>/sources             : ISA-specific source files
# ./config/<isa>/<platform>/sources  : Platform-specific source files
# ./config/sources                   : cross-platform source files
sources(LOADER_SRC          ${CONFIG_DIR}/loader/sources)
sources(LOADER_ISA_SRC      ${CONFIG_DIR}/loader/${TARGET_ISA}/sources)
sources(LOADER_PLATFORM_SRC ${CONFIG_DIR}/loader/${TARGET_ISA}/${TARGET_PLATFORM}/sources)

#set_property(DIRECTORY)


if(OPT_DISK_GPT)
    add_definitions(-DOPT_DISK_GPT)
endif(OPT_DISK_GPT)

if(OPT_DISK_MBR)
   add_definitions(-DOPT_DISK_MBR)
endif(OPT_DISK_MBR)

add_definitions(
    -DSPLOADERK
)

# compiler flags
set(BASE_FLAGS -std=gnu11 -ffreestanding)

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
target_compile_options(
    ${LOADER_ELF}
    PUBLIC ${BASE_FLAGS} ${LOADER_FLAGS} ${WERROR} ${LOADER_WARNFLAGS}
)
target_link_libraries(${LOADER_ELF} ${LIBSPLOADERK})

add_custom_command(
    OUTPUT ${LOADER_BIN}
    COMMAND ${GCC_LD} -o ${LOADER_BIN} -T ${LOADER_LD_SCRIPT} -s --oformat binary ${LOADER_ELF}
    COMMENT "Linking flat binary loader.bin"
    DEPENDS ${LOADER_ELF}
)

add_custom_target(loader ALL
    DEPENDS ${LOADER_BIN} ${BOOTSTRAP_BIN}
)

# let other projects know about these targets

set(LOADER_BIN ${CMAKE_CURRENT_BINARY_DIR}/${LOADER_BIN} CACHE INTERNAL "" FORCE)
if(BOOTSTRAP_BIN)
    set(BOOTSTRAP_BIN ${CMAKE_CURRENT_BINARY_DIR}/${BOOTSTRAP_BIN} CACHE INTERNAL "" FORCE)
endif(BOOTSTRAP_BIN)
