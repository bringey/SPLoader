
#include <SPLoader/kbd/parse.h>


int kbd_parse_set1(ScancodePacket *packet, KeyEvent *evt) {
    (void)packet; (void)evt;
    return KBD_PARSE_ERROR;
}

// int kbd_parse_set2(ScancodePacket *packet, KeyEvent *evt) {
//     (void)packet; (void)evt;
//     return KBD_PARSE_ERROR;
// }

// int kbd_parse_set3(ScancodePacket *packet, KeyEvent *evt) {
//     (void)packet; (void)evt;
//     return KBD_PARSE_ERROR;
// }