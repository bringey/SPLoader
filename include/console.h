/*
** File: include/SPLoader/console.h
**
** Author: bringey
**
** Contains functions for console output.
*/

#ifndef _SPLOADER_CONSOLE_H
#define _SPLOADER_CONSOLE_H

#ifndef __ASM__


int spl_con_clear(void);

int spl_con_init(void);

int spl_con_putchar(char ch);

int spl_con_puts(const char *str);

int spl_con_printf(const char *fmt, ...);

#define spl_con_width() _spl_con_width()

#define spl_con_height() _spl_con_height()


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
int _spl_con_clear(void);

/*
** @brief Calculates an index in the console buffer for the given coordinates
*/
unsigned _spl_con_index(unsigned x, unsigned y);

/*
** @brief Puts a character at the given row and column 
*/
int _spl_con_put(unsigned index, char ch);

int _spl_con_scroll(unsigned lines);

int _spl_con_updateCursor(unsigned index);

// driver properties

unsigned _spl_con_height(void);

unsigned _spl_con_width(void);


#endif  //__ASM__

#endif