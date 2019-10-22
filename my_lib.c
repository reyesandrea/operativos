/*
    ### AVENTURA 1 ###
    Squad: LRAOS
    Miembros: 
        Camino, Lluís 
        López, Rubén
        Reyes, Andrea
*/

#include "my_lib.h"

int helper(struct my_stack_node *node, int size, int fd, int counter);

/* 
 * Compara dos cadenas de caracteres.
 * @param str1 y str2
 * @return:
           -1 indica que str1 < str2.
            1 indica que str2 < str1.
            0 indica que str1 = str2.
 */
int my_strcmp(const char *str1, const char *str2){
    int r=3,i=0;
    size_t len1, len2;
    len1 = my_strlen(str1);
    len2 = my_strlen(str2);
    while (i!= (len1-1)|| i!=(len2-1))
    {
        if(str1[i]==str2[i]){
            r=0;
        }
        else if(str1[i]<str2[i]){
            return r=-1;
        }else{
            return r=1;
        }
        i++;
    }
    return r;
}

/* 
 * Calcula el nº de bytes de una cadena.
 * @param str
 * @return: longitud
 */
size_t my_strlen(const char *str){
    int cont = 0;
    while (str[cont]!='\0'){
        cont++;
    }
    return cont;
}

/*
 * Copia una cadena.
 * @param dest, src
 * @return: dest
 */
char *my_strcpy(char *dest, const char *src){
    int i = 0;
    for(i=0; i<=my_strlen(src)+1;i++){ //Cambiar esto por my_strlen
        dest[i]= src[i];
    }
    return dest;
}

/*
 * Añade una parte de una cadena de caracteres a otra cadena
 * @param dest src
 * @return: dest
 */
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

/*
 * Añade una cadena de caracteres a otra
 * @param dest src
 * @return: dest
 */
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

/* ##### GESTOR DE PILAS ##### */

/**
 * Inicializa y devuelve una pila de tamaño `size`.
 * @param size Tamaño de los datos de la pila
 * @return
 */
struct my_stack *my_stack_init(int size) {
    struct my_stack *stack;
    stack = malloc(sizeof(int) + sizeof(struct my_stack*));
    stack->size = size;
    stack->first = NULL;
    return stack;
}

/**
 * Añade un nodo a la pila.
 * @param stack
 * @param data
 * @return 0 si se ha añadido ó -1 si ha ocurrido un error
 */
int my_stack_push(struct my_stack *stack, void *data) {
    if (!stack) return -1;  // Verifica si la pila no ha sido instanciada.
    struct my_stack_node *nodo;
    nodo = malloc(sizeof(data));
    nodo->data = data;
    nodo->next = stack->first;
    stack->first = nodo;
    return 0;
}

/**
 * Elimina el nodo superior de la pila y libera la memoria que ocupaba ese nodo.
 * @param stack
 * @return data (puntero a los datos del elemento eliminado) 
 */
void *my_stack_pop(struct my_stack *stack){

    struct my_stack_node *borrar;

    if(stack->first==NULL){ //Verifica si la pila está vacia
        return NULL;
    }
    else{
        borrar = stack->first;
        void * data = borrar->data;
        stack -> first = stack -> first -> next;
        free(borrar);
        return data;
    }
}

/**
 * Cuenta la cantidad total de nodos que hay en la pila
 * @param stack
 * @return len (número de nodos totales que hay en la pila)
 */
int my_stack_len (struct my_stack *stack){

    int len = 0;
    struct my_stack_node *punt = stack->first;

    //Si la pila no está vacía, la recorremos incrementando el contador
    if (stack->first != NULL){
        while (punt->next!=NULL){
            len++;
            punt = punt -> next;
        }
        len++;
    }
    return len;
}

/**
 * Escribe los nodos de la pila en un fichero.
 * @param stack
 * @param filename
 * @return número de nodos escrito o -1 si se produce un error;
 */
int my_stack_write(struct my_stack *stack, char *filename) {
    if (stack == NULL) return -1;
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd != -1) {
        write(fd, &stack->size, sizeof(int));
        int r = helper(stack->first, stack->size, fd, 0);
        close(fd);
        return r;
    }
    return -1;
}

/**
 * Función recursiva usada por my_stack_write para recorrer y escribir los nodos de una pila
 * en un fichero.
 * @param node
 * @param fd
 * @param counter
 * @return Número de nodos escritos o -1 si se produce un error;
 */
int helper(struct my_stack_node *node, int size, int fd, int counter) {
    if (node == NULL) return counter;
    int c = helper(node->next, size, fd, counter + 1);
    if (write(fd, node->data, size) == -1) return -1;
    return c;
}

/**
 * Función que lee los datos de la pila almacenados en el fichero 
 * @param filename
 * @return NULL si hubo error
 */
struct my_stack *my_stack_read(char *filename){

    void *data ;
    int op = 0;
    int rd = 0;
    int size;
    struct my_stack *aux=NULL;

    //Primero abrimos el fichero
    op = open(filename, O_RDONLY);
    if(op == -1){
        return NULL;
    }

    //Iniciamos el porceso de lectura si no está vacío
    rd = read(op, &size, sizeof(int));
    if(rd == -1){
        return NULL;
    }
    aux = my_stack_init(size);
    data= malloc(size);
    while (read(op, data, size)>0) {
        my_stack_push(aux, data);
        data = malloc(size);
    }
    close(op);
    return aux;
}

/**
 * Libera la memoria reservada.
 * @param stack
 * @return nº de bytes liberados
 */
int my_stack_purge(struct my_stack *stack){
    int cont = 0, nbyteslib, i;
    int lon = my_stack_len(stack);
    for(i=0;i<lon && stack->first!=NULL;i++){
        void *j = my_stack_pop(stack);
        free(j);
        cont++;
    }
    int result = nbyteslib = cont*(sizeof(struct my_stack_node) + stack->size) + sizeof(struct my_stack);
    free(stack);
    return result;
}
