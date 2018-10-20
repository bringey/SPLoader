
set(TARGET_PREFIX "i686-elf")

set(USB_IMG "usb.img")

macro(custom_targets)

    add_custom_command(
        OUTPUT ${USB_IMG}
        COMMAND ${MKIMAGE} -b ${BOOTSTRAP_BIN} ${LOADER_BIN} -o ${USB_IMG}
        COMMENT "Making bootable usb image"
        DEPENDS ${MKIMAGE} ${BOOTSTRAP_BIN} ${LOADER_BIN}
    )

    add_custom_target(
        "usb"
        DEPENDS ${USB_IMG}
    )

endmacro(custom_targets)