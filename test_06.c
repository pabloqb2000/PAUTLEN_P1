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
    escribir_segmento_codigo(fasm);
    escribir_inicio_main(fasm);

    //m=0;
    escribir_operando(fasm,"0",0);
    asignar(fasm,"m",0);

    //Gestion de etiquetas para abrir el ambito del if. Esta sera la etiqueta que tenga.
    getiqueta++;
    cima_etiquetas++;
    etiquetas[cima_etiquetas]=getiqueta;
    etiqueta = getiqueta;

    //Condición del if. Salto a fin si, si la condicion se da.
    escribir_operando(fasm,"m",1);
    escribir_operando(fasm,"5",0);
    mayor(fasm,1,0,etiqueta);
    ifthenelse_inicio(fasm, 0, etiqueta);

    //printf 2
    escribir_operando(fasm,"2",0);
    escribir(fasm,0,ENTERO);

    //Salto a fin sino al terminar el if, impresion de la etiqueta fin_si. Recogemos la etiqueta.
    etiqueta = etiquetas[cima_etiquetas];
    ifthenelse_fin_then(fasm, etiqueta);

    //printf 3
    escribir_operando(fasm,"3",0);
    escribir(fasm,0,ENTERO);

    //Fin del condicional if. Imprimimos la etiqueta de ambito del fin del condicional y restamos el contador.
    etiqueta = etiquetas[cima_etiquetas];
    ifthenelse_fin(fasm, etiqueta);
    cima_etiquetas--;

    escribir_fin(fasm);
    
    fclose(fasm);
    return 0;
}