#ifndef SCOPE_PORTABLE_H_INCLUDED
#define SCOPE_PORTABLE_H_INCLUDED

/* We can preempt a scope with a constructing statement using loops. */
#define __scope_ctor_block(B) for(register int __c = 0; __c < 2; __c++) if(!__c) B else

/* We can defer a destructing statement to the end of a scope by using loops in a tricky way. */
#define __scope_dtor_block(B) for(register int __d = 0; __d < 2; __d++) if(__d) B else

/* These can be used to concatenate two blocks of legal C code in a scope. */
#define __block_cat(A, B) __scope_ctor_block(A) B

#include <setjmp.h>
#include "debug_portable.h"

#define __try for(struct {int __e; jmp_buf __b; unsigned char __u;} \
  __t = {.__e = 0, .__u = 0}; __t.__e != -1;) __scope_ctor_block({ \
    assert_message(__t.__u < 2, "Unhandled exception!"); __t.__u += __t.__u; \
  }) __scope_dtor_block({ \
    if(__t.__e == -2) /* handle finally case */ { \
      if(!__t.__u) __t.__u = 1; /* Finally has not been handled */ \
      else __t.__e = -1; /* No finally case, break out */ \
    } \
  }) if(!(__t.__e)) __scope_ctor_block({ \
    if(setjmp(__t.__b)) continue; \
  }) __scope_dtor_block({__t.__u = 0; __t.__e = -2;}) \

#define __throw(E) do {__t.__u = 1; __t.__e = E; longjmp(__t.__b, 1);} while(0)

#define __except(E) else if(__t.__e == E) __scope_dtor_block({__t.__u = 0; __t.__e = -2;})

#define __finally else if(__t.__e == -2) __scope_dtor_block({__t.__e = -1;})

#endif /* SCOPE_PORTABLE_H_INCLUDED */
