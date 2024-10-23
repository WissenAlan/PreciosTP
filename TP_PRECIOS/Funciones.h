#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED
#define ERROR_ARCH 1
#define TODO_OK 0
#define TAM_LINEA 200
#define TAM_COD 7
#define TAM_ESP 251
#define TAM_NOM 51
#define ES_MINUS(X) ((X) >= 'a' && (X) <= 'z') ? true : false
#define ES_MAYUS(X) ((X) >= 'A' && (X) <= 'Z') ? true : false
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define CAP_INI 100
#define FACTOR_INCR 50
#define SIN_MEM 6
#define TODO_OK 0

typedef int (*Cmp) (const void*e1, const void*e2);

typedef struct{
    int anio;
    int mes;
    int codEmpresa;
    int codProd;
    float precio;
    int numForm;
}Datos;

typedef struct{
    int codProd;
    char nombreProd[TAM_NOM];
    char especifProd[TAM_ESP];
}Especificaciones;

typedef struct
{
    void *vec;
    size_t ce;
    size_t cap;
    size_t tamElem;
}Vector;

int pasarDatosTxtABin(const char* NombreArch, Vector*); //Tambien carga el vector de datos
int pasarEspecificacionesTxtABin(const char* NombreArch, Vector*);
void leerLinea(Datos *datos,char* Linea);
void leerLineaEsp(Especificaciones *esp,char* Linea);
int mostrarArchDatos(const char* NombreArchBin);
void mostrarArchEsp(const char* NombreArch);

int crearArchSinPrecio(Vector *, Vector *);
int buscarSinPrecio(void*,Vector*);

void normalizarCadena(char* cad);
int calcularPromedio(const Vector*,const Vector*,const int[]);
int mostrarPunto5();

int vectorInsertar(Vector* Vec, void* elem);
bool vectorCrear(Vector *vec, size_t tamanioElemento);
void vectorMostrar(const Vector*);
void vectorOrdenarSeleccion(Vector*, Cmp);
void intercambiar(void* ant, void* sig, size_t );
void vectorMostrarEsp(const Vector*);
void vectorMostrarDatos(const Vector*);
void vectorOrdenarInsercion(Vector *vec,Cmp);


#endif // FUNCIONES_H_INCLUDED
