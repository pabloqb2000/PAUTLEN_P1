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
    fprintf(fpasm, "segment .data\n");
    fprintf(fpasm, "\terror_division_0 db \"Error division por 0!\", 0\n");
    fprintf(fpasm, "\terror_tamano_array db \"El indice supera el tamano del array!\", 0\n");
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
    fprintf(fpasm, "\tmov dword [__esp], esp\n\n");
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

    // Error division entre 0
    fprintf(fpasm, "\n\n");
    fprintf(fpasm, "division_entre_0_fin:\n");
    fprintf(fpasm, "\tpush dword error_division_0\n");
    fprintf(fpasm, "\tcall print_string\n");
    fprintf(fpasm, "\tadd esp, 4\n");
    fprintf(fpasm, "\tcall print_endofline\n");
    fprintf(fpasm, "\tjmp fin_programa\n");

    // Error tamaño array
    fprintf(fpasm, "\n\n");
    fprintf(fpasm, "tamano_array_fin:\n");
    fprintf(fpasm, "\tpush dword error_tamano_array\n");
    fprintf(fpasm, "\tcall print_string\n");
    fprintf(fpasm, "\tadd esp, 4\n");
    fprintf(fpasm, "\tcall print_endofline\n");
    fprintf(fpasm, "\tjmp fin_programa\n");
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

    // Comprobar division entre 0
    fprintf(fpasm, "\tcmp ebx, 0\n");
    fprintf(fpasm, "\tje division_entre_0_fin\n");

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


/* FUNCIONES DE ESTRUCTURA */

/*
Generación de código para el inicio de una estructura if-then-else
Como es el inicio de uno bloque de control de flujo de programa que requiere de una nueva
etiqueta deben ejecutarse antes las tareas correspondientes a esta situación
exp_es_variable
Es 1 si la expresión de la condición es algo asimilable a una variable (identificador,
elemento de vector)
Es 0 en caso contrario (constante u otro tipo de expresión)
*/
void ifthenelse_inicio(FILE * fpasm, int exp_es_variable, int etiqueta) {
    // Sacamos de la pila el resultado de la comparacion (expresion logica del if)
    fprintf(fpasm, "\tpop eax\n");
    if (exp_es_variable)
        fprintf(fpasm, "\tmov eax, [eax]\n");
    
    fprintf(fpasm, "\tcmp eax, 0\n");   // Si la exp. es False, saltamos al final del bloque 'ifthenelse_then'.
    fprintf(fpasm, "\tje ifthenelse_fin_then_%d\n", etiqueta);
}

/*
Generación de código para el inicio de una estructura if-then
Como es el inicio de uno bloque de control de flujo de programa que requiere de una nueva
etiqueta deben ejecutarse antes las tareas correspondientes a esta situación
exp_es_variable
Es 1 si la expresión de la condición es algo asimilable a una variable (identificador,
elemento de vector)
Es 0 en caso contrario (constante u otro tipo de expresión)
*/
void ifthen_inicio(FILE * fpasm, int exp_es_variable, int etiqueta) {
    // Sacamos de la pila el resultado de la comparacion (expresion logica del if)
    fprintf(fpasm, "\tpop eax\n");
    if (exp_es_variable)
        fprintf(fpasm, "\tmov eax, [eax]\n");

    fprintf(fpasm, "\tcmp eax, 0\n");   // Si la exp. es False, saltamos al final del bloque 'ifthen'.
    fprintf(fpasm, "\tje ifthen_fin_%d\n", etiqueta);
}

/*
Generación de código para el fin de una estructura if-then
Como es el fin de uno bloque de control de flujo de programa que hace uso de la etiqueta
del mismo se requiere que antes de su invocación tome el valor de la etiqueta que le toca
según se ha explicado
Y tras ser invocada debe realizar el proceso para ajustar la información de las etiquetas
puesto que se ha liberado la última de ellas.
*/
void ifthen_fin(FILE * fpasm, int etiqueta) {
    fprintf(fpasm, "\nifthen_fin_%d:\n", etiqueta);
}

/*
Generación de código para el fin de la rama then de una estructura if-then-else
Sólo necesita usar la etiqueta adecuada, aunque es el final de una rama, luego debe venir
otra (la rama else) antes de que se termine la estructura y se tenga que ajustar las etiquetas
por lo que sólo se necesita que se utilice la etiqueta que corresponde al momento actual.
*/
void ifthenelse_fin_then(FILE * fpasm, int etiqueta) {
    fprintf(fpasm, "\tjmp ifthenelse_fin_%d\n", etiqueta);
    fprintf(fpasm, "\nifthenelse_fin_then_%d:\n", etiqueta);
}

/*
Generación de código para el fin de una estructura if-then-else
Como es el fin de uno bloque de control de flujo de programa que hace uso de la etiqueta
del mismo se requiere que antes de su invocación tome el valor de la etiqueta que le toca
según se ha explicado
Y tras ser invocada debe realizar el proceso para ajustar la información de las etiquetas
puesto que se ha liberado la última de ellas.
*/
void ifthenelse_fin( FILE * fpasm, int etiqueta) {
    fprintf(fpasm, "\nifthenelse_fin_%d:\n", etiqueta);
}

/*
Generación de código para el inicio de una estructura do-while
Como es el inicio de uno bloque de control de flujo de programa en este ejercicio opcional no es necesario
 tener encuenta control de etiquetas para do-while anidado.
*/
void dowhile_inicio(FILE * fpasm, int etiqueta) {
    fprintf(fpasm, "\ndowhile_inicio_%d:\n", etiqueta);
}

/*
Generación de código para el momento en el que se ha generado el código de la expresión
de control del bucle
Sólo necesita usar la etiqueta adecuada, por lo que sólo se necesita que se recupere el valor
de la etiqueta que corresponde al momento actual.
exp_es_variable
Es 1 si la expresión de la condición es algo asimilable a una variable (identificador,
o elemento de vector)
Es 0 en caso contrario (constante u otro tipo de expresión)
*/
void dowhile_exp_pila (FILE * fpasm, int exp_es_variable, int etiqueta) {
    // Leer expresion pila
    fprintf(fpasm, "\tpop eax\n");
    if (exp_es_variable)
        fprintf(fpasm, "\tmov eax, [eax]\n");

    // Hacer comparacion y saltar
    fprintf(fpasm, "\tcmp eax, 0\n");
    fprintf(fpasm, "\tje dowhile_fin_%d\n", etiqueta);
    fprintf(fpasm, "\tjmp dowhile_inicio_%d\n", etiqueta);
}

/*
Generación de código para el final de una estructura dowhile
Como es el fin de uno bloque de control de flujo de programa que hace uso de la etiqueta
del mismo se requiere que antes de su invocación tome el valor de la etiqueta que le toca
según se ha explicado
Y tras ser invocada debe realizar el proceso para ajustar la información de las etiquetas
puesto que se ha liberado la última de ellas.
*/
void dowhile_fin( FILE * fpasm, int etiqueta) {
    fprintf(fpasm, "\ndowhile_fin_%d:\n", etiqueta);
}

/*
Generación de código para el inicio de una estructura while
Como es el inicio de uno bloque de control de flujo de programa que requiere de una nueva
etiqueta deben ejecutarse antes las tareas correspondientes a esta situación
exp_es_variable
Es 1 si la expresión de la condición es algo asimilable a una variable (identificador,
elemento de vector)
Es 0 en caso contrario (constante u otro tipo de expresión)
*/
void while_inicio(FILE * fpasm, int etiqueta) {
    fprintf(fpasm, "\nwhile_inicio_%d:\n", etiqueta);
}

/*
Generación de código para el momento en el que se ha generado el código de la expresión
de control del bucle
Sólo necesita usar la etiqueta adecuada, por lo que sólo se necesita que se recupere el valor
de la etiqueta que corresponde al momento actual.
exp_es_variable
Es 1 si la expresión de la condición es algo asimilable a una variable (identificador,
o elemento de vector)
Es 0 en caso contrario (constante u otro tipo de expresión)
*/
void while_exp_pila (FILE * fpasm, int exp_es_variable, int etiqueta) {
    // Leer expresion pila
    fprintf(fpasm, "\tpop eax\n");
    if (exp_es_variable)
        fprintf(fpasm, "\tmov eax, [eax]\n");

    // Hacer comparacion y saltar
    fprintf(fpasm, "\tcmp eax, 0\n");
    fprintf(fpasm, "\tje while_fin_%d\n", etiqueta);
    fprintf(fpasm, "\tjmp while_inicio_%d\n", etiqueta);
}

/*
Generación de código para el final de una estructura while
Como es el fin de uno bloque de control de flujo de programa que hace uso de la etiqueta
del mismo se requiere que antes de su invocación tome el valor de la etiqueta que le toca
según se ha explicado
Y tras ser invocada debe realizar el proceso para ajustar la información de las etiquetas
puesto que se ha liberado la última de ellas.
*/
void while_fin( FILE * fpasm, int etiqueta) {
    fprintf(fpasm, "\nwhile_fin_%d:\n", etiqueta);
}


/* ASIGNACION DE VECTORES */

/*
Generación de código para indexar un vector
Cuyo nombre es nombre_vector
Declarado con un tamaño tam_max
La expresión que lo indexa está en la cima de la pila
Puede ser una variable (o algo equivalente) en cuyo caso exp_es_direccion vale 1
Puede ser un valor concreto (en ese caso exp_es_direccion vale 0)
Según se especifica en el material, es suficiente con utilizar dos registros para realizar esta
tarea.
Stack:
Valor a introducir
donde introducir
*/
void escribir_elemento_vector(FILE * fpasm,char * nombre_vector, int tam_max, int exp_es_direccion){
    fprintf(fpasm, "\tpop dowrd eax\n"); //eax = m
    fprintf(fpasm, "\tpop dword ebx\n"); //ebx = dir. del elemento o elemento
    if(exp_es_direccion){
        fprintf(fpasm,"\tmov dword ebx, [ebx]\n"); //ebx = valor
    }
    fprintf(fpasm, "\tmov dword _%s[eax], ebx");
}


/* DECLARACION DE FUNCIONES */

/*
Generación de código para iniciar la declaración de una función.
Es necesario proporcionar
Su nombre
Su número de variables locales
*/
void declararFuncion(FILE * fpasm, char * nombre_funcion, int num_var_loc){
    fprintf(fpasm, "\n_%s_funcion:\n");
    fprintf(fpasm, "\tpush ebp\n");
    fprintf(fpasm, "\tmov ebp, esp\n");
    fprintf(fpasm, "\tsub esp, %d\n", 4*num_var_loc);
}

/*
Generación de código para el retorno de una función.
La expresión que se retorna está en la cima de la pila.
Puede ser una variable (o algo equivalente) en cuyo caso exp_es_direccion vale 1
Puede ser un valor concreto (en ese caso exp_es_direccion vale 0)
*/
void retornarFuncion(FILE * fpasm, int es_variable){
    fprintf(fpasm, "\tpop eax\n");
    if(es_variable){
        fprintf(fpasm, "\tmov dword eax, [eax]\n");
    }
    fprintf(fpasm, "\tmov esp, ebp\n");
    fprintf(fpasm, "\tpop ebp\n");
    fprintf(fpasm, "\tret\n");
}

/*
Función para dejar en la cima de la pila la dirección efectiva del parámetro que ocupa la
posición pos_parametro (recuerda que los parámetros se ordenan con origen 0) de un total
de num_total_parametros
*/
void escribirParametro(FILE* fpasm, int pos_parametro, int num_total_parametros){
    fprintf(fpasm, "\tlea eax, [ebp]%d", 4*(1 + (num_total_parametros - pos_parametro)));
    fprintf(fpasm, "\tpush dword eax\n");
}

/*
Función para dejar en la cima de la pila la dirección efectiva de la variable local que ocupa
la posición posicion_variable_local (recuerda que ordenadas con origen 1)
*/
void escribirVariableLocal(FILE* fpasm, int posicion_variable_local){
    fprintf(fpasm, "\tmov ebx, ebp\n");
    fprintf(fpasm, "\tsub ebx, %d\n", 4*posicion_variable_local);
    fprintf(fpasm, "\tlea eax, [ebx]\n");
    fprintf(fpasm, "\tpush dword eax\n");
}

/*
Función para poder asignar a un destino que no es una variable “global” (tipo _x) por
ejemplo parámetros o variables locales (ya que en ese caso su nombre real de alto nivel, no
se tiene en cuenta pues es realmente un desplazamiento a partir de ebp: ebp+4 o ebp-8 por
ejemplo).
Se debe asumir que en la pila estará
Primero (en la cima) lo que hay que asignar
Debajo (se ha introducido en la pila antes) la dirección donde hay que asignar
es_variable
Es 1 si la expresión que se va a asignar es algo asimilable a una variable
(identificador, o elemento de vector)
Es 0 en caso contrario (constante u otro tipo de expresión)
*/
void asignarDestinoEnPila(FILE* fpasm, int es_variable) {
    // Leemos direccion donde hay que asignar
    fprintf(fpasm, "\tpop eax\n");

    // Leemos valor a asignar
    fprintf(fpasm, "\tpop ebx\n");
    if (es_variable)
        fprintf(fpasm, "\tmov ebx, [ebx]\n");

    // Asignamos el valor
    fprintf(fpasm, "\tmov [eax], ebx\n");    
}

/*
Como habrás visto en el material, nuestro convenio de llamadas a las funciones asume que
los argumentos se pasan por valor, esto significa que siempre se dejan en la pila “valores” y
no “variables”
Esta función realiza la tarea de dado un operando escrito en la pila y sabiendo si es variable
o no (es_variable) se deja en la pila el valor correspondiente
*/
void operandoEnPilaAArgumento(FILE * fasm, int es_variable) {
    if(es_variable) {
        fprintf(fasm, "\tpop eax\n");
        fprintf(fasm, "\tmov eax, [eax]\n");
        fprintf(fasm, "\tpush eax");
    }
}

/*
Esta función genera código para llamar a la función nombre_funcion asumiendo que los
argumentos están en la pila en el orden fijado en el material de la asignatura.
Debe dejar en la cima de la pila el retorno de la función tras haberla limpiado de sus
argumentos
Para limpiar la pila puede utilizar la función de nombre limpiarPila
*/
void llamarFuncion(FILE * fasm, char * nombre_funcion, int num_argumentos) {
    fprintf(fasm, "\tpush dword _%s\n", nombre_funcion);
    fprintf(fasm, "\tcall %s\n", nombre_funcion);
    limpiarPila(fasm, num_argumentos);
}

/*
Genera código para limpiar la pila tras invocar una función
Esta función es necesaria para completar la llamada a métodos, su gestión dificulta el
conocimiento por parte de la función de llamada del número de argumentos que hay en la
pila
*/
void limpiarPila(FILE * fasm, int num_argumentos) {
    fprintf(fasm, "\tadd esp, %d\n", 4*num_argumentos);
}


