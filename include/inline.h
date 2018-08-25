
#ifndef _INLINE_H
#define _INLINE_H

#ifndef __ASM__

#define INLINE inline __attribute__((always_inline))

#endif  // __ASM__

#endif