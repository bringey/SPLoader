
#ifdef FE_MENU

#include <SPLoader/main/menu.h>
#include <SPLoader/console.h>
#include <SPLoader/kbd/kbd.h>
#include <SPLoader/err.h>

#include <stdbool.h>

#ifndef SPLOADER_VERSION
// placeholder version macro
#define SPLOADER_VERSION "v0.1"
#endif


// colors
static unsigned BLACK_ON_WHITE;
static unsigned WHITE_ON_BLACK;

static const char TITLE[] = "SPLoader";
//static const char VERSION[] = SPLOADER_VERSION;

typedef struct MenuItem_s {
    char name[72];
    char *temp;
} MenuItem;

typedef struct MenuContainer_s {
    unsigned selected;
    unsigned itemCount;
    MenuItem *items;
} MenuContainer;


static void __menu_display(MenuContainer *cont);


int spl_menu_init(void) {
    // calculate colors
    BLACK_ON_WHITE = spl_con_color(CON_COLOR_BLACK, CON_COLOR_WHITE);
    WHITE_ON_BLACK = spl_con_color(CON_COLOR_WHITE, CON_COLOR_BLACK);
    return E_SUCCESS;
}



int spl_menu_main(void) {

    unsigned width = spl_con_width();
    unsigned height = spl_con_height();

    // set the scroll menu
    spl_con_setWindow(1, height - 1);

    // print the title bar
    spl_con_setColor(BLACK_ON_WHITE);
    spl_con_printf_at(0, 0, " %s", TITLE);
    int versionLen = width - sizeof(TITLE) - 1; // remaining space on line
    if (versionLen >= 0) {
        spl_con_printf_at(sizeof(TITLE), 0, "%*s ", versionLen, VERSION);
    }

    MenuContainer container;
    container.selected = 0;
    container.items = (MenuItem*)0x5000;

    // for testing purposes
    container.items[0].temp = "Menu Item 1";
    container.items[1].temp = "Menu Item 2";
    container.items[2].temp = "Menu Item 3";
    container.itemCount = 3;
    

    KeyEvent evt;
    bool updateDisplay = true;

    for (;;) {
        if (updateDisplay) {
            __menu_display(&container);
        }

        spl_kbd_waitForEvent(&evt);
        if ((evt.flags & KEY_FLAG_PRESSED) == KEY_FLAG_PRESSED) {
            switch (evt.key) {
                case KEY_UP:
                    if (container.selected != 0) {
                        --container.selected;
                        updateDisplay = true;
                    }
                    break;
                case KEY_DOWN:
                    if (container.selected != container.itemCount - 1) {
                        ++container.selected;
                        updateDisplay = true;
                    }
                    break;
                default:
                    updateDisplay = false;
                    break;
            }
        }
    }

    //__menu_display();

    return E_SUCCESS;
}


void __menu_display(MenuContainer *container) {

    unsigned width = spl_con_width();
    for (unsigned i = 0; i < container->itemCount; ++i) {
        if (i == container->selected) {
            spl_con_setColor(BLACK_ON_WHITE);
            spl_con_setCursor(4, i + 4);
        } else {
            spl_con_setColor(WHITE_ON_BLACK);
        }
        spl_con_printf_at(0, i + 4, "    %-*s    ", width - 8, container->items[i].temp);
    }
    
}


// void __menu_display(void) {

//     spl_con_setColor(WHITE_ON_BLACK);
//     spl_con_printf_at(0, 2, " %-*s ", spl_con_width() - 2, "Select an option:");

//     unsigned boxWidth = spl_con_width() - 2;
//     unsigned boxHeight = 10;
//     spl_con_putchar_at(1, 4, '\xDA');                    // upper-left corner
//     for (unsigned i = 0; i != boxWidth - 2; ++i) {
//         spl_con_putchar_at(i + 2, 4, '\xC4');            // top border
//     }
//     spl_con_putchar_at(boxWidth, 4, '\xBF');             // upper-right corner
//     for (unsigned i = 0; i != boxHeight; ++i) {
//         spl_con_putchar_at(boxWidth, 5 + i, '\xB3');     // right border
//     }
//     spl_con_putchar_at(boxWidth, 5 + boxHeight, '\xD9'); // bottom-right corner
//     for (unsigned i = 0; i != boxWidth - 2; ++i) {
//         spl_con_putchar_at(i + 2, boxHeight + 5, '\xC4'); // bottom border
//     }
//     spl_con_putchar_at(1, boxHeight + 5, '\xC0');        // bottom-left corner
//     for (unsigned i = 0; i != boxHeight; ++i) {
//         spl_con_putchar_at(1, i + 5, '\xB3');            // left border
//     }

// }

#else
typedef int dummy;
#endif // FE_MENU

