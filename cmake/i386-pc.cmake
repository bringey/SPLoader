
set(TARGET_PREFIX "i686-elf")

set(STAGE1 "stage1")
set(USB_IMG "usb.img")

macro(custom_targets)

    add_custom_command(
        OUTPUT ${STAGE1}
        COMMAND ${MKBIN} ${LOADER_BIN} -o ${STAGE1} --mbr
        COMMENT "Making stage 1 binary"
        DEPENDS ${MKBIN} ${LOADER_BIN}
    )

    add_custom_command(
        OUTPUT ${USB_IMG}
        COMMAND ${MKIMAGE} -b ${BOOTSTRAP_BIN} ${STAGE1} -o ${USB_IMG}
        COMMENT "Making bootable usb image"
        DEPENDS ${MKIMAGE} ${BOOTSTRAP_BIN} ${STAGE1}
    )

    add_custom_target(
        "usb"
        DEPENDS ${USB_IMG}
    )

    # Include MBR/GPT parsing in the lib
    target_compile_definitions(
        ${LIBSPLOADERK} PUBLIC
        -DSPL_OPT_MBR
        -DSPL_OPT_GPT
    )

endmacro(custom_targets)