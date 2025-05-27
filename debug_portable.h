#ifndef DEBUG_PORTABLE_H_INCLUDED
#define DEBUG_PORTABLE_H_INCLUDED


/* #define NO_STDLIB_ABORT */
/* #define NO_STDLIB_SIGNAL */
/* #define NO_STDLIB_PRINT */


#define STRINGIZE( L )     #L
#define STRINGMACRO( M, L ) M(L)
#define FILE_LINE STRINGMACRO( STRINGIZE, __LINE__ )
#define reminder(M) __FILE__ "(" FILE_LINE ") | Reminder: " M


#ifndef NO_STDLIB_ABORT
  #include <stdlib.h>
  #define __abort() abort()
#elif !defined(NO_STDLIB_SIGNAL)
  #include <signal.h> /* Use POSIX signal */
  #define __abort() raise(SIGABRT)
#else
  /* Trigger segfault (hopefully) */
  #define __abort() do { *((volatile int*)(0)) = 1; } while(0)
  /* #define __abort() do { static void (*f)() = 0; f(); } while(0) */
#endif


#ifndef NO_STDLIB_PRINT
  #include <stdio.h>
  #define __assert_puts(M) fputs(M, stderr)
#else
  #define __assert_puts(M) do {} while(0)
#endif


#define _assert_base(TYPE, C, M) do { if(!(C)) { \
  __assert_puts(__FILE__ "(" FILE_LINE ") | "); \
  __assert_puts(TYPE); __assert_puts(": "); __assert_puts(M); \
  __assert_puts("\n\rABORT\n\r"); \
  __abort(); \
} } while(0)

#ifdef DEBUG

  /* Use real debug-print function for ISO C pedantry regarding macro varargs */
  #ifndef NO_STDLIB_PRINT
    #include <stdio.h>
    #include <stdarg.h> /* Needed for varargs */
    static void dbgprintf(const char* const fmt, ...) {
      va_list args; va_start(args, fmt);
      vfprintf(stderr, fmt, args);
      va_end(args);
    }
  #else
    static void dbgprintf(const char* const fmt, ...) { (void)fmt; }
  #endif

  #define dbgassert_message(C, M) _assert_base("Debug assertion failed", C, M)
  #define dbgcheck(C) (C)
  #define dbgtrap(M) _assert_base("Debug trap", 0, M)

#else

  static void dbgprintf(const char* fmt, ...) { (void)fmt; }

  #define dbgassert_message(C, M) do {} while(0)
  #define dbgcheck(C) (0)
  #define dbgtrap(M) do {} while(0)

#endif


#define assert_message(C, M) _assert_base("Assertion failed", C , M)
#define dbgassert(C) dbgassert_message(C, #C)


#endif /* DEBUG_PORTABLE_H_INCLUDED */
