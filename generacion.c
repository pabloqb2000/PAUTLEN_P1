#include "generacion.h"
#include <stdio.h>

/*
Código para el principio de la sección .bss.
Con seguridad sabes que deberás reservar una variable entera para guardar el
puntero de pila extendido (esp). Se te sugiere el nombre __esp para esta variable.
*/
void escribir_cabecera_bss(FILE* fpasm) {
    fprintf(fpasm, "\n");
    fprintf(fpasm, "segment .bss\n");
    fprintf(fpasm, "__esp resd 1\n");
}

/*
Declaración (con directiva db) de las variables que contienen el texto de los
mensajes para la identificación de errores en tiempo de ejecución.
En este punto, al menos, debes ser capaz de detectar la división por 0.
*/
void escribir_subseccion_data(FILE* fpasm) {
    fprintf(fpasm, "\n");
    fprintf(fpasm, "segment .data\n");
}

/*
Para ser invocada en la sección .bss cada vez que se quiera declarar una
variable:
- El argumento nombre es el de la variable.
- tipo puede ser ENTERO o BOOLEANO (observa la declaración de las constantes
del principio del fichero).
- Esta misma función se invocará cuando en el compilador se declaren
vectores, por eso se adjunta un argumento final (tamano) que para esta
primera práctica siempre recibirá el valor 1.
*/
void declarar_variable(FILE* fpasm, char * nombre, int tipo, int tamano) {
    switch (tipo) {
        case ENTERO:
            fprintf(fpasm, "%s resd %d", nombre, tamano);
            break;
        case BOOLEANO:
            fprintf(fpasm, "%s resb %d", nombre, tamano);
            break;
        default:
            break;
    }
}

/*
En este punto se debe escribir, al menos, la etiqueta main y la sentencia que
guarda el puntero de pila en su variable (se recomienda usar __esp).
*/
void escribir_inicio_main(FILE* fpasm) {
    fprintf(fpasm, "\n");
    fprintf(fpasm, "main:\n");
    fprintf(fpasm, "mov dword [__esp], esp\n");
}

