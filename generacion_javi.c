
void escribir_segmento_codigo(FILE* fpasm) {
    fprintf(fpasm, "\n");
    fprintf(fpasm, "segment .text\n");
    fprintf(fpasm, "\tglobal main\n");
    fprintf(fpasm, "\textern scan_int, print_int, scan_boolean, print_boolean\n");
    fprintf(fpasm, "\textern print_endofline, print_blank, print_string\n");
}
/*
Para escribir el comienzo del segmento .text, básicamente se indica que se
exporta la etiqueta main y que se usarán las funciones declaradas en la librería
alfalib.o
*/

void asignar(FILE* fpasm, char* nombre, int es_variable) {
    fprintf(fpasm, "\n");
    if (es_variable == 0) {
        fprintf(fpasm, "\tpop eax\n");
        fprintf(fpasm, "\tmov [_%s], eax\n", nombre);
    } else if ( es_variable == 1) {
        fprintf(fpasm, "\tpop eax\n");
        fprintf(fpasm, "\tmov eax, [eax]\n");
        fprintf(fpasm, "\tmov [_%s], eax\n", nombre);
    } else {
        // Error
    }
}
/*
- Genera el código para asignar valor a la variable de nombre nombre.
- Se toma el valor de la cima de la pila.
- El último argumento es el que indica si lo que hay en la cima de la pila es
una referencia (1) o ya un valor explícito (0).
*/

void descargar_pila(FILE* fpasm, int es_variable_1, int es_variable_2) {
    fprintf(fpasm, "\n");
    if (es_variable_2 == 0) {
        fprintf(fpasm, "\tpop ebx\n");
    } else if (es_variable_2 == 1) {
        fprintf(fpasm, "\tpop ebx\n");
        fprintf(fpasm, "\tmov ebx, [ebx]\n");
    } else {
        // Error
    }
    if (es_variable_1 == 0) {
        fprintf(fpasm, "\tpop eax\n");
    } else if (es_variable_1 == 1) {
        fprintf(fpasm, "\tpop eax\n");
        fprintf(fpasm, "\tmov eax, [eax]\n");
    } else {
        // Error
    }
}

/* FUNCIONES COMPARATIVAS */
void igual(FILE* fpasm, int es_variable1, int es_variable2, int etiqueta) {
    descargar_pila(fpasm, es_variable1, es_variable2);
    fprintf(fpasm, "\n");
    fprintf(fpasm, "\tcmp eax, ebx\n");
    fprintf(fpasm, "\tje IGUAL_%d\n", etiqueta);        // je - jump equal
    fprintf(fpasm, "\tpush dword 0\n");        
    fprintf(fpasm, "\tjmp NO_IGUAL_%s\n", etiqueta);
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
    fprintf(fpasm, "\tjmp NO_NOIGUAL_%s\n", etiqueta);
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
    fprintf(fpasm, "\tjmp NO_MENORIGUAL_%s\n", etiqueta);
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
    fprintf(fpasm, "\tjmp NO_MAYORIGUAL_%s\n", etiqueta);
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
    fprintf(fpasm, "\tjmp NO_MENOR_%s\n", etiqueta);
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
    fprintf(fpasm, "\tjmp NO_MAYOR_%s\n", etiqueta);
    fprintf(fpasm, "MAYOR_%d:\n", etiqueta);
    fprintf(fpasm, "\tpush dword 1\n");
    fprintf(fpasm, "NO_MAYOR_%d:\n", etiqueta);
}
/*
Todas estas funciones reciben como argumento si los elementos a comparar son o
no variables. El resultado de las operaciones, que siempre será un booleano (“1”
si se cumple la comparación y “0” si no se cumple), se deja en la pila como en el
resto de operaciones. Se deben usar etiquetas para poder gestionar los saltos
necesarios para implementar las comparaciones.
*/


