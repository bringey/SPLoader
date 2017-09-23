/*
** File: loader/main/main.c
**
** Author: Brennan Ringey
**
** Contains the main function for the loader
**
*/

#include <SPLoader/console.h>
#include <SPLoader/err.h>
#include <SPLoader/abort.h>
#include <SPLoader/version.h>


int main(void) {

    con_init();
    con_clear();

    con_printf("SPLoader v%s\n", VERSION_STR);

    return 0;
}