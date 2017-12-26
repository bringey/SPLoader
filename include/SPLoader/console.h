/*
** File: include/SPLoader/console.h
**
** Author: Brennan Ringey
**
** Contains functions for console output. All functions are to be part of
** the libcore library.
*/

#ifndef _SPLOADER_CONSOLE_H
#define _SPLOADER_CONSOLE_H

#ifndef __ASM__

//
// ConColor enum. Based off of the default color palette for VGA 
// text-compatible mode.
//
typedef enum ConColor_e {
    CON_COLOR_BLACK          = 0x0,
    CON_COLOR_BLUE           = 0x1,
    CON_COLOR_GREEN          = 0x2,
    CON_COLOR_CYAN           = 0x3,
    CON_COLOR_RED            = 0x4,
    CON_COLOR_MAGENTA        = 0x5,
    CON_COLOR_BROWN          = 0x6,
    CON_COLOR_GRAY           = 0x7,
    CON_COLOR_DARK_GRAY      = 0x8,
    CON_COLOR_BRIGHT_BLUE    = 0x9,
    CON_COLOR_BRIGHT_GREEN   = 0xA,
    CON_COLOR_BRIGHT_CYAN    = 0xB,
    CON_COLOR_BRIGHT_RED     = 0xC,
    CON_COLOR_BRIGHT_MAGENTA = 0xD,
    CON_COLOR_YELLOW         = 0xE,
    CON_COLOR_WHITE          = 0xF,
} ConColor;

#define CON_DEFAULT_FG CON_COLOR_WHITE
#define CON_DEFAULT_BG CON_COLOR_BLACK

int spl_con_clear(void);

int spl_con_clearWindow(void);

#define spl_con_color(fg, bg) _spl_con_color(fg, bg)

int spl_con_init(void);

int spl_con_putchar(char ch);

int spl_con_putchar_at(unsigned x, unsigned y, char ch);

int spl_con_puts(const char *str);

int spl_con_puts_at(unsigned x, unsigned y, const char *str);

int spl_con_printf(const char *fmt, ...);

int spl_con_printf_at(unsigned x, unsigned y, const char *fmt, ...);

int spl_con_scroll(unsigned lines);

int spl_con_setWindow(unsigned minY, unsigned maxY);

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
int _spl_con_clearRegion(unsigned rowStart, unsigned rowEnd);

/*
** @brief calculates a color value for the given ConColor
**
*/
unsigned _spl_con_color(ConColor fg, ConColor bg);

/*
** @brief Calculates an index in the console buffer for the given coordinates
*/
unsigned _spl_con_index(unsigned x, unsigned y);

/*
** @brief Puts a character at the given row and column 
*/
int _spl_con_put(unsigned index, unsigned color, char ch);

#define spl_con_resetColor() spl_con_setColor(CON_DEFAULT_FG, CON_DEFAULT_BG)

int _spl_con_scroll(unsigned lineStart, unsigned lineEnd, unsigned lines);

int spl_con_setColor(unsigned color);

int spl_con_setCursor(unsigned x, unsigned y);

int _spl_con_updateCursor(unsigned index);

// driver properties

unsigned _spl_con_height(void);

unsigned _spl_con_width(void);


#endif  //__ASM__

#endif