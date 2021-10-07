#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generacion.h"

int main (int argc, char ** argv)
{
    int etiqueta = 0;
    int getiqueta = 0;
    int etiquetas[MAX_ETIQUETAS];
    int cima_etiquetas=-1;
    FILE * fasm;

    if (argc != 2) {
        fprintf (stdout, "ERROR POCOS ARGUMENTOS\n"); 
        return -1;
    }

    fasm = fopen(argv[1],"w");

    escribir_subseccion_data(fasm);
    escribir_cabecera_bss(fasm);

    //int m;
    declarar_variable(fasm,"m", 1, 1);

    //int [4] v;
    declarar_variable(fasm, "v", 1, 4);
    escribir_segmento_codigo(fasm);
    escribir_inicio_main(fasm);

    //m=0;
    escribir_operando(fasm,"0",0);
    asignar(fasm,"m",0);

    //While. Gestion inicial de las etiquetas, guardado de etiqueta.
    getiqueta++;
    cima_etiquetas++;
    etiquetas[cima_etiquetas]=getiqueta;

    //Inicio del while. Impresion de la etiqueta.
    etiqueta = etiquetas[cima_etiquetas];
    while_inicio(fasm, etiqueta);

    //Condicion del bucle while.
    escribir_operando(fasm,"m",1);
    escribir_operando(fasm,"4",0);
    menor_igual(fasm,1,0,etiqueta);

    //Recuperamos la etiqueta para imprimir la comparacion del while.
    etiqueta = etiquetas[cima_etiquetas];
    while_exp_pila(fasm, 0, etiqueta);

    //printf m
    escribir_operando(fasm,"m",1);
    escribir(fasm,1,ENTERO);

    //v[m] = m*10;
    escribir_operando(fasm,"m",1);
    escribir_operando(fasm,"10",0);
    multiplicar(fasm,1,0);
    escribir_operando(fasm,"m",1);
    escribir_elemento_vector(fasm,"v", 4, 1);
    asignarDestinoEnPila(fasm,0);

    //m = m + 1
    escribir_operando(fasm,"m",1);
    escribir_operando(fasm,"1",0);
    sumar(fasm,1,0);
    asignar(fasm,"m",0);

    //Recuperamos la etiqueta para imprimir el fin de etiqueta del while.
    etiqueta = etiquetas[cima_etiquetas];
    while_fin(fasm, etiqueta);

    //Al cerrar el ámbito, decrementamos el contador de cima de etiquetas.
    cima_etiquetas--;

    escribir_fin(fasm);
    
    fclose(fasm);
    return 0;
}