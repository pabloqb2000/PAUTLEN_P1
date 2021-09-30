#include "generacion.h"

/*
Código para el principio de la sección .bss.
Con seguridad sabes que deberás reservar una variable entera para guardar el
puntero de pila extendido (esp). Se te sugiere el nombre __esp para esta variable.
*/
void escribir_cabecera_bss(FILE* fpasm) {
    fprintf(fpasm, "\n");
    fprintf(fpasm, "segment .bss\n");
    fprintf(fpasm, "\t__esp resd 1\n");
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
            fprintf(fpasm, "\t%s resd %d", nombre, tamano);
            break;
        case BOOLEANO:
            fprintf(fpasm, "\t%s resd %d", nombre, tamano);
            break;
        default:
            // Not implemented
            printf("Variable type not implemented\n");
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
    fprintf(fpasm, "\tmov dword [__esp], esp\n");
}

/*
Función que debe ser invocada cuando se sabe un operando de una operación
aritmético-lógica y se necesita introducirlo en la pila.
- nombre es la cadena de caracteres del operando tal y como debería aparecer
en el fuente NASM
- es_variable indica si este operando es una variable (como por ejemplo b1)
con un 1 u otra cosa (como por ejemplo 34) con un 0. Recuerda que en el
primer caso internamente se representará como _b1 y, sin embargo, en el
segundo se representará tal y como esté en el argumento (34).
*/
void escribir_operando(FILE* fpasm, char* nombre, int es_variable) {
    if (es_variable) {
        fprintf(fpasm, "\tpush dword _%s\n", nombre);
    } else {
        fprintf(fpasm, "\tpush dword %s\n", nombre);
    }
}

/* FUNCIONES ARITMÉTICO-LÓGICAS BINARIAS */

/*
En todas ellas se realiza la operación como se ha resumido anteriormente:
- Se extrae de la pila los operandos
- Se realiza la operación
- Se guarda el resultado en la pila
Los dos últimos argumentos indican respectivamente si lo que hay en la pila es
una referencia a un valor o un valor explícito.
Deben tenerse en cuenta las peculiaridades de cada operación. En este sentido
sí hay que mencionar explícitamente que, en el caso de la división, se debe
controlar si el divisor es “0” y en ese caso se debe saltar a la rutina de error
controlado (restaurando el puntero de pila en ese caso y comprobando en el retorno
que no se produce “Segmentation Fault”)
*/

void descargar_pila(FILE* fpasm, int es_variable_1, int es_variable_2) {
    // Almacenar variable 2
    fprintf(fpasm, "\tpop ebx\n");
    if (es_variable_2)
        fprintf(fpasm, "\tmov ebx, [ebx]\n");

    // Almacenar variable 1
    fprintf(fpasm, "\tpop eax\n");
    if (es_variable_1)
        fprintf(fpasm, "\tmov eax, [eax]\n");
}

void sumar(FILE* fpasm, int es_variable_1, int es_variable_2) {
    // Cargar eax y ebx de la pila
    descargar_pila(fpasm, es_variable_1, es_variable_2);

    // Hacer operacion
    fprintf(fpasm, "\tadd eax, ebx\n");

    // Almacenar resultado
    fprintf(fpasm, "\tpush eax\n");
}

void restar(FILE* fpasm, int es_variable_1, int es_variable_2) {
    // Cargar eax y ebx de la pila
    descargar_pila(fpasm, es_variable_1, es_variable_2);

    // Hacer operacion
    fprintf(fpasm, "\tsub eax, ebx\n");

    // Almacenar resultado
    fprintf(fpasm, "\tpush eax\n");
}

void multiplicar(FILE* fpasm, int es_variable_1, int es_variable_2) {
    // Cargar eax y ebx de la pila
    descargar_pila(fpasm, es_variable_1, es_variable_2);
    
    // Hacer operacion
    fprintf(fpasm, "\tmov edx\n");
    fprintf(fpasm, "\timul ebx\n");

    // Almacenar resultado
    fprintf(fpasm, "\tpush eax\n");
}

void divide(FILE* fpasm, int es_variable_1, int es_variable_2) {
    // Cargar eax y ebx de la pila
    descargar_pila(fpasm, es_variable_1, es_variable_2);
    
    // Hacer operacion
    fprintf(fpasm, "\tidiv ebx\n");
    // CONTROL DE ERRORES

    // Almacenar resultado
    fprintf(fpasm, "\tpush eax\n");
}

void y(FILE* fpasm, int es_variable_1, int es_variable_2) {
    // Cargar eax y ebx de la pila
    descargar_pila(fpasm, es_variable_1, es_variable_2);
    
    // Hacer operacion
    fprintf(fpasm, "\tand eax, ebx\n");

    // Almacenar resultado
    fprintf(fpasm, "\tpush eax\n");
}

void o(FILE* fpasm, int es_variable_1, int es_variable_2) {
    // Cargar eax y ebx de la pila
    descargar_pila(fpasm, es_variable_1, es_variable_2);
    
    // Hacer operacion
    fprintf(fpasm, "\tor eax, ebx\n");

    // Almacenar resultado
    fprintf(fpasm, "\tpush eax\n");
}

/* FUNCIONES DE ESCRITURA Y LECTURA */

/*
Se necesita saber el tipo de datos que se va a procesar (ENTERO o BOOLEANO) ya
que hay diferentes funciones de librería para la lectura (idem. escritura) de cada
tipo.
Se deben insertar en la pila los argumentos necesarios, realizar la llamada
(call) a la función de librería correspondiente y limpiar la pila.
*/
void leer(FILE* fpasm, char* nombre, int tipo) {
    switch (tipo) {
        case ENTERO:
            fprintf(fpasm, "\tpush dword %s", nombre);
            fprintf(fpasm, "\tcall scan_int\n");
            fprintf(fpasm, "\tcall add esp, 4\n\n");
            break;

        case BOOLEANO: 
            fprintf(fpasm, "\tpush dword %s", nombre);
            fprintf(fpasm, "\tcall scan_boolean\n");
            fprintf(fpasm, "\tcall add esp, 4\n\n");
            break;

        default:
            printf("La lectura de este tipo no está implementada\n");
            break;
    }
}
