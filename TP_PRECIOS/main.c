#include <stdio.h>
#include <stdlib.h>
#include "Funciones.h"

int cmpMesyNumForm(const void *e1, const void *e2);
int cmpDatos(const void *e1, const void *e2);
int cmpEsp(const void *e1, const void *e2);
int especificacionesTxtABin(char *linea, void *reg);
int datosTxtABin(char *linea, void *reg);
bool errorFatal(int cod);

const int punto5[] = {2141101, 2213103, 2223103, 3211306, 3222105};

int main(int argc, char *argv[])
{
    //---------CREACION DE VECTORES PARA LA REALIZACION DEL TP
    Vector vecDatos, vecEsp;
    vectorCrear(&vecDatos, sizeof(Datos));
    vectorCrear(&vecEsp, sizeof(Especificaciones));

    //---------MANEJO DE ARCHIVOS TXT
    archivoTxtABin(argv[1], "Datos.dat", sizeof(Datos), datosTxtABin, errorFatal, &vecDatos);
    archivoTxtABin(argv[2], "Especificaciones.dat", sizeof(Especificaciones), especificacionesTxtABin, errorFatal, &vecEsp);

    //---------ORDENAMIENTOS
    vectorOrdenarSeleccion(&vecDatos, cmpDatos);
    vectorOrdenarInsercion(&vecEsp, cmpEsp);

    //---------MUESTREO DE VECTORES
    vectorMostrarDatos(&vecDatos);
    vectorMostrarEsp(&vecEsp);

    //---------CALCULOS
    crearArchSinPrecio(&vecDatos, &vecEsp);
    {
        printf("\nARCHIVO SIN PRECIOS\n\n");
        mostrarArchEsp("sinprecios.dat");
    }
    calcularPromedio(&vecDatos, &vecEsp, punto5, cmpDatos);
    calcularMediaGeometrica(&vecDatos, &vecEsp, cmpDatos);
    calcularVarianzayDesvio(&vecDatos, cmpMesyNumForm);

    //---------
    vectorEliminar(&vecDatos);
    vectorEliminar(&vecEsp);
    return 0;
}

int cmpDatos(const void *e1, const void *e2)
{
    Datos *dato1 = (Datos *)e1;
    Datos *dato2 = (Datos *)e2;

    if (dato1->codProd == dato2->codProd)
        return dato1->codEmpresa - dato2->codEmpresa;
    else
        return dato1->codProd - dato2->codProd;
}

int cmpMesyNumForm(const void *e1, const void *e2)
{
    Datos *dato1 = (Datos *)e1;
    Datos *dato2 = (Datos *)e2;

    if (dato1->mes != dato2->mes)
    {
        return dato1->mes - dato2->mes;
    }

    return dato1->numForm - dato2->numForm;
}

int cmpEsp(const void *e1, const void *e2)
{
    Especificaciones *dato1 = (Especificaciones *)e1;
    Especificaciones *dato2 = (Especificaciones *)e2;
    int res = (dato1->codProd) - (dato2->codProd);
    return (res);
}

bool errorFatal(const int cod)
{
    switch (cod)
    {
    case ERR_LINEA_LARGA:
        return true;
    }

    return false;
}

int datosTxtABin(char *linea, void *reg)
{
    char codAux[TAM_COD];
    Datos *datos = reg;
    char *act = strchr(linea, '\n');
    if (!act)
        return ERR_LINEA_LARGA;

    *act = '\0';
    act = strrchr(linea, '|');
    sscanf(act + 1, "%d", &datos->numForm);
    *act = '\0';

    act = strrchr(linea, '|');
    strncpy(codAux, act + 1, TAM_COD);
    int i = 0;
    int flag = 0;
    while (flag != 1 && codAux[i] != '\0')
    {
        if (codAux[i] == ',')
        {
            codAux[i] = '.';
            datos->precio = strtof(codAux, NULL);
            flag = 1;
        }
        i++;
    }

    if (flag == 0)
        datos->precio = atoi(codAux);
    *act = '\0';

    act = strrchr(linea, '"');
    *act = '\0';
    act = strrchr(linea, '"');
    strncpy(codAux, act + 1, TAM_COD);
    datos->codProd = atoi(codAux);
    *act = '\0';

    act = strrchr(linea, '"');
    *act = '\0';
    act = strrchr(linea, '"');
    strncpy(codAux, act + 1, TAM_COD);
    datos->codEmpresa = atoi(codAux);
    *act = '\0';

    act = strrchr(linea, '|');
    *act = '\0';
    act = strrchr(linea, '|');
    sscanf(act + 1, "%d", &datos->mes);
    *act = '\0';

    sscanf(linea, "%d", &datos->anio);

    return TODO_OK;
}

int especificacionesTxtABin(char *linea, void *reg)
{
    char codAux[TAM_COD];
    char *act = strchr(linea, '\n');
    Especificaciones *esp = reg;
    if (!act)
        return ERR_LINEA_LARGA;

    *act = '\0';
    act = strrchr(linea, '"');
    *act = '\0';
    act = strrchr(linea, '|');
    strncpy(esp->especifProd, act + 2, TAM_ESP);
    esp->especifProd[TAM_ESP] = '\0';
    *act = '\0';
    normalizarCadena(esp->especifProd);

    act = strrchr(linea, '"');
    *act = '\0';
    act = strrchr(linea, '|');
    strncpy(esp->nombreProd, act + 2, TAM_NOM);
    esp->nombreProd[TAM_NOM - 1] = '\0';
    *act = '\0';
    normalizarCadena(esp->nombreProd);

    act = strrchr(linea, '"');
    *act = '\0';
    act = strrchr(linea, '"');
    strncpy(codAux, act + 1, TAM_COD);
    esp->codProd = atoi(codAux);

    return TODO_OK;
}