#include <stdio.h>
#include <limits.h>
#include "my_lib.h"

int main(int argc, char *argv[]) {
    if (argv[1] == NULL) {
       printf("USAGE: ./reader filename\n");
       return 0;
   }
   struct my_stack *pila = my_stack_read(argv[1]);
   int length = my_stack_len(pila);
   pila = my_stack_read(argv[1]);
   if (pila == NULL) {
       printf("Couldn't open stack file %s \n",argv[1]);
       return 0;
   }
   printf("Stack length: %d\n", length);
   int suma = 0, min = INT_MAX, max = 0;
   for (int i = 0; i < length; i++) {
       int *pop = my_stack_pop(pila);
       printf("%d \n", *pop);
       suma += *pop;
       if (*pop < min) min = *pop;
       if (*pop > max) max = *pop;
   }
   printf("Items\t %d\nSum\t %d\nMin\t %d\nMax\t %d\nAverage\t %d \n", length, suma, min, max, suma / pila->size);
}
