/*
    ### AVENTURA 2 ###
    Squad: LRAOS
    Miembros: 
        Camino, Lluís 
        López, Rubén
        Reyes, Andrea
*/

#define _POSIX_C_SOURCE 200112L
#define COMMAND_LINE_SIZE 1024
#define ARGS_SIZE 64
#define PROMPT '$'
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#pragma region //COLORES (Eliminar los que no se usen)
#define RESET_COLOR    "\x1b[0m"
#define NEGRO_T        "\x1b[30m"
#define NEGRO_F        "\x1b[40m"
#define ROJO_T     "\x1b[31m"
#define ROJO_F     "\x1b[41m"
#define VERDE_T        "\x1b[32m"
#define VERDE_F        "\x1b[42m"
#define AMARILLO_T "\x1b[33m"
#define    AMARILLO_F  "\x1b[43m"
#define AZUL_T     "\x1b[34m"
#define    AZUL_F      "\x1b[44m"
#define MAGENTA_T  "\x1b[35m"
#define MAGENTA_F  "\x1b[45m"
#define CYAN_T     "\x1b[36m"
#define CYAN_F     "\x1b[46m"
#define BLANCO_T   "\x1b[37m"
#define BLANCO_F   "\x1b[47m"
#pragma endregion

void imprimir_prompt();
char *read_line(char *line); 
int execute_line(char *line);
int parse_args(char **args, char *line);
int check_internal(char **args); 
int internal_cd(char **args); 
int internal_export(char **args); 
int internal_source(char **args); 
int internal_jobs(char **args); 
int internal_cd(char **args); 

void imprimir_prompt() {
    char dir [ARGS_SIZE];
    getcwd(dir, ARGS_SIZE);
    char ESC = 27;
    printf("%c[1m"VERDE_T"%s:~"AZUL_T"%s"ROJO_T"%c "RESET_COLOR , ESC ,getenv("USER"),dir,PROMPT);
    printf("%c[0m",ESC); /* turn off bold */
    printf("\n");
    fflush(stdout);
}

int main() {
  char line[ARGS_SIZE];
  while (read_line(line)) {
      execute_line(line);
  }
  return 0;
}

char *read_line(char *line) {
  imprimir_prompt();
  fgets(line, ARGS_SIZE, stdin);
  return line;
}

int execute_line(char *line) {
    char *args[ARGS_SIZE];
    parse_args(args, line);
    return check_internal(args);
}

int parse_args(char **args, char *line) {
  const char s[3]="\t\n ";
  char * token;
  token = strtok(line, s);
  int i = 0;
  while (token != NULL) {
    if (token[0] != '#') {
      printf("Token nº %d: %s\n", i, token); // esta línea se debe eliminar después
      args[i] = token;
      i++;
    }
    token = strtok(NULL, s);
  }
  args[i] = token;
  return i;
}

int check_internal(char **args) {
  int comp = strcmp(*args, "cd");
  if (comp == 0) {
    return internal_cd(args);
  }
  comp = strcmp(*args, "export");
  if (comp == 0) {
    return internal_export(args);
  }
  comp = strcmp(*args, "source");
  if (comp == 0) {
    return internal_source(args);
  }
  comp = strcmp(*args, "jobs");
  if (comp == 0) {
    return internal_jobs(args);
  }
  return 0;
}

int internal_cd(char **args) {
  printf("Función CD\n");
  return 1;
}

int internal_export(char **args) {
  printf("Función export\n");
  return 1;
}
int internal_source(char **args) {
  printf("Función source\n");
  return 1;
}
int internal_jobs(char **args) {
  printf("Función Jobs\n");
  return 1;
}
