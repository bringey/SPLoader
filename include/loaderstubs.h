/*
** File: include/loaderstubs.h
**
** Author: bringey
**
** Header file for the loaderstubs library. Contains stubs and other utilities
** for running loader code on a hosted environment for testing.
*/

#ifndef _LOADERSTUBS_H
#define _LOADERSTUBS_H

#define UNIT_TESTING

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <setjmp.h>

#include <cmocka.h>

#include <loader/abort.h>

typedef enum {
    EKIND_NONE      = 0,        // No exception
    EKIND_ABORTED   = 1,        // loader_abort() was called
    EKIND_EXCEPT    = 2,        // exceptv(ex, code) was called
} ExceptionKind;

typedef struct {
    ExceptionKind state;
    unsigned ex;
    int code;
} Exception;

extern bool __expectingException;
extern jmp_buf __env;
extern Exception __lastEx, __expectedEx;

//
// function used only by the expect_exception macro and should not be called
// directly. Tests the __expectedEx and __lastEx globals for equality. If they
// are unequal, the function will fail the current test.
//
void expect_exception_end(const char *fnCall);

//
// do a function call expecting an exception to occur, this works similar to
// how cmocka's expect_assert_failure works
//
#define expect_exception(fnCall) \
    do { \
        const int result = setjmp(__env); \
        if (result) { \
            __expectingException = false; \
        } else { \
            __expectingException = true; \
            fnCall ; \
            __lastEx.state = EKIND_NONE; \
        } \
        expect_exception_end(#fnCall); \
    } while (0)

#define expect_abort() \
    __expectedEx = (Exception){ .state = EKIND_ABORTED}

#define expect_exceptv(_ex, _code) \
    __expectedEx = (Exception){.state = EKIND_EXCEPT, .ex = _ex, .code = _code}

#define expect_except(_ex) expect_exceptv(_ex, 0)

#define expect_assert() expect_except(EX_ASSERT)

#endif