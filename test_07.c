#include "generacion.h"

int main (int argc, char ** argv)
{
    int etiqueta = 0;
    int getiqueta = 0;
    int etiquetas[MAX_ETIQUETAS];
    int cima_etiquetas=-1;
    FILE * fasm;

    fasm = fopen(argv[1],"w");

    escribir_subseccion_data(fasm);
    escribir_cabecera_bss(fasm);

    //int z;
    declarar_variable(fasm,"z", 1, 1);
    escribir_segmento_codigo(fasm);

    //Declaramos la funcion. Vamos a imprimir su etiqueta y decir que tiene una variable local.
    //function int doble(int arg)
    //{
    // int auxArg;
    declararFuncion(fasm,"doble",1);

    //auxArg = arg; Asignacion de parametro a variable local. Solo hay un parametro.
    escribirParametro(fasm,0,1);
    escribirVariableLocal(fasm,1);
    asignarDestinoEnPila(fasm,1);

    //2*arg.
    escribir_operando(fasm,"2",0);
    escribirParametro(fasm,0,1);
    multiplicar(fasm,0,1);

    //Retornamos de la funcion con lo que esta encima de la pila.
    retornarFuncion(fasm,0);
    escribir_inicio_main(fasm);

    //z=2
    escribir_operando(fasm,"2",0);
    asignar(fasm,"z",0);
    escribir_operando(fasm,"z",1);

    // printf doble(z)
    //Llamamos a la funcion que tiene 1 argumento. Estamos dando un salto a la etiqueta. Primero apilamos el parametro.
    operandoEnPilaAArgumento(fasm,1);
    llamarFuncion(fasm,"doble",1);

    //Imprimimos el resultado de la funcion.
    escribir(fasm,0,ENTERO);

    escribir_fin(fasm);
    
    fclose(fasm);
    return 0;
}