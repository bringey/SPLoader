/*
** File: loader/main/main.c
**
** Author: Brennan Ringey
**
** Contains the main function for the loader
**
*/

#include <SPLoader/i386-pc/mem/map.h>

#include <SPLoader/console/out.h>
#include <SPLoader/timer/timer.h>
#include <SPLoader/kbd/kbd.h>

void testCallback(unsigned ticks) {
    con_printf("Timer callback! ticks: %d\n", ticks);
}

int main(void) {

    con_clear();

    con_setBgColor(CON_COLOR_WHITE);
    con_setFgColor(CON_COLOR_BLACK);
    con_printf(" SPLoader %69s ", "v0.1");
    con_setBgColor(CON_DEFAULT_BG);
    con_setFgColor(CON_DEFAULT_FG);

    memmap_dump(false);

    // timer_init();
    // timer_start(testCallback);

    // for (;;) {
    //     asm("hlt");
    // }

    kbd_init();

    KeyEvent evt;
    for (;;) {
        kbd_waitForEvent(&evt);
        con_printf("[KeyEvent] Key: %02x Flags: %02x\n", evt.key, evt.flags);
    }

    //con_setCursor(40, 12);

    // for (int i = 0; i != 25; ++i) {
    //     con_puts("Test Line: ");
    //     con_putchar('A' + i);
    //     con_putchar('\n');
    // }

    // con_scroll(3);

    // con_puts("test test test");
    // con_puts_at(10, 10, "STR placed at 10,10");

    // con_printf("Printf testing\n");
    // con_printf_at(20, 20, "PRINTF placed at 20,20");

    // con_printf("%-8x", 0xBABE);

    //con_puts("Test test test\n");
    //con_puts("scroll");
    //con_driver_scroll(1);

    return 0;
}