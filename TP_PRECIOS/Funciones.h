#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED
#define ERR_MEM 2
#define ERROR_ARCH 1
#define TODO_OK 0
#define ERR_LINEA_LARGA 3
#define TAM_LINEA 200
#define TAM_COD 7
#define TAM_ESP 251
#define TAM_NOM 53
#define ES_MINUS(X) ((X) >= 'a' && (X) <= 'z') ? true : false
#define ES_MAYUS(X) ((X) >= 'A' && (X) <= 'Z') ? true : false
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define CAP_INI 100
#define FACTOR_INCR 50
#define SIN_MEM 6
#include <math.h>

typedef int (*Cmp)(const void *e1, const void *e2);

typedef int (*TxtABin)(char *linea, void *reg);
typedef bool (*ErrorFatal)(const int cod);

typedef struct
{
    int anio;
    int mes;
    int codEmpresa;
    int codProd;
    float precio;
    int numForm;
} Datos;

typedef struct
{
    int codProd;
    char nombreProd[TAM_NOM];
    char especifProd[TAM_ESP];
} Especificaciones;

typedef struct
{
    void *vec;
    size_t ce;
    size_t cap;
    size_t tamElem;
} Vector;

int archivoTxtABin(const char *nomArchTxt, const char *nomArchBin, size_t tamReg, TxtABin txtABin, ErrorFatal errorFatal, Vector *vec);

int mostrarArchDatos(const char *NombreArchBin);
void mostrarArchEsp(const char *NombreArch);

int crearArchSinPrecio(const Vector *, const Vector *);
int buscarSinPrecio(const void *, const Vector *);
void normalizarCadena(char *cad);
int calcularPromedio(const Vector *, const Vector *, const int[], Cmp cmpDatos);
int calcularVarianzayDesvio(const Vector *, Cmp cmpMesyNumForm);
void calcularMediaGeometrica(const Vector *vecDatos, const Vector *vecEsp, Cmp cmpDatos);
float calcularPromedio2(int mesact, int formact, Datos *ini);

int vectorInsertar(Vector *Vec, void *elem);
bool vectorCrear(Vector *vec, size_t tamanioElemento);
void vectorEliminar(Vector *vec);
void vectorMostrar(const Vector *);
void vectorOrdenarSeleccion(const Vector *, const Cmp);
void intercambiar(void *ant, void *sig, const size_t);
void vectorOrdenarInsercion(const Vector *vec, const Cmp);
void vectorMostrarEsp(const Vector *);
void vectorMostrarDatos(const Vector *);
void buscarNombre(const Vector *vec, const int codBuscar, char *nombreExp);
int BuscarSiYaExistia(int dato, Datos *limInf, Datos *limSup);
Datos *BuscarNuevoMes(Datos *ini, Datos *u, int mesact, Vector *vecNF);
void vectorVaciar(Vector *v);
bool redimensionarVector(Vector *v, size_t nuevaCap);

#endif // FUNCIONES_H_INCLUDED
