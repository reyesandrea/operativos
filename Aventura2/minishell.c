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

char *read_line(char *line); 
int execute_line(char *line);
int parse_args(char **args, char *line);
int check_internal(char **args); 
int internal_cd(char **args); 
int internal_export(char **args); 
int internal_source(char **args); 
int internal_jobs(char **args); 
int my_strcmp(const char *str1, const char *str2);

int imprimir_prompt(){
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
  char *aux = line; 
  while (read_line(aux)){
      execute_line(aux);
  }

  return 0;
}

char *read_line(char *line){
  imprimir_prompt();
  fgets(line, ARGS_SIZE, stdin);
  return line;
}

int execute_line(char *line){
    int i;
    char *args[ARGS_SIZE];
    i= parse_args(args, line);
    i = check_internal(args);
}

int parse_args(char **args, char *line){
  char str[] = read_line(line);
  const char s[2] = "\t\n\r";
  char token;
  int i = 1;
  token = strtok(str, s);
  while( token != NULL ) { // añadir condición para que ignore comentarios
    
    /* ################################ */
    printf("Token nº %d: %s\n", i, token); // esta línea se debe eliminar después
    /* ################################ */

    token = strtok(NULL, s);
    i++;
  }
  return i-1;
}

int check_internal(char **args){
  int comp = my_strcmp(args, "cd"), r;
  if (comp == 0){
    r = internal_cd(args);
  }else{
    comp = my_strcmp(args, "export");
    if (comp == 0){
      r = internal_export(args);
    }else{
      comp = my_strcmp(args, "source");
      if (comp == 0){
        r = internal_source(args);
      }else{
        r = internal_jobs(args);
      }
    }
  }
  return r;
}

// int internal_cd(char **args); 
// int internal_export(char **args); 
// int internal_source(char **args); 
// int internal_jobs(char **args);

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
    len1 = strlen(str1);
    len2 = strlen(str2);
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
