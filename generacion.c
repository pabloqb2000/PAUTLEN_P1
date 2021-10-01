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
            fprintf(fpasm, "\t_%s resd %d\n", nombre, tamano);
            break;
        case BOOLEANO:
            fprintf(fpasm, "\t_%s resd %d\n", nombre, tamano);
            break;
        default:
            // Not implemented
            printf("Variable type not implemented\n");
            break;
    }
}

/*
Para escribir el comienzo del segmento .text, básicamente se indica que se
exporta la etiqueta main y que se usarán las funciones declaradas en la librería
alfalib.o
*/
void escribir_segmento_codigo(FILE* fpasm) {
    fprintf(fpasm, "\n");
    fprintf(fpasm, "segment .text\n");
    fprintf(fpasm, "\tglobal main\n");
    fprintf(fpasm, "\textern scan_int, print_int, scan_boolean, print_boolean\n");
    fprintf(fpasm, "\textern print_endofline, print_blank, print_string\n");
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
Al final del programa se escribe:
- El código NASM para salir de manera controlada cuando se detecta un error
en tiempo de ejecución (cada error saltará a una etiqueta situada en esta
zona en la que se imprimirá el correspondiente mensaje y se saltará a la
zona de finalización del programa).
- En el final del programa se debe:
·Restaurar el valor del puntero de pila (a partir de su variable __esp)
·Salir del programa (ret).
*/
void escribir_fin(FILE* fpasm){
    fprintf(fpasm, "\n\n");
    fprintf(fpasm, "fin_programa:\n");
    fprintf(fpasm, "\tmov esp, [__esp]\n");
    fprintf(fpasm, "\tret\n");
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

/*
- Genera el código para asignar valor a la variable de nombre nombre.
- Se toma el valor de la cima de la pila.
- El último argumento es el que indica si lo que hay en la cima de la pila es
una referencia (1) o ya un valor explícito (0).
*/
void asignar(FILE* fpasm, char* nombre, int es_variable) {
    fprintf(fpasm, "\n");
    fprintf(fpasm, "\tpop eax\n");

    if (es_variable == 1) {
        fprintf(fpasm, "\tmov eax, [eax]\n");
    }

    fprintf(fpasm, "\tmov [_%s], eax\n", nombre);
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
    fprintf(fpasm, "\n\tpop ebx\n");
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
    fprintf(fpasm, "\tcdq\n");
    fprintf(fpasm, "\timul ebx\n");

    // Almacenar resultado
    fprintf(fpasm, "\tpush eax\n");
}

void dividir(FILE* fpasm, int es_variable_1, int es_variable_2) {
    // Cargar eax y ebx de la pila
    descargar_pila(fpasm, es_variable_1, es_variable_2);
    
    // Hacer operacion
    fprintf(fpasm, "\tcdq\n");
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

/*
Función aritmética de cambio de signo.
Es análoga a las binarias, excepto que sólo requiere de un acceso a la pila ya
que sólo usa un operando.
*/
void cambiar_signo(FILE* fpasm, int es_variable){
    fprintf(fpasm, "\n\tpop eax\n");
    if (es_variable)
        fprintf(fpasm, "\tmov eax, [eax]\n");

    fprintf(fpasm, "\tmov ebx, 0\n");
    fprintf(fpasm, "\tsub ebx, eax\n");
    fprintf(fpasm, "\tpush ebx\n");
}

/*
Función monádica lógica de negación. No hay un código de operación de la ALU
que realice esta operación por lo que se debe codificar un algoritmo que, si
encuentra en la cima de la pila un 0 deja en la cima un 1 y al contrario.
El último argumento es el valor de etiqueta que corresponde (SIN LUGAR A DUDAS,
la implementación del algoritmo requerirá etiquetas,). Véase en los ejemplos de
programa principal como puede gestionarse el número de etiquetas cuantos_no.
*/
void no(FILE* fpasm, int es_variable, int cuantos_no){
    fprintf(fpasm, "\n\tpop eax\n");
    if (es_variable)
        fprintf(fpasm, "\tmov eax, [eax]\n");

    fprintf(fpasm, "\tmov ebx, eax\n");
    fprintf(fpasm, "\tadd eax, 07FFFFFFFh\n");
    fprintf(fpasm, "\tor eax, ebx\n");
    fprintf(fpasm, "\tnot eax\n");
    fprintf(fpasm, "\tshr eax, 31\n");
    fprintf(fpasm, "\tpush eax\n");
}

/* FUNCIONES COMPARATIVAS */

/*
Todas estas funciones reciben como argumento si los elementos a comparar son o
no variables. El resultado de las operaciones, que siempre será un booleano (“1”
si se cumple la comparación y “0” si no se cumple), se deja en la pila como en el
resto de operaciones. Se deben usar etiquetas para poder gestionar los saltos
necesarios para implementar las comparaciones.
*/
void igual(FILE* fpasm, int es_variable1, int es_variable2, int etiqueta) {
    descargar_pila(fpasm, es_variable1, es_variable2);
    fprintf(fpasm, "\n");
    fprintf(fpasm, "\tcmp eax, ebx\n");
    fprintf(fpasm, "\tje IGUAL_%d\n", etiqueta);        // je - jump equal
    fprintf(fpasm, "\tpush dword 0\n");        
    fprintf(fpasm, "\tjmp NO_IGUAL_%d\n", etiqueta);
    fprintf(fpasm, "IGUAL_%d:\n", etiqueta);
    fprintf(fpasm, "\tpush dword 1\n");
    fprintf(fpasm, "NO_IGUAL_%d:\n", etiqueta);
}

void distinto(FILE* fpasm, int es_variable1, int es_variable2, int etiqueta) {
    descargar_pila(fpasm, es_variable1, es_variable2);
    fprintf(fpasm, "\n");
    fprintf(fpasm, "\tcmp eax, ebx\n");
    fprintf(fpasm, "\tjne NOIGUAL_%d\n", etiqueta);     // jne - jump not equal
    fprintf(fpasm, "\tpush dword 0\n");        
    fprintf(fpasm, "\tjmp NO_NOIGUAL_%d\n", etiqueta);
    fprintf(fpasm, "NOIGUAL_%d:\n", etiqueta);
    fprintf(fpasm, "\tpush dword 1\n");
    fprintf(fpasm, "NO_NOIGUAL_%d:\n", etiqueta);
}

void menor_igual(FILE* fpasm, int es_variable1, int es_variable2, int etiqueta) {
    descargar_pila(fpasm, es_variable1, es_variable2);
    fprintf(fpasm, "\n");
    fprintf(fpasm, "\tcmp eax, ebx\n");
    fprintf(fpasm, "\tjle MENORIGUAL_%d\n", etiqueta);  // jle - jump less than or equal
    fprintf(fpasm, "\tpush dword 0\n");        
    fprintf(fpasm, "\tjmp NO_MENORIGUAL_%d\n", etiqueta);
    fprintf(fpasm, "MENORIGUAL_%d:\n", etiqueta);
    fprintf(fpasm, "\tpush dword 1\n");
    fprintf(fpasm, "NO_MENORIGUAL_%d:\n", etiqueta);
}

void mayor_igual(FILE* fpasm, int es_variable1, int es_variable2, int etiqueta) {
    descargar_pila(fpasm, es_variable1, es_variable2);
    fprintf(fpasm, "\n");
    fprintf(fpasm, "\tcmp eax, ebx\n");
    fprintf(fpasm, "\tjge MAYORIGUAL_%d\n", etiqueta);  // jge - jump greater than or equal
    fprintf(fpasm, "\tpush dword 0\n");        
    fprintf(fpasm, "\tjmp NO_MAYORIGUAL_%d\n", etiqueta);
    fprintf(fpasm, "MAYORIGUAL_%d:\n", etiqueta);
    fprintf(fpasm, "\tpush dword 1\n");
    fprintf(fpasm, "NO_MAYORIGUAL_%d:\n", etiqueta);
}

void menor(FILE* fpasm, int es_variable1, int es_variable2, int etiqueta) {
    descargar_pila(fpasm, es_variable1, es_variable2);
    fprintf(fpasm, "\n");
    fprintf(fpasm, "\tcmp eax, ebx\n");
    fprintf(fpasm, "\tjl MENOR_%d\n", etiqueta);        // jl - jump less than
    fprintf(fpasm, "\tpush dword 0\n");        
    fprintf(fpasm, "\tjmp NO_MENOR_%d\n", etiqueta);
    fprintf(fpasm, "MENOR_%d:\n", etiqueta);
    fprintf(fpasm, "\tpush dword 1\n");
    fprintf(fpasm, "NO_MENOR_%d:\n", etiqueta);
}

void mayor(FILE* fpasm, int es_variable1, int es_variable2, int etiqueta) {
    descargar_pila(fpasm, es_variable1, es_variable2);
    fprintf(fpasm, "\n");
    fprintf(fpasm, "\tcmp eax, ebx\n");
    fprintf(fpasm, "\tjg MAYOR_%d\n", etiqueta);        // jg - jump greater than
    fprintf(fpasm, "\tpush dword 0\n");        
    fprintf(fpasm, "\tjmp NO_MAYOR_%d\n", etiqueta);
    fprintf(fpasm, "MAYOR_%d:\n", etiqueta);
    fprintf(fpasm, "\tpush dword 1\n");
    fprintf(fpasm, "NO_MAYOR_%d:\n", etiqueta);
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
            fprintf(fpasm, "\n\tpush dword _%s\n", nombre);
            fprintf(fpasm, "\tcall scan_int\n");
            fprintf(fpasm, "\tadd esp, 4\n\n");
            break;

        case BOOLEANO: 
            fprintf(fpasm, "\n\tpush dword _%s\n", nombre);
            fprintf(fpasm, "\tcall scan_boolean\n");
            fprintf(fpasm, "\tadd esp, 4\n\n");
            break;

        default:
            printf("La lectura de este tipo no está implementada\n");
            break;
    }
}

void escribir(FILE* fpasm, int es_variable, int tipo){
    if(es_variable){
        fprintf(fpasm, "\n\tpop eax\n");
        fprintf(fpasm, "\tpush dword [eax]\n");
    }
    switch(tipo){
        case ENTERO:
            fprintf(fpasm, "\n\tcall print_int\n");
            fprintf(fpasm, "\tcall print_endofline\n");
            fprintf(fpasm, "\tadd esp, 4\n");
            break;
        case BOOLEANO:
            fprintf(fpasm, "\n\tcall print_boolean\n");
            fprintf(fpasm, "\tcall print_endofline\n");
            fprintf(fpasm, "\tadd esp, 4\n");
            break;
        default:
            printf("La escritura de este tipo no esta implementada.\n");
    }
}
