

#include <SPLoader/main/menu.h>
#include <SPLoader/console/out.h>
#include <SPLoader/err.h>

#ifndef SPLOADER_VERSION
// placeholder version macro
#define SPLOADER_VERSION "v0.1"
#endif


// colors
static unsigned BLACK_ON_WHITE;
static unsigned WHITE_ON_BLACK;

static const char TITLE[] = "SPLoader";
static const char VERSION[] = SPLOADER_VERSION;


int menu_init(void) {
    // calculate colors
    BLACK_ON_WHITE = con_color(CON_COLOR_BLACK, CON_COLOR_WHITE);
    WHITE_ON_BLACK = con_color(CON_COLOR_WHITE, CON_COLOR_BLACK);
    return E_SUCCESS;
}



int menu_main(void) {

    unsigned width = con_width();
    unsigned height = con_height();

    // set the scroll menu
    con_setWindow(1, height - 1);	

    // print the title bar
    con_setColor(BLACK_ON_WHITE);
    con_printf_at(0, 0, " %s", TITLE);
    int versionLen = width - sizeof(TITLE) - 2; // remaining space on line
    if (versionLen >= 0) {
        //con_printf_at(sizeof(TITLE) + 1, 0, "%*s", versionLen, VERSION);
        con_printf_at(sizeof(TITLE), 0, " %s", VERSION);
    }

    return E_SUCCESS;
}