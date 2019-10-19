#include "my_lib.h"

int helper(struct my_stack_node *node, int fd, int counter);

/*La función strcmp() compara las cadenas apuntadas por str1 y str2.
Devuelve un entero, en función de su código ASCII:
< 0 indica que str1 < str2.
> 0 indica que str2 < str1.
= 0 indica que str1 = str2.
*/
int my_strcmp(const char *str1, const char *str2){
    int r=3,i=0;
    size_t len1, len2;
    len1 = my_strlen(str1);
    len2 = my_strlen(str2);
    printf("Longitud de 1: %ld \n",len1);
    printf("Longitud de 2: %ld \n",len2);
    printf("%d \n", str1[2]);
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

// ########### Aventura 2 ########### //

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
//POP
void *my_stack_pop(struct my_stack *stack){
	
    struct my_stack_node *borrar;

    if(stack->first==NULL){ //Verifica si la pila está vacia
		return NULL;            
    }
    else{
        borrar = stack->first;
	//free(borrar);
        stack -> first = stack -> first -> next;
        return borrar -> data;
    }        
}
//LEN
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
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC | S_IRUSR | S_IWUSR);
    if (fd != -1) {
        write(fd, &stack->size, sizeof(int));
        return helper(stack->first, fd, 0);
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
int helper(struct my_stack_node *node, int fd, int counter) {
    if (node == NULL) return counter;
    int c = helper(node->next, fd, counter + 1);
    if (write(fd, node->data, sizeof(node)) == -1) return -1;
    return c;
}

//READ
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
    while (read(op, data, sizeof(struct my_stack_node))>0) {
        my_stack_push(aux, data);
        data = malloc(size);
    }
    close(op);
    return aux;
}

//struct my_stack *my_stack_read(char *filename); Ruben
//int my_stack_write(struct my_stack *stack, char *filename); Luis
//int my_stack_purge(struct my_stack *stack); Andrea
