/*
 * Copyright (c) 2008 Travis Geiselbrecht
 *
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 */
#pragma once

#if ENABLE_ASSERTIONS
#define ASSERT(x)                                                              \
    do {                                                                       \
        if ((!(x))) {                                                          \
            panic("ASSERT FAILED at (%s:%d): %s\n", __FILE__, __LINE__, #x);   \
        }                                                                      \
    } while (0)
#else
#define ASSERT(x)                                                              \
    do {                                                                       \
    } while (0)
#endif

#define assert(e) ASSERT(e)
#define static_assert(e) STATIC_ASSERT(e)

/*******************************************************************************
 * Macro to flag a compile time assertion. It uses the preprocessor to generate
 * an invalid C construct if 'cond' evaluates to false.
 * The following compilation error is triggered if the assertion fails:
 * "error: size of array 'msg' is negative"
 * The 'unused' attribute ensures that the unused typedef does not emit a
 * compiler warning.
 ******************************************************************************/
#define CASSERT(cond, msg) typedef char msg[(cond) ? 1 : -1] __unused
