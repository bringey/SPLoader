/*
** File: include/loader/console.h
**
** Author: bringey
**
** Contains functions for console output.
*/

#ifndef _LOADER_CONSOLE_H
#define _LOADER_CONSOLE_H

#ifndef __ASM__


int ldr_con_clear(void);

int ldr_con_init(void);

int ldr_con_putchar(char ch);

int ldr_con_puts(const char *str);

int ldr_con_printf(const char *fmt, ...);

#define ldr_con_width() _ldr_con_width()

#define ldr_con_height() _ldr_con_height()


//
// Platform-dependent functions
//

/*
** @brief clear function for the console driver
**
** Clears a region of the on-screen console buffer. The lines from
** rowStart to rowEnd will be cleared. rowEnd must be greater than
** rowStart.
*/
int _ldr_con_clear(void);

/*
** @brief Calculates an index in the console buffer for the given coordinates
*/
unsigned _ldr_con_index(unsigned x, unsigned y);

/*
** @brief Puts a character at the given row and column 
*/
int _ldr_con_put(unsigned index, char ch);

int _ldr_con_scroll(unsigned lines);

int _ldr_con_updateCursor(unsigned index);

// driver properties

unsigned _ldr_con_height(void);

unsigned _ldr_con_width(void);


#endif  //__ASM__

#endif