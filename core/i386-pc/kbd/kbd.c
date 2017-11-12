
#ifdef FE_MENU

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

#ifdef DEBUG_KBD_PARSE
#include <SPLoader/console/out.h>
#endif

static void __kbd_isr(int vector, int code);

// #define SCANCODE_BUFSIZE 32
// static unsigned _scancodeBufIndex;
// static uint8_t _scancodeBuf[SCANCODE_BUFSIZE];

static bool _eventFlag;
static KeyEvent _lastEvent;
static KeyFlags _flags;

static ScancodePacket _packet;
static ScancodeParseFn _parseFn;


int spl_kbd_init(void) {
    //_scancodeBufIndex = 0;
    _eventFlag = false;
    _packet.length = 0;
    _parseFn = spl_kbd_parse_set1;
    _flags = 0;
    __isr_install(INT_VEC_KEYBOARD, __kbd_isr, NULL);

    return E_SUCCESS;
}

int spl_kbd_waitForEvent(KeyEvent *evt) {
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

    #ifdef DEBUG_KBD_PARSE
        con_printf("%02x ", scancode);
    #endif

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
            #ifdef DEBUG_KBD_PARSE
                con_puts(": RESPONSE\n");
            #endif
            _packet.length = 0;
            break;
        default:
            _packet.buffer[_packet.length++] = scancode;
            result = _parseFn(&_packet, &evt);
            switch (result) {
                case KBD_PARSE_COMPLETE:
                    // scancode was complete, save event data
                    _lastEvent = evt;
                    _eventFlag = true;
                    #ifdef DEBUG_KBD_PARSE
                        con_puts(": COMPLETE\n");
                    #endif
                    _packet.length = 0;
                    break;
                case KBD_PARSE_DISCARD:
                    #ifdef DEBUG_KBD_PARSE
                        con_puts(": DISCARD\n");
                    #endif
                    _packet.length = 0;
                    break;
                case KBD_PARSE_ERROR:
                    #ifdef DEBUG_KBD_PARSE
                        con_puts(": ERROR\n");
                    #endif
                    _packet.length = 0;
                    break;
                case KBD_PARSE_INCOMPLETE:
                    // incomplete scancode, nothing to do
                    break;
            }
            break;
    }

    if (_eventFlag) {
        bool keypressed = (_lastEvent.flags & KEY_FLAG_PRESSED) == KEY_FLAG_PRESSED;
        bool modifier = false;
        bool toggle = false;
        KeyFlags mask;

        switch (_lastEvent.key) {
            case KEY_LSHIFT:
            case KEY_RSHIFT:
                mask = KEY_FLAG_SHIFT;
                modifier = true;
                break;
            case KEY_LCTRL:
            case KEY_RCTRL:
                mask = KEY_FLAG_CTRL;
                modifier = true;
                break;
            case KEY_LALT:
            case KEY_RALT:
                mask = KEY_FLAG_ALT;
                modifier = true;
                break;
            case KEY_NUM_LOCK:
                mask = KEY_FLAG_NUM_LOCK;
                toggle = true;
                break;
            case KEY_CAPS_LOCK:
                mask = KEY_FLAG_CAPS_LOCK;
                toggle = true;
                break;
            case KEY_SCROLL_LOCK:
                mask = KEY_FLAG_SCROLL_LOCK;
                toggle = true;
                break;
            default:
                break;
        }

        if (modifier) {
            if (keypressed) {
                _flags |= mask;  // set the modifier
            } else {
                _flags &= ~mask; // clear the modifier
            }
        }

        if (toggle) {
            if (keypressed) {
                _flags ^= mask;  // toggle the lock
            }
        }
        _lastEvent.flags |= _flags; // add the current flags to the latest event
    }

    // if (_scancodeBufIndex != SCANCODE_BUFSIZE) {
        
    //     //con_printf("%02x ", scancode);
    //     _scancodeBuf[_scancodeBufIndex++] = scancode;
    // }


    __outb(PIC_MASTER_CMD_PORT, PIC_EOI);
}


#else
typedef int dummy;
#endif // FE_MENU
