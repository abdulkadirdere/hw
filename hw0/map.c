#include <stdio.h>
#include <stdlib.h>

int foo;            /* a staticly allocated variable */

int
recur( int i ) {      /* a recursive function */
  int j = i;            /* a stack allocated variable within a recursive function */
  /* fix this so it prints the address of j */
  printf("recur call %d: stack@ %lx\n", i, (long unsigned int) &j);
    
  if (i > 0) {
    return recur(i-1);
  }
  return 0;
}

int stuff = 7;          // a statically allocarted, pre-initialized variable

int main ( ) {
  int i;            // a stack allocated variable
  char *buf1 = malloc(100); // dynamically allocate some stuff
  char *buf2 = malloc(100); // ... and some more stuff
  
  printf("_main  @ %lx\n", (long unsigned int) &main); 
  printf("recur @ %lx\n", (long unsigned int) &recur); 
  printf("_main stack: %lx\n", (long unsigned int) &i); 
  printf("static data: %lx\n", (long unsigned int) &stuff); 
  printf("Heap: malloc 1: %lx\n", (long unsigned int) buf1);
  printf("Heap: malloc 2: %lx\n", (long unsigned int) buf2);
  
  recur(3);
  return 0;
}
