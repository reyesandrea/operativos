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
#define N_JOBS 64
#define TRUE 1
#define FALSE 0
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> //errno
#include <string.h> //strerror()
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdbool.h>
#include <fcntl.h>

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

struct info_process {
    pid_t pid;
    char status; // ’E’, ‘D’, ‘F’
    char command_line[COMMAND_LINE_SIZE]; // Comando
};
int n_pids;
static struct info_process jobs_list[N_JOBS];

void imprimir_prompt();
char *read_line(char *line); 
int execute_line(char *line);
int parse_args(char **args, char *line);
char *my_strcat(char *dest, const char *src);
size_t my_strlen(const char *str);
int check_internal(char **args); 
int internal_cd(char **args); 
int internal_export(char **args); 
int internal_source(char **args); 
int internal_jobs(char **args); 
void reaper(int signum);
void ctrlc(int signum);
int jobs_list_add(pid_t pid, char status, char *command_line);
int jobs_list_find(pid_t pid);
int  jobs_list_remove(int pos);
void reaper(int signum);
int internal_jobs();
void ctrlz(int signum);
int internal_fg(char **args);
int internal_bg(char **args);
int is_background(char* line);
int is_output_redirection(char **args);


void imprimir_prompt() {
    char dir [ARGS_SIZE];
    getcwd(dir, ARGS_SIZE);
    char ESC = 27;
    printf("%c[1m"VERDE_T"%s:~"AZUL_T"%s"ROJO_T"%c "RESET_COLOR , ESC ,getenv("USER"),dir,PROMPT);
    printf("%c[0m",ESC); /* turn off bold */
    fflush(stdout);
}


int main() {
  char line[ARGS_SIZE];
  signal(SIGCHLD, reaper); 
  signal(SIGINT, ctrlc); // Asociación de la señal SIGINT
  signal(SIGTSTP, ctrlz); // Asociación de la señal SIGTSTP a la función ctrlz
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


int is_background(char* line) { 
  if (line[(strlen(line) - 2)] == '&') { 
    line[(strlen(line) - 2)] = 0; 
    return TRUE; 
  } 
  return FALSE; 
}

int is_output_redirection(char **args) {
  int i = 0;
  while (args[i] != NULL) {
    if (strcmp(args[i], ">") == 0 && args[i + 1] != NULL) {
      args[i] = NULL;
      int fd = open(args[i + 1], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
      dup2(fd, 1);
      close(fd);
      return TRUE;
    }
    i++;
  }
  return FALSE;
}

int execute_line(char *line) {
    char lineAux[ARGS_SIZE];
    strcpy(lineAux, line);
    lineAux[(strlen(lineAux) - 1)] = 0;
    char *args[ARGS_SIZE];
    int bg = is_background(line);
    //printf("%s", line);
    parse_args(args, line);
    int check = check_internal(args);
    int stdout = dup(1);
    is_output_redirection(args);
    if (check == FALSE) { // Ejecutar comando externo
      pid_t pid = fork();
      if (pid == 0) { // Proceso hijo
        signal(SIGCHLD, SIG_DFL);
        signal(SIGINT, SIG_IGN);
        if (bg == TRUE) {
          signal(SIGTSTP, SIG_IGN);
        }
        printf("[execute_line() → PID Hijo: %d]\n", getpid());
        if (execvp(args[0], args) == -1) {
          fprintf(stderr, "%s\n", strerror(errno));
          //jobs_list[0].pid = 0;
          //exit(FALSE);
        }
        dup2(stdout, 1);
        close(stdout);
        reaper(SIGCHLD);
      } else if (pid > 0) { // Proceso padre
        dup2(stdout, 1);
        close(stdout);
        printf("[execute_line() → PID Padre: %d]\n", getpid());
        if (bg == TRUE) {
          jobs_list_add(pid, 'E', lineAux);
        } else {
          jobs_list[0].pid = pid;
          strcpy(jobs_list[0].command_line, lineAux);
          while (jobs_list[0].pid != 0) {
            pause();
          }
        }
      } else {
        // Error
      }
      return TRUE;
    } else {
      return check;
    }
}

/**
 * Divide una instrucción en tokens, que guarda en el 
 * parámetro **args.
 * @param args, line
 * @return Número de tokens
 */ 
int parse_args(char **args, char *line) {
  const char s[4]="\t\n ";
  char * token;
  token = strtok(line, s);
  int i = 0;
  while (token != NULL) {
    if (token[0] != '#') {
      args[i] = token;
      i++;
    }
    token = strtok(NULL, s);
  }
  args[i] = token;
  return i;
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
 * Función booleana que averigua si args[0] se trata de un
 * comando interno y llama a la función correspondiente para 
 * tratarlo.
 * @param args
 * @return:
 *          FALSE: si no se trata de un comando interno
 *          TRUE: se ha ejecutado un comando interno
 */
int check_internal(char **args) {
  int comp = strcmp(args[0], "cd");
  if (comp == 0) {
    return internal_cd(&args[0]);
  }
  comp = strcmp(args[0], "export");
  if (comp == 0) {
    return internal_export(&args[0]);
  }
  comp = strcmp(args[0], "source");
  if (comp == 0) {
    return internal_source(&args[0]);
  }
  comp = strcmp(args[0], "jobs");
  if (comp == 0) {
    return internal_jobs(&args[0]);
  }
  comp = strcmp(args[0], "fg");
  if (comp == 0) {
    return internal_fg(&args[0]);
  }
  comp = strcmp(args[0], "bg");
  if (comp == 0) {
    return internal_bg(&args[0]);
  }
  comp = strcmp(args[0], "exit");
  if (comp == 0) {
    exit(0);
  }
  return FALSE;
}


/* 
 * Función que notifica la sintaxis correcta de la instrucción
 * utilizando la salida estandar de errores stderr.
 * @param args
 * @return:
 *          TRUE: no ocurrieron errores en la ejecución
 *         -1: ocurrió un error durante la ejecucuón
 */
int internal_cd(char **args) {
  int r, i=2;
  char s[180], *aux, *ruta, *delim = "'\\\"";

  /* ### Línea de test - Eliminar después ### */
  printf("Ruta anterior: [internal_cd() → %s] \n", getcwd(s,sizeof(s)));
  /* ################################ */

  if (args[1]==NULL){
    r = chdir(getenv("HOME"));

    /* ### Línea de test - Eliminar después ### */
    printf("Ruta actual: [internal_cd() → %s] \n", getcwd(s,sizeof(s)));
    /* ################################ */

    if (r==-1){
      fprintf(stderr, "chdir: %s\n", strerror(errno));
    }
    
    return r == 0 ? TRUE : -1;

  }else{

    if(args[2] != NULL){ // cd avanzado
      /* Reserva de memoria para aux, que es donde se
         almacenará la concatenación de los tokens */
      if(!(aux = (char *) malloc(COMMAND_LINE_SIZE))) return -1; 
      strcpy(aux, args[1]);
   
      while (args[i]!=NULL) { // Concatenación de tokens
        strcat(aux, " ");
        strcat(aux, args[i]);
        i++;
      }

      /* Reserva de memoria para ruta, que es donde se
         almacenará la nueva ruta sin caracteres no deseados */
      if (!(ruta = malloc(COMMAND_LINE_SIZE))) return -1;
      char *str = strtok(aux, delim);

      while (str!=NULL) {
        ruta = strcat(ruta, str);
        str = strtok(NULL,delim);
      }
    
      r = chdir(ruta);  

      // Liberar espacio reservado      
      free(aux);    
      free(ruta);

      printf("Ruta actual: [internal_cd() → %s] \n", getcwd(s,sizeof(s)));
      if (r==-1){
        fprintf(stderr, "chdir: %s\n", strerror(errno));
      }
      return r == 0 ? TRUE : -1;

    }else{
      r = chdir(args[1]);

      /* ### Línea de test - Eliminar después ### */
      printf("Ruta actual: [internal_cd() → %s] \n", getcwd(s,sizeof(s)));
      /* ################################ */
      if (r==-1){
        fprintf(stderr, "chdir: %s\n", strerror(errno));
      }
      return r == 0 ? TRUE : -1;
    }
  }
}


/**
 * Comando interno export, define una variable de entorno.
 * Sintaxis: export Nombre=Valor
 * @param args
 * @return:
 *          TRUE: ejecución correcta
 *          -1: error en la ejecución
 */ 
int internal_export(char **args){
  //Control de errores: sin argumentos despues de export
  if(args[1] == NULL){
    fprintf(stderr, "Error de sintaxis. Uso: export Nombre=Valor\n");
    return -1;//error
  }
  //Separación en tokens de la instruccion
  char *nom = strtok(args[1], "=");
  char *val = strtok(NULL, " ");

  //Control de errores: uso incorrecto de los argumentos contiguos a export
  if(val == NULL){
    fprintf(stderr, "Error de sintaxis. Uso: export Nombre=Valor\n");
    return -1;//error
  }

  printf("[internal_export()→ Esta función asignará valores a variables de entorno]\n");
  printf("[internal_export()→ Nombre: %s]\n", nom);
  printf("[internal_export()→ Valor: %s]\n",val);

  printf("[internal_export()→ antiguo valor para %s: %s]\n", nom, getenv(nom));
  setenv(nom, val, 1);     
  printf("[internal_export()→ nuevo valor para %s: %s]\n",nom,getenv(nom) );

  return TRUE;
} 


/**
 * Comando interno source, lee un fichero línea a línea
 * Sintaxis: source <nombre_fichero>
 * @param args
 * @return:
 *          0: ejecución correcta
 *         -1: error en la ejecución
 */ 
int internal_source(char **args) {
  FILE *fp;
  char str[100];
  int e;
  printf("Función source\n");
  if (args[1]==NULL){
    fprintf(stderr, "Error de sintaxis. Uso: source <nombre_fichero>\n");
    return -1;
  }else{
    fp = fopen(args[1], "r");   // se abre el fichero
    if(fp==NULL){ // se comprueba que existe el fichero
      fprintf(stderr, "fopen: %s\n", strerror(errno));
      return -1;
    }
    while(fgets(str, 100, fp)){ // se lee el fichero línea a línea
      e = execute_line(str);
      fflush(stdin);  // se limpia el stream del fichero
    }
    e = fclose(fp);   // se cierra el fichero
    if(e==-1){
      fprintf(stderr, "fclose: %s\n", strerror(errno));
      return e;
    }
  }
  return TRUE;
}


/**
 * Función que imprime por pantalla el identificador de
 * trabajo, su PID, la línea de comandos y el estatus
 * (D de Detenido, E de Ejecutando)
 * @param: args
 **/
int internal_jobs(char **args) {
  int i = 1;
  while (jobs_list[i].pid!=0){
    printf("[%d] %d   %c    %s \n", i, jobs_list[i].pid, jobs_list[i].status, jobs_list[i].command_line);
    i++;
  }
  return TRUE;
}


/**
 * Función que controla si el hijo que acaba es el que se ejecuta en primer plano 
 * (waitpid() devuelve el pid del hijo que ha terminado) y, en tal caso, reseteará 
 * los datos de jobs_list[0].pid. En caso de estar en background, llamará a la
 * función  jobs_list_find() para buscar en la lista de trabajos el PID del proceso 
 * que ha acabado.
 * Imprime por la salida estándard de errores que ese proceso ha terminado (indicando 
 * los datos del mismo) y llama a la función jobs_list_remove() para eliminar el 
 * proceso de la lista.
 * @param: signum
 **/
void reaper(int signum){
  int status, posicion=0;
  signal(SIGCHLD,reaper);
  signal(SIGINT,ctrlc);
  pid_t pidaux=waitpid(-1,&status,WNOHANG);
  while(pidaux>0){
    
    if(pidaux == jobs_list[0].pid){//proceso que acaba en primer plano      
      if(WIFEXITED(status)){
        fprintf(stderr,"\n[reaper()→ Proceso hijo %d en foreground (%s) finalizado con exit code %d]\n", pidaux, jobs_list[posicion].command_line, WEXITSTATUS(status));
      }else if(WIFSIGNALED(status)){
        fprintf(stderr,"\n[reaper()→ Proceso hijo %d en foreground (%s) finalizado con señal numero %d]\n", pidaux, jobs_list[posicion].command_line, WTERMSIG(status));
      }
      jobs_list[0].pid = 0;
      jobs_list[0].status = 'F';
      strcpy(jobs_list[0].command_line,"\0");
      pidaux=waitpid(-1,&status,WNOHANG);
      fflush(stdout);
    }else{
      posicion = jobs_list_find(pidaux);
       if(WIFEXITED(status)){
         fprintf(stderr,"\n[reaper()→ Proceso hijo %d (%s) finalizado con exit code %d]\n", pidaux,jobs_list[posicion].command_line,WEXITSTATUS(status));
       }else if(WIFSIGNALED(status)){
         fprintf(stderr,"\n[reaper()→ Proceso hijo %d (%s) finalizado con señal numero %d]\n", pidaux,jobs_list[posicion].command_line,WTERMSIG(status));
       }
      if(posicion != -1){
        jobs_list_remove(posicion);
      }
      pidaux=waitpid(-1,&status,WNOHANG);
    }
  }
}


/**
 * Función que actúa como un manejador propio para la 
 * señal SIGINT (Ctrl+C)
 * @param signum: número que identifica la señal recibida
 **/
void ctrlc(int signum){
  signal(signum, ctrlc);
  char mensaje[1500];
  sprintf(mensaje, "\n[ctrlc()→ Soy el proceso con PID %d, el proceso en foreground es %d]\n",getpid(),jobs_list[0].pid);
  write(2, mensaje, strlen(mensaje));

  if(jobs_list[0].pid > 0){
    if(strcmp(jobs_list[0].command_line, "./minishell") != 0) {
      if(kill(jobs_list[0].pid,SIGTERM)==0){
        sprintf(mensaje, "\n[ctrlc()→ Señal %d enviada al proceso %d", signum,  getpid());
        write(2, mensaje, strlen(mensaje));
      }else{
        perror("kill");
        exit(-1);
      }
    }else{
      sprintf(mensaje, "\n[ctrlc()→ Error: Señal %d no enviada por %d debido a que el proceso en el foreground es el shell]\n", SIGTERM, signum);
      write(2, mensaje, strlen(mensaje));
    }
  }else{
    sprintf(mensaje, "\n[ctrlc()→ Error: Señal %d no enviada por %d debido a que no hay ningún proceso en foreground]\n", SIGTERM, getpid());
    write(2, mensaje, strlen(mensaje));
  }
}


int jobs_list_remove(int pos){
    jobs_list[pos].pid = jobs_list[n_pids-1].pid;
    jobs_list[pos].status = jobs_list[n_pids-1].status;
    strcpy(jobs_list[pos].command_line,jobs_list[n_pids-1].command_line);
    jobs_list[n_pids-1].pid = 0;
    n_pids--;
}

int jobs_list_add(pid_t pid, char status, char *command_line){
  if(n_pids<ARGS_SIZE){
    n_pids++;
    jobs_list[n_pids].pid = pid;
    jobs_list[n_pids].status = status;
    strcpy(jobs_list[n_pids].command_line , command_line);
    printf("[%d]  %d    %c  %s \n", n_pids, pid, status, command_line);
  }else{
    return -1;//error nº maximo alcanzado
  }
}


/**
 * Función que busca en el array de trabajos el PID que recibe como
 * argumento
 * @param pid
 * @return 
 *          i: posicion del PID en el array de trabajos
 **/
int jobs_list_find(pid_t pid){
  bool found = FALSE;
  int i = 1;
  while (found == FALSE){
    if (jobs_list[i].pid == pid){
      found = TRUE;
      return i;
    }
    i++;
  }
} 


/**
 * Función que actúa como un manejador propio para la 
 * señal SIGTSTP (Ctrl+Z)
 * @param signum: número que identifica la señal recibida
 **/
void ctrlz(int signum){
  signal(signum, ctrlz);
  char mensaje[1500];
  if(jobs_list[0].pid > 0){
    // Verificación de que el proceso en el foreground no sea el minishell
    if(strcmp(jobs_list[0].command_line, "./minishell") != 0) {
      if(kill(jobs_list[0].pid,SIGTSTP)==0){
        jobs_list[0].status = 'D'; // El proceso se detuvo

        // Se añaden los datos del proceso detenido a jobs_list[n_pids]
        jobs_list_add(jobs_list[0].pid, jobs_list[0].status, jobs_list[0].command_line);
        
        sprintf(mensaje, "\n[ctrlz()→ Soy el proceso con PID %d, el proceso en foreground es %d (%s)]",getpid(),jobs_list[0].pid, jobs_list[0].command_line);
        write(2, mensaje, strlen(mensaje));
        sprintf(mensaje, "\n[ctrlz()→ Señal %d (SIGTSTP) enviada al proceso %d (%s) por %d ]\n", signum, jobs_list[0].pid, jobs_list[0].command_line, getpid());
        write(2, mensaje, strlen(mensaje));
        // Reseteo de los datos de jobs_list[0]
        jobs_list[0].pid = 0;
        strcpy(jobs_list[0].command_line,"\0");
        jobs_list[0].status = 'F';
      }else{
        perror("kill");
        exit(-1);
      }
    }else{
      sprintf(mensaje, "\n[ctrlz() → Error: Señal %d no enviada por %d debido a que el proceso en el foreground es el shell]\n", SIGTSTP, signum);
      write(2, mensaje, strlen(mensaje));
    }
  }else{
    sprintf(mensaje, "\n[ctrlz() → Error: Señal %d no enviada por %d debido a que no hay ningún proceso en foreground]\n", SIGTSTP, getpid());
    write(2, mensaje, strlen(mensaje));
  }
}


/**
 * Función que envía un trabajo detenido al foreground reactivando
 * su ejecución, o uno del background al foreground.
 * @param args: line
 **/
int internal_fg(char **args){
  
  int pos = *args[1];
  pos = pos - '0';
  char mensaje[1500], *line, *esp=" ";
  if(args[1] == NULL){  // Chequeo de sintaxis
    fprintf(stderr, "[internal_fg() → Error de sintaxis. Uso: fg nº_de_trabajo]\n");
    return -1; //error
  }else{
    if (pos >= n_pids || pos==0){ // Verificación de existencia del trabajo
      fprintf(stderr, "[internal_fg() → Error. No existe ese trabajo.]\n");
      return -1; //error
    }
    if (jobs_list[pos].status == 'D'){
      if(kill(jobs_list[pos].pid,SIGCONT)==0){
        jobs_list[pos].status = 'E';
    
        const char s[2] = "&";
        
        strcpy(jobs_list[0].command_line, strtok(jobs_list[pos].command_line, s));

        jobs_list[0].pid = jobs_list[pos].pid;
        jobs_list[0].status = jobs_list[pos].status;

        jobs_list_remove(pos);
        printf("[internal_fg() → Señal %d (SIGCONT) enviada a %d (%s)] \n", SIGCONT, jobs_list[0].pid,jobs_list[0].command_line);
        printf("%s\n", jobs_list[0].command_line);
        
      }else{
        perror("kill");
        exit(-1);
      }
    }else{
      sprintf(mensaje, "[internal_fg() → Error: Señal 18 (SIGCONT) no enviada debido a que el proceso en el foreground es el shell]\n");
      write(2, mensaje, strlen(mensaje));
      return -1;
    }
  }
    while(jobs_list[0].pid>0){
    pause();
  }
  return TRUE;
}

/**
* Función que reactivar un proceso detenido para que 
* siga ejecutándose pero en segundo plano
**/
int internal_bg(char **args){
    int pos = *args[1];
    if (pos >= n_pids || pos == 0) {
         fprintf(stderr,"No exite ese trabajo");
        return -1;
    } else if (jobs_list[pos].status== 'E'){
        fprintf(stderr,"El trabajo ya está en segundo plano");
        return -1;
    } else {
        jobs_list[pos].status= 'E';
        strcat(jobs_list[pos].command_line, " &");
        if(kill(jobs_list[pos].pid,SIGCONT)==0){
            printf("[internal_bg()→ señal 18 (SIGCONT) enviada a %d %s", getpid(), jobs_list[pos].command_line);
            printf("\n[%d] %d   %c    %s \n", pos, jobs_list[pos].pid, jobs_list[pos].status, jobs_list[pos].command_line);
        
        }else{
            perror("kill");
            return -1;
      }
    }
}