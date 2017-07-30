

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

static void __menu_display(void);

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
    int versionLen = width - sizeof(TITLE) - 1; // remaining space on line
    if (versionLen >= 0) {
        con_printf_at(sizeof(TITLE), 0, "%*s ", versionLen, VERSION);
    }

    __menu_display();

    return E_SUCCESS;
}

void __menu_display(void) {

    con_setColor(WHITE_ON_BLACK);
    con_printf_at(0, 2, " %-*s ", con_width() - 2, "Select an option:");

    unsigned boxWidth = con_width() - 2;
    unsigned boxHeight = 10;
    con_putchar_at(1, 4, '\xDA');                    // upper-left corner
    for (unsigned i = 0; i != boxWidth - 2; ++i) {
        con_putchar_at(i + 2, 4, '\xC4');            // top border
    }
    con_putchar_at(boxWidth, 4, '\xBF');             // upper-right corner
    for (unsigned i = 0; i != boxHeight; ++i) {
        con_putchar_at(boxWidth, 5 + i, '\xB3');     // right border
    }
    con_putchar_at(boxWidth, 5 + boxHeight, '\xD9'); // bottom-right corner
    for (unsigned i = 0; i != boxWidth - 2; ++i) {
        con_putchar_at(i + 2, boxHeight + 5, '\xC4'); // bottom border
    }
    con_putchar_at(1, boxHeight + 5, '\xC0');        // bottom-left corner
    for (unsigned i = 0; i != boxHeight; ++i) {
        con_putchar_at(1, i + 5, '\xB3');            // left border
    }

}