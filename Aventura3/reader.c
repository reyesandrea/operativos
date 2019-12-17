/*
    ### AVENTURA 3 ###
    Squad: LRAOS
    Miembros: 
        Camino, Lluís 
        López, Rubén
        Reyes, Andrea
*/

#include <stdio.h>
#include <limits.h>
#include "my_lib.h"

int main(int argc, char *argv[]) {
    if (argv[1] == NULL) {
       printf("USAGE: ./reader filename\n");
       return 0;
   }
   struct my_stack *pila = my_stack_read(argv[1]);
   
   //Comprobar si la pila existe
   if (!pila) {
       printf("Couldn't open stack file %s \n",argv[1]);
       exit(EXIT_FAILURE);
   } 
   int length = my_stack_len(pila);
   pila = my_stack_read(argv[1]);
   

   //Comprobar si la pila contiene menos de 10 elementos 
   if (length<10){
       printf("Stack with less than 10 elements\n");
   }
    
   //Si la pila tine mas de 10 elementos avisar
   if(length>10){
       printf("La pila tiene más de 10 elementos, se procederá a leer los 10 primeros\n");
   }

   printf("Stack length: %d\n", length);
   int suma = 0, min = INT_MAX, max = 0;
   for (int i = 0; i < 10; i++) {
       int *pop = my_stack_pop(pila);
       printf("%d \n", *pop);
       suma += *pop;
       if (*pop < min) min = *pop;
       if (*pop > max) max = *pop;
   }
   printf("Items\t %d\nSum\t %d\nMin\t %d\nMax\t %d\nAverage\t %d \n", length, suma, min, max, suma / pila->size);
}
