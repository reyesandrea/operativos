Entrega Aventura 2
Squad: LRAOS
Miembros: 
  - Camino, Lluís 
  - López, Rubén
  - Reyes, Andrea
  
Nuestro shell cumple con los siguientes requisitos:
  - Prompt con variables, colores y negritas
  - Comando CD y CD avanzado
  - Comandos export y source
  - Permite ejecutar comandos externos
  - Gestión de ^C y ^Z
  - Comando jobs
  - Direccionamiento (>)
  - Entrada de comandos con teclas especiales (librería READLINE)
  
Además hemos implementado los comandos opcionales:
  - FG
  - BG
  
Los archivos fuente my_shell.c y nivel7.c se pueden compilar con el archivo Makefile o directamente con el comando:
  - gcc -std=c99 my_shell.c -o my_shell -lreadline para my_shell.c ó
  - gcc nivel7.c -o nivel7 para nivel7.c
