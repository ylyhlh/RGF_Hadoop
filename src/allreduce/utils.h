#ifndef UTILS_H
#define UTILS_H

#include <assert.h>
#include <sys/time.h>

/**
 * Use assert() when the test is only intended for debugging.
 * Use verify() when the test is crucial for both debug and release binary.
 */
#ifdef NDEBUG
#define verify(expr, ...) do { if (!(expr)) { printf(__VA_ARGS__); abort(); } } while (0)
#else
#define verify(expr, ...) assert(expr)
#endif

#endif // UTILS_H
