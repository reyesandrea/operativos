#include "my_lib.h"
#include <string.h>

/*La función strcmp() compara las cadenas apuntadas por str1 y str2.
Devuelve un entero, en función de su código ASCII:
< 0 indica que str1 < str2.
> 0 indica que str2 < str1.
= 0 indica que str1 = str2.
*/
int my_strcmp(const char *str1, const char *str2){
    int num1 = *(str1);
    int num2 = *(str2);
      
  return num1 -num2;
}
/*La función calcula el nº de bytes de la cadena apuntada por str, 
sin incluir el carácter nulo de terminación ‘\0’.
Devuelve la longitud de str. No devuelve error.*/
size_t my_strlen(const char *str){
    int cont = 0;
    while (str[cont]!='\0'){
        cont++;
    }
    return cont;
}

/*
La función copia la cadena apuntada por src (con el carácter de terminación ‘\0’)
en la memoria apuntada por dest.
Devuelve el puntero dest. No devuelve error.
*/
char *my_strcpy(char *dest, const char *src){
    int i = 0;
    for(i=0; i<=my_strlen(src)+1;i++){ //Cambiar esto por my_strlen
        dest[i]= src[i];
    }
    return dest;
}

/*La función strncpy() copia n caracteres de la cadena apuntada 
por src (con el carácter de terminación ‘\0’) en la memoria apuntada por dest.*/
char *my_strncpy(char *dest, const char *src, size_t n){
    int i = 0;
    for(i=0; i<=my_strlen(src)+1 && i<n;i++){ 
        dest[i]= src[i];
    }
    while(i<= n){
        dest[i]=0;
        i++;
    }
    
    return dest;
}

/*La función strcat() añade la cadena apuntada por src a la cadena apuntada por dest*/
char *my_strcat(char *dest, const char *src){
    int i = 0, j=0;
    /*Bucle en el que recorremos dest buscando el primer caracter nulo, al encontrarlo,
    lo sustituimos por el caracter no nulo correspondiente de src*/
    for(j=0; j<my_strlen(dest)+1;j++){
        if(dest[j]=='\0' && src[i]!='\0'){
            dest[j]=src[i];
            i++;
        }
    }    
    return dest;
}