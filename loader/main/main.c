/*
** File: loader/main/main.c
**
** Author: Brennan Ringey
**
** Contains the main function for the loader
**
*/

#include <SPLoader/main/menu.h>

#include <SPLoader/console/out.h>
#include <SPLoader/timer/timer.h>
#include <SPLoader/kbd/kbd.h>

#include <SPLoader/err.h>
#include <SPLoader/abort.h>


int main(void) {

    con_clear();

    int error;

    if ((error = timer_init()) != E_SUCCESS) {
        abort("failed to initialize timer");
    }

    if ((error = kbd_init()) != E_SUCCESS) {
        abort("failed to initialized keyboard");
    }

    menu_init();

    menu_main();

    // KeyEvent evt;
    // for (;;) {
    //     kbd_waitForEvent(&evt);
    //     con_printf("[KeyEvent] Key: %02x Flags: %02x\n", evt.key, evt.flags);
    // }

    return 0;
}