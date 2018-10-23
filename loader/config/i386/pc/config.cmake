
# enable gpt and mbr support
set(OPT_DISK_GPT ON)
set(OPT_DISK_MBR ON)

set(LOADER_LD_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/src/i386-pc/loader.lds)

set(LOADER_ENTRY "src/i386-pc/stage0b/early.S")

set(BOOTSTRAP_SRC "src/i386-pc/stage0a/bootstrap.S")

# targets
# the main CMakeLists.txt will build ${BOOTSTRAP_BIN} if it is defined
set(BOOTSTRAP_ELF "bootstrap.elf")
set(BOOTSTRAP_BIN "bootstrap.bin")

add_executable(${BOOTSTRAP_ELF} ${BOOTSTRAP_SRC})
set_target_properties(
    ${BOOTSTRAP_ELF}
    PROPERTIES
    LINK_FLAGS "-Wl,-Ttext,0x0,-e,begtext -nostdlib"
)

add_custom_command(
    OUTPUT ${BOOTSTRAP_BIN}
    COMMAND ${GCC_LD} -Ttext 0x0 -s --oformat binary -e begtext -o ${BOOTSTRAP_BIN} ${BOOTSTRAP_ELF}
    COMMENT "Creating flat binary bootstrap.bin"
    DEPENDS ${BOOTSTRAP_ELF}
)