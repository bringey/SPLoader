
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
        COMMAND ${MKIMAGE2} -b ${BOOTSTRAP_BIN} ${STAGE1} -o ${USB_IMG}
        COMMENT "Making bootable usb image"
        DEPENDS ${MKIMAGE2} ${BOOTSTRAP_BIN} ${STAGE1}
    )

    add_custom_target(
        "usb"
        DEPENDS ${USB_IMG}
    )

endmacro(custom_targets)