

#include <SPLoader/kbd/kbd.h>
#include <SPLoader/kbd/parse.h>
#include <SPLoader/kbd/ScancodeParseFn.h>
#include <SPLoader/err.h>

#include <SPLoader/i386-pc/idt/isr.h>
#include <SPLoader/i386-pc/pic.h>
#include <SPLoader/i386-pc/ps2/ps2.h>
#include <SPLoader/i386/io.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


static void __kbd_isr(int vector, int code);

// #define SCANCODE_BUFSIZE 32
// static unsigned _scancodeBufIndex;
// static uint8_t _scancodeBuf[SCANCODE_BUFSIZE];

static bool _eventFlag;
static KeyEvent _lastEvent;

static ScancodePacket _packet;
static ScancodeParseFn _parseFn;



int kbd_init(void) {
    //_scancodeBufIndex = 0;
    _eventFlag = false;
    _packet.length = 0;
    _parseFn = kbd_parse_set1;
    isr_install(INT_VEC_KEYBOARD, __kbd_isr, NULL);

    return E_SUCCESS;
}

int kbd_waitForEvent(KeyEvent *evt) {
    if (evt == NULL) {
        return E_ARGNULL;
    }

    _eventFlag = false;
    do {
        asm("hlt");
    } while (!_eventFlag);

    *evt = _lastEvent;
    //_eventFlag = false;

    // _scancodeBufIndex = 0;
    // do {
    //     asm ("hlt");
    // } while (_scancodeBufIndex == 0);

    // uint8_t scancode = _scancodeBuf[0];
    // bool released = (scancode & 0x80) == 0x80;
    // scancode &= 0x7f;
    // if (scancode < 0x59) {
    //     evt->key = _SCANCODE_SET1[scancode];
    //     if (released) {
    //         evt->flags = 0;
    //     } else {
    //         evt->flags = KEY_FLAG_PRESSED;
    //     }
    // }


    return E_SUCCESS;
}



void __kbd_isr(int vector, int code) {
    (void)vector; (void)code;

    uint8_t scancode = __inb(PS2_PORT_DATA);
    __outb(0x80, 0);

    //con_printf("%02x ", scancode);

    int result;
    KeyEvent evt;

    switch (scancode) {
        case PS2_RB_ERROR1:
        case PS2_RB_ERROR2:
        case PS2_RB_RESEND:
        case PS2_RB_ECHO:
        case PS2_RB_ACK:
            // these response bytes are sent from the keyboard in response to
            // a command, or due to an error. Since we do not support sending
            // commands at the moment, any response other than a scancode is
            // to be considered an error. Reset the scancode buffer
            _packet.length = 0;
            break;
        default:
            _packet.buffer[_packet.length++] = scancode;
            result = _parseFn(&_packet, &evt);
            switch (result) {
                case KBD_PARSE_COMPLETE:
                    _lastEvent = evt;
                    _eventFlag = true;
                    //con_putchar('C');
                    // fall-through
                case KBD_PARSE_ERROR:
                    _packet.length = 0;
                    //con_putchar('\n');
                    break;
                case KBD_PARSE_INCOMPLETE:
                    //con_puts("I ");
                    break;
            }
            break;
    }

    // if (_scancodeBufIndex != SCANCODE_BUFSIZE) {
        
    //     //con_printf("%02x ", scancode);
    //     _scancodeBuf[_scancodeBufIndex++] = scancode;
    // }


    __outb(PIC_MASTER_CMD_PORT, PIC_EOI);
}