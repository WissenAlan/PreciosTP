#include <stdio.h>
#include <stdlib.h>
#include "Funciones.h"

int cmpDatosP6(const void* e1, const void* e2);
int cmpDatos(const void* e1, const void* e2);
int cmpEsp(const void *e1, const void *e2);
const int punto5[] = {2141101, 2213103, 2223103 ,3211306, 3222105};

int main(int argc,char* argv[])
{
    Vector vecDatos, vecEsp;

    vectorCrear(&vecDatos,sizeof(Datos));
    vectorCrear(&vecEsp,sizeof(Especificaciones));
    ///////////////////////////////////////////////
    pasarDatosTxtABin(argv[1],&vecDatos);
    pasarEspecificacionesTxtABin(argv[2],&vecEsp);
    /////////ORDENAMIENTOS/////////////////////
   // vectorOrdenarSeleccion(&vecDatos, cmpDatos);
   // vectorOrdenarInsercion(&vecEsp,cmpEsp);
    ///////////////////////////////////////////
    //////////////////////////////////////////////
   // vectorMostrarDatos(&vecDatos);
    //vectorMostrarEsp(&vecEsp);
    /////Ordenado por mes y nro de formulario/////
    vectorOrdenarSeleccion(&vecDatos, cmpDatosP6);
    vectorMostrarDatos(&vecDatos);
    CalcularVarianzayDesvio(&vecDatos);
    //calcularPromedio(&vecDatos,&vecEsp, punto5);
    //mostrarPunto5();
    //vectorOrdenarSeleccion(&vecDatos, cmpDatos);
    calcularMedidaGeometrica(&vecDatos,&vecEsp);
   // crearArchSinPrecio(&vecDatos,&vecEsp);
    //printf("\nARCHIVO SIN PRECIOS\n\n");
   // mostrarArchEsp("sinprecios.dat");
    //MostrarArchDatos("Datos.dat");
    //mostrarArchEsp("especificaciones.dat");
    return 0;
}

int cmpDatos(const void* e1, const void* e2){
    Datos* dato1 = (Datos*)e1;
    Datos* dato2 = (Datos*)e2;

    if(dato1->codProd == dato2->codProd)
        return dato1->codEmpresa-dato2->codEmpresa;
    else
        return dato1->codProd - dato2->codProd;
}

int cmpDatosP6(const void* e1, const void* e2){
    Datos* dato1 = (Datos*)e1;
    Datos* dato2 = (Datos*)e2;
    return dato1->mes-dato2->mes;
}

int cmpEsp(const void*e1, const void*e2)
{
    Especificaciones* dato1 = (Especificaciones*)e1;
    Especificaciones* dato2 = (Especificaciones*)e2;
    int res=(dato1->codProd) - (dato2->codProd);
    return(res);
}
