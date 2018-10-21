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


int con_clear(void);

int con_init(void);

int con_putchar(char ch);

int con_puts(const char *str);

int con_printf(const char *fmt, ...);

#define con_width() _con_width()

#define con_height() _con_height()


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
int _con_clear(void);

/*
** @brief Calculates an index in the console buffer for the given coordinates
*/
unsigned _con_index(unsigned x, unsigned y);

/*
** @brief Puts a character at the given row and column 
*/
int _con_put(unsigned index, char ch);

int _con_scroll(unsigned lines);

int _con_updateCursor(unsigned index);

// driver properties

unsigned _con_height(void);

unsigned _con_width(void);


#endif  //__ASM__

#endif