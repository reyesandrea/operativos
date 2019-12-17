#include <stdio.h>
#include <pthread.h>
#include "my_lib.h"

#define STACK_LENGTH 10
#define NUM_THREADS 10
#define N 1000000

void *func();

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct my_stack *pila;

int main(int argc, char *argv[]) {
    if (argv[1] == NULL) { // Mensaje de error en caso de formato incorrecto
        printf("USAGE: ./av3 filename\n");
        return 0;
    }
    pila = my_stack_read(argv[1]);
    printf("Threads: %d, Iterations: %d \n", NUM_THREADS, N);
    pthread_t ids[NUM_THREADS];
    printf("Initial stack length: %d\n", (pila != NULL) ? my_stack_len(pila) : 0);
    if (pila == NULL) { // Crear pila si el archivo no la contiene
        pila = my_stack_init(STACK_LENGTH);
    }
    for (int i = my_stack_len(pila); i < STACK_LENGTH; i++) { // Llenar pila si tiene menos de 10 elementos
        int *data = malloc(sizeof(int));
        *data = 0;
        my_stack_push(pila, data);
    }
    printf("Final stack length: %d\n", my_stack_len(pila));
    for (int i = 0; i < NUM_THREADS; i++) { // Crear NUM_THREADS hilos
        pthread_create(&ids[i], NULL, func, NULL);
        printf("%d) Thread %d created\n", i, (int) ids[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) { // Esperar a que acaben todos los hilos
        pthread_join(ids[i], NULL); 
    }
    printf("Written elements from stack to file: %d\n", my_stack_write(pila, argv[1])); // Guardar contenido pila en archivo
    printf("Released bytes: %d\n", my_stack_purge(pila)); // Vaciar pila memoria
    pthread_exit(&mutex);
}

void *func() {
    for (int i = 0; i < N; i++) {
        pthread_mutex_lock(&mutex); // Región de exclusión mútua
        int *data = my_stack_pop(pila);
        pthread_mutex_unlock(&mutex); //
        (*data)++;
        pthread_mutex_lock(&mutex); // Región de exclusión mútua
        my_stack_push(pila, data);
        pthread_mutex_unlock(&mutex); //
    }
    pthread_exit(&mutex);
}
