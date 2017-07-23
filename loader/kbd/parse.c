
#include <SPLoader/kbd/Key.h>
#include <SPLoader/kbd/parse.h>

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

//
// Lookup table for scancode set 1
// Scancodes in this lookup table are 1 byte long.
//
static uint8_t _SCANCODE_SET1[] = {
    KEY_RSVD,   KEY_ESCAPE,   KEY_1,           KEY_2,            // 0x00 - 0x03
    KEY_3,      KEY_4,        KEY_5,           KEY_6,            // 0x04 - 0x07
    KEY_7,      KEY_8,        KEY_9,           KEY_0,            // 0x08 - 0x0B
    KEY_MINUS,  KEY_EQUALS,   KEY_BACKSPACE,   KEY_TAB,          // 0x0C - 0x0F
    KEY_Q,      KEY_W,        KEY_E,           KEY_R,            // 0x10 - 0x13
    KEY_T,      KEY_Y,        KEY_U,           KEY_I,            // 0x14 - 0x17
    KEY_O,      KEY_P,        KEY_OPEN_BRACE,  KEY_CLOSE_BRACE,  // 0x18 - 0x1B
    KEY_ENTER,  KEY_LCTRL,    KEY_A,           KEY_S,            // 0x1C - 0x1F
    KEY_D,      KEY_F,        KEY_G,           KEY_H,            // 0x20 - 0x23
    KEY_J,      KEY_K,        KEY_L,           KEY_SEMICOLON,    // 0x24 - 0x27
    KEY_QUOTE,  KEY_TILDA,    KEY_LSHIFT,      KEY_BACKSLASH,    // 0x28 - 0x2B
    KEY_Z,      KEY_X,        KEY_C,           KEY_V,            // 0x2C - 0x2F
    KEY_B,      KEY_N,        KEY_M,           KEY_COMMA,        // 0x30 - 0x33
    KEY_PERIOD, KEY_SLASH,    KEY_RSHIFT,      KEY_NP_MULTIPLY,  // 0x34 - 0x37
    KEY_LALT,   KEY_SPACE,    KEY_CAPS_LOCK,   KEY_F1,           // 0x38 - 0x3B
    KEY_F2,     KEY_F3,       KEY_F4,          KEY_F5,           // 0x3C - 0x3F
    KEY_F6,     KEY_F7,       KEY_F8,          KEY_F9,           // 0x40 - 0x43
    KEY_F10,    KEY_NUM_LOCK, KEY_SCROLL_LOCK, KEY_NP_7,         // 0x44 - 0x47
    KEY_NP_8,   KEY_NP_9,     KEY_NP_MINUS,    KEY_NP_4,         // 0x48 - 0x4B
    KEY_NP_5,   KEY_NP_6,     KEY_NP_PLUS,     KEY_NP_1,         // 0x4C - 0x4F
    KEY_NP_2,   KEY_NP_3,     KEY_NP_0,        KEY_NP_PERIOD,    // 0x50 - 0x53
    KEY_RSVD,   KEY_RSVD,     KEY_RSVD,        KEY_F11,          // 0x54 - 0x57
    KEY_F12,    KEY_RSVD,     KEY_RSVD,        KEY_RSVD,         // 0x58 - 0x5B
    KEY_RSVD,   KEY_RSVD,     KEY_RSVD,        KEY_RSVD,         // 0x5C - 0x5F
    KEY_RSVD,   KEY_RSVD,     KEY_RSVD,        KEY_RSVD,         // 0x60 - 0x63
    KEY_RSVD,   KEY_RSVD,     KEY_RSVD,        KEY_RSVD,         // 0x64 - 0x67
    KEY_RSVD,   KEY_RSVD,     KEY_RSVD,        KEY_RSVD,         // 0x68 - 0x6B
    KEY_RSVD,   KEY_RSVD,     KEY_RSVD,        KEY_RSVD,         // 0x6C - 0x6F
    KEY_RSVD,   KEY_RSVD,     KEY_RSVD,        KEY_RSVD,         // 0x70 - 0x73
    KEY_RSVD,   KEY_RSVD,     KEY_RSVD,        KEY_RSVD,         // 0x74 - 0x77
    KEY_RSVD,   KEY_RSVD,     KEY_RSVD,        KEY_RSVD,         // 0x78 - 0x7B
    KEY_RSVD,   KEY_RSVD,     KEY_RSVD,        KEY_RSVD          // 0x7C - 0x7F
};

//
// Extra lookup table for scancode set 1.
// Scancodes in this lookup table begin with E0 or E0 2A E0
//
static uint8_t _SCANCODE_SET1_E0[] = {
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x00 - 0x03
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x04 - 0x07
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x08 - 0x0B
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x0C - 0x0F
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x10 - 0x13
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x14 - 0x17
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x18 - 0x1B
    KEY_NP_ENTER,   KEY_RCTRL,     KEY_RSVD,   KEY_RSVD,         // 0x1C - 0x1F
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x20 - 0x23
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x24 - 0x27
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x28 - 0x2B
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x2C - 0x2F
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x30 - 0x33
    KEY_RSVD,       KEY_NP_DIVIDE, KEY_RSVD,   KEY_PRINTSCRN,    // 0x34 - 0x37
    KEY_RALT,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x38 - 0x3B
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x3C - 0x3F
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x40 - 0x43
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_HOME,         // 0x44 - 0x47
    KEY_UP,         KEY_PAGE_UP,   KEY_RSVD,   KEY_LEFT,         // 0x48 - 0x4B
    KEY_RSVD,       KEY_RIGHT,     KEY_RSVD,   KEY_END,          // 0x4C - 0x4F
    KEY_DOWN,       KEY_PAGE_DOWN, KEY_INSERT, KEY_DELETE,       // 0x50 - 0x53
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x54 - 0x57
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_LSUPER,       // 0x58 - 0x5B
    KEY_RSUPER,     KEY_MENU,      KEY_RSVD,   KEY_RSVD,         // 0x5C - 0x5F
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x60 - 0x63
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x64 - 0x67
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x68 - 0x6B
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x6C - 0x6F
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x70 - 0x73
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x74 - 0x77
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD,         // 0x78 - 0x7B
    KEY_RSVD,       KEY_RSVD,      KEY_RSVD,   KEY_RSVD          // 0x7C - 0x7F
};

static uint8_t _SCANCODE_SET1_PAUSE[] = {
    0xE1, 0x1D, 0x45, 0xE1, 0x9D, 0xC5
};

int kbd_parse_set1(ScancodePacket *packet, KeyEvent *evt) {
    // set 1 has three possible scancode lengths
    // 1 - <c>                           - make
    //     (0x80 | <c>)                  - break
    // 2 - 0xE0 <c>                      - make
    //     0xE0 (0x80 | <c>)             - break
    // 6 - 0xE1 0x1D 0x45 0xE1 0x9D 0xC5 - pause pressed
    //
    // Some keyboard/controller setups send some extra bytes for the 0xE0
    // scancodes, like E0 2A <make code> and <break code> E0 AA. For those
    // cases we simply discard them.

    uint8_t length = packet->length;

    if (length == 0) {
        // should never happen, why parse an empty packet?
        return KBD_PARSE_ERROR;
    }

    int result;
    uint8_t head = packet->buffer[0];

    //
    // determine if the scancode is invalid or incomplete from
    // the first byte (head) in the buffer.
    // If the head is 0xE0, then the scancode is 2 bytes long
    // If the head is 0xE1, then the scancode is a pause make code and is
    // 6 bytes long
    //
    if (head == 0xE0) {
        if (length == 1) {
            return KBD_PARSE_INCOMPLETE;
        } else if (length > 2) {
            return KBD_PARSE_DISCARD;
        }
    } else if (head == 0xE1) {
        if (length < 6) {
            return KBD_PARSE_INCOMPLETE;
        } else if (length > 6) {
            return KBD_PARSE_DISCARD;
        }
    }

    uint8_t scancode;
    uint8_t *table = NULL;

    switch (packet->length) {
        case 1:
            // <c>
            // (0x80 | <c>)
            scancode = head;
            table = _SCANCODE_SET1;
            break;
        case 2:
            // 0xE0 <c>
            // 0xE0 (0x80 | <c>)
            scancode = packet->buffer[1];
            table = _SCANCODE_SET1_E0;
            break;
        case 6:
            // 0xE1 0x1D 0x45 0xE1 0x9D 0xC5
            // all this for just a single make code
            for (int i = 0; i != 6; ++i) {
                if (packet->buffer[i] != _SCANCODE_SET1_PAUSE[i]) {
                    return KBD_PARSE_DISCARD;
                }
            }
            evt->key = KEY_PAUSE;
            evt->flags = KEY_FLAG_PRESSED;
            result = KBD_PARSE_COMPLETE;
            break;
        default:
            // we "should" never get here
            result = KBD_PARSE_ERROR;
            break;
    }

    //
    // if a table and scancode was set, we lookup the scancode's keycode in the
    // table and if it isn't KEY_RSVD, we have a complete scancode. The keycode
    // and flags are then stored in the event pointer.
    //
    if (table != NULL) {
        // if the 7th bit is set, the key was released
        // if it was cleared, the key was pressed
        bool isBreak = (scancode & 0x80) == 0x80;
        scancode &= 0x7F; // clear bit 7
        
        Key key = table[scancode];
        if (key == KEY_RSVD) {
            // invalid or unknown scancode, stop the parse
            result = KBD_PARSE_DISCARD;
        } else {
            // lookup successful, store the key and flags in the event pointer.
            evt->key = key;
            evt->flags = (isBreak) ? 0 : KEY_FLAG_PRESSED;
            result = KBD_PARSE_COMPLETE;
        }
    }

    return result;
}

// int kbd_parse_set2(ScancodePacket *packet, KeyEvent *evt) {
//     (void)packet; (void)evt;
//     return KBD_PARSE_ERROR;
// }

// int kbd_parse_set3(ScancodePacket *packet, KeyEvent *evt) {
//     (void)packet; (void)evt;
//     return KBD_PARSE_ERROR;
// }