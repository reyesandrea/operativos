#include <stdio.h>
#include "my_lib.h"

int main(int argc, char *argv[]) {
   if (argv[1] == NULL) {
       printf("USAGE: ./reader filename\n");
       return 0;
   }
   struct my_stack *pila = my_stack_read(argv[1]);
   printf("Initial stack length: %d\n", (pila != NULL) ? pila->size : 0);
   if (pila == NULL) {
       pila = my_stack_init(10);
       my_stack_write(pila, argv[1]);
   }
   for (int i = pila->size; i < 10; i++) {
       int * data = (int *) malloc(sizeof(int));
       my_stack_push(pila, data);
   }
   printf("Final stack length: %d\n", pila->size);
}
