#include "debug_portable.h"
#include "exceptions_portable.h"

#include <stdio.h>

int main(int argc, char** argv) {

  __try {

    printf("Trying\n\r");

    __try {
      __throw(3);
      printf("try-in-try\n\r");
    } __except(2) {
      printf("try-in-try exception 2\n\r");
    } __except(3) {
      printf("try-in-try exception 3\n\r");
    } __finally {
      printf("try-in-try finally\n\r");
    }

  } __except(1) {

    printf("Caught exception 1\n\r");

  } __except(2) {

    printf("Caught exception 2\n\r");

  } __finally {
    printf("try finally\n\r");
  }


  return 0;
}
