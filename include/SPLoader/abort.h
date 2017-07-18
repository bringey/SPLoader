/*
** File: include/SPLoader/abort.h
**
** Author: Brennan Ringey
**
*/

#ifndef _SPLOADER_ABORT_H
#define _SPLOADER_ABORT_H

#ifndef __ASM__

#define abort(reason) _abort(__FILE__, __LINE__, reason)

void _abort(const char *file, unsigned line, const char *reason);

#endif  // __ASM__

#endif