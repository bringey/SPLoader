/*
** File: loader/main/main.c
**
** Author: Brennan Ringey
**
** Contains the main function for the loader
**
*/

#ifdef FE_MENU
    #include <SPLoader/main/menu.h>
    #include <SPLoader/kbd/kbd.h>
#endif

#if defined(FE_MENU) || defined(FE_CONSOLE)
    #include <SPLoader/console/out.h>
#endif

#if defined(FE_MENU) || defined(FE_TIMER)
    #include <SPLoader/timer/timer.h>
#endif

#include <SPLoader/err.h>
#include <SPLoader/abort.h>


int main(void) {

    #if defined (FE_MENU) || defined(FE_CONSOLE)
        con_init();
        con_clear();
    #endif


    int error;

    #if defined(FE_MENU) || defined(FE_TIMER)

        if ((error = timer_init()) != E_SUCCESS) {
            abort("failed to initialize timer");
        }

    #endif

    #ifdef FE_MENU

        if ((error = kbd_init()) != E_SUCCESS) {
            abort("failed to initialized keyboard");
        }

        menu_init();

        menu_main();

    #endif

    (void)error;

    return 0;
}