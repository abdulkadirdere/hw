#include <stdio.h>
#include <stdlib.h>

int foo;            /* a staticly allocated variable */

int recur(int i) {      /* a recursive function */
    int j = i;            /* a stack allocated variable within a recursive function */
    printf("recur call %d: stack@ %lx\n", i, (long) &j); /* %l is for long type */
    if (i > 0) {
        return recur(i-1);
    }
    return 0;
}

int stuff = 7;          /* a statically allocated, pre-initialized variable */

int main (int argc, char *argv[]) {
    int i;            /* a stack allocated variable */
    char *buf1 =(char*) malloc(100); /* dynamically allocate stuff but cast it */
    char *buf2 =(char*) malloc(100); /* and some more stuff with casting */
    printf("_main  @ %lx\n",(long) &main); /*main needs & = pointing to an address*/
    printf("recur @ %lx\n",(long) recur); /*  %l is a type long */
    printf("_main stack: %lx\n",(long) i); /* & = address of stack variable */
    printf("static data: %lx\n",(long) &stuff); /* &=address */
    printf("Heap: malloc 1: %lx\n", (long unsigned int) buf1);
    printf("Heap: malloc 2: %lx\n", (long unsigned int) buf2);
    recur(3);
    return 0;
}
