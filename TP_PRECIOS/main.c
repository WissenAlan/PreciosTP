#include <stdio.h>
#include <stdlib.h>
#include "Funciones.h"

int cmpDatosP6(const void* e1, const void* e2);
int cmpDatos(const void* e1, const void* e2);
int cmpEsp(const void *e1, const void *e2);
bool errorFatal(int cod);
const int punto5[] = {2141101, 2213103, 2223103 ,3211306, 3222105};

int main(int argc,char* argv[])
{
    Vector vecDatos, vecEsp;

    vectorCrear(&vecDatos,sizeof(Datos));
    vectorCrear(&vecEsp,sizeof(Especificaciones));
    ///////////////////////////////////////////////
    pasarDatosTxtABin(argv[1],&vecDatos, errorFatal);
    pasarEspecificacionesTxtABin(argv[2],&vecEsp, errorFatal);
    /////////ORDENAMIENTOS/////////////////////
    //mostrarArchDatos("Datos.dat");
    //mostrarArchEsp("especificaciones.dat");
    vectorOrdenarSeleccion(&vecDatos, cmpDatos);
    vectorOrdenarInsercion(&vecEsp,cmpEsp);
//    vectorMostrarEsp(&vecEsp);
    ///////////////////////////////////////////
    //////////////////////////////////////////////
    //vectorMostrarDatos(&vecDatos);
//    crearArchSinPrecio(&vecDatos,&vecEsp);
//    printf("\nARCHIVO SIN PRECIOS\n\n");
//    mostrarArchEsp("sinprecios.dat");
    calcularPromedio(&vecDatos,&vecEsp, punto5);
    mostrarPunto5();
    calcularMedidaGeometrica(&vecDatos,&vecEsp);
    /////Ordenado por mes y nro de formulario/////
//    vectorOrdenarSeleccion(&vecDatos, cmpDatosP6);
//    calcularVarianzayDesvio(&vecDatos);
    vectorEliminar(&vecDatos);
    vectorEliminar(&vecEsp);
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

bool errorFatal(int cod)
{
    switch(cod)
    {
    case ERR_LINEA_LARGA:
        return true;
    }

    return false;
}
