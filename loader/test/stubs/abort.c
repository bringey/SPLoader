/*
** File: loader/test/stubs/abort.c
**
** Author: bringey
**
** Implementations for _loader_abort, exceptv - longjmp mechanism for testing
** exceptions.
*/

#include <loaderstubs.h>

#include <stdio.h>
#include <string.h>

bool __expectingException = false;
jmp_buf __env;
Exception __lastEx = (Exception){ .state = EKIND_NONE };
Exception __expectedEx = (Exception){ .state = EKIND_NONE };

// re-implement these functions to do a mock assert
// this way we can check if exceptions are being raised when they should

void ldr_exceptv(unsigned ex, int code) {
    __lastEx.state = EKIND_EXCEPT;
    __lastEx.ex = ex;
    __lastEx.code = code;
    if (__expectingException) {
        longjmp(__env, 1);
    } else {
        fail_msg("unexpected exceptv(%d, %d) called", ex, code);
    }
}

void _ldr_abort(void) {
    __lastEx.state = EKIND_ABORTED;
    if (__expectingException) {
        longjmp(__env, 1);
    } else {
        fail_msg("unexpected abort() called");
    }
}

//
// utility function, convert an Exception struct to a string stored in the
// given buffer.
//
// examples:
// { .state = EKIND_NONE }                          -> "no exception"
// { .state = EKIND_ABORT }                         -> "abort"
// { .state = EKIND_EXCEPT, .ex = 2 }               -> "exception 2"
// { .state = EKIND_EXCEPT, .ex = 2, .code = 4 }    -> "exception 2:4"
//
static void exceptionToStr(Exception ex, char *buf, size_t bufsize) {
    switch (ex.state) {
        case EKIND_NONE:
            strncpy(buf, "no exception", bufsize);
            break;
        case EKIND_ABORTED:
            strncpy(buf, "abort", bufsize);
            break;
        case EKIND_EXCEPT:
            if (ex.code == 0) {
                snprintf(buf, bufsize, "exception %d", ex.ex);
            } else {
                snprintf(buf, bufsize, "exception %d:%d", ex.ex, ex.code);
            }
            break;
    }
    // terminate the buffer for sanity
    buf[bufsize - 1] = '\0';
}

void expect_exception_end(const char *fnCall) {
    // compare __expectedEx with __lastEx, if they are "equal" then we got what
    // we expected, otherwise the test fails
    //
    // Possible failures:
    // 1. Differring exception types (ie abort and except)
    // 2. expected except(ex) but ex does not match
    // 3. expected exceptv(ex, code) but ex or code does not match
    if (__expectedEx.state != __lastEx.state ||
        (__expectedEx.state == EKIND_EXCEPT && (__expectedEx.ex != __lastEx.ex || __expectedEx.code != __lastEx.code)))
    {
        char expectedBuf[80];
        char gotBuf[80];
        exceptionToStr(__expectedEx, expectedBuf, sizeof(expectedBuf) / sizeof(char));
        exceptionToStr(__lastEx, gotBuf, sizeof(gotBuf) / sizeof(char));
        fail_msg("expected %s in %s, got %s", expectedBuf, fnCall, gotBuf);
    }
}
