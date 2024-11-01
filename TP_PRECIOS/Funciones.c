#include "Funciones.h"

void *_buscarMenor(void *ini, const void *fin, const Cmp cmp, const size_t size);
int _mostrarPunto5();

int archivoTxtABin(const char *nomArchTxt, const char *nomArchBin, size_t tamReg, TxtABin txtABin, ErrorFatal errorFatal, Vector *vec)
{
    FILE *archBin = fopen(nomArchBin, "wb");

    if (!archBin)
    {
        return ERROR_ARCH;
    }

    FILE *archTxt = fopen(nomArchTxt, "rt");

    if (!archTxt)
    {
        fclose(archBin);
        return ERROR_ARCH;
    }

    void *reg = malloc(tamReg);

    if (!reg)
    {
        fclose(archBin);
        fclose(archTxt);
        return ERR_MEM;
    }

    char linea[TAM_LINEA];
    int ret = TODO_OK;

    while (fgets(linea, TAM_LINEA, archTxt) && !errorFatal(ret))
    {
        ret = txtABin(linea, reg);

        if (ret == TODO_OK)
        {
            vectorInsertar(vec, reg);
            fwrite(reg, tamReg, 1, archBin);
        }
    }

    fclose(archBin);
    fclose(archTxt);
    free(reg);

    return ret;
}

int mostrarArchDatos(const char *nombreArchBin)
{
    FILE *pf = fopen(nombreArchBin, "rb");
    if (!pf)
        return ERROR_ARCH;

    Datos datos;

    fread(&datos, sizeof(Datos), 1, pf);
    while (!feof(pf))
    {
        printf("%4d\t%1d\t%d\t%d\t%8.2f\t%1d\n", datos.anio, datos.mes, datos.codEmpresa, datos.codProd, datos.precio, datos.numForm);
        fread(&datos, sizeof(Datos), 1, pf);
    }

    fclose(pf);
    return TODO_OK;
}

void mostrarArchEsp(const char *nombreArch)
{
    FILE *pf = fopen(nombreArch, "rb");
    if (!pf)
        printf("Error al abrir el archivo");

    Especificaciones esp;

    fread(&esp, sizeof(Especificaciones), 1, pf);
    while (!feof(pf))
    {
        printf("%-10d\t%-30s\t%-40s\n", esp.codProd, esp.nombreProd, esp.especifProd);
        fread(&esp, sizeof(Especificaciones), 1, pf);
    }
    fclose(pf);
}

bool vectorCrear(Vector *vec, size_t tamanioElemento)
{
    vec->vec = malloc(CAP_INI * tamanioElemento);

    if (vec->vec == NULL)
    {
        printf("\n[ERROR] No se pudo crear el vector");
        return false;
    }

    vec->ce = 0;
    vec->cap = CAP_INI;
    vec->tamElem = tamanioElemento;

    return true;
}

int vectorInsertar(Vector *vec, void *elem)
{
    if (vec->ce == vec->cap)
    {

        size_t nCap = vec->cap * FACTOR_INCR;
        void *nVec = realloc(vec->vec, nCap * vec->tamElem);

        if (!nVec)
            return SIN_MEM;

        vec->vec = nVec;
        vec->cap = nCap;
    }

    void *nuevaPosicion = vec->vec + (vec->ce * vec->tamElem);

    memcpy(nuevaPosicion, elem, vec->tamElem);

    vec->ce++;

    return TODO_OK;
}

void vectorEliminar(Vector *vec)
{
    free(vec->vec);
    vec->vec = NULL;
    vec->ce = 0;
    vec->cap = 0;
    vec->tamElem = 0;
}

void vectorVaciar(Vector *v)
{
    v->ce = 0;
    redimensionarVector(v, CAP_INI);
}

bool redimensionarVector(Vector *v, size_t nuevaCap)
{
    void *nVec = realloc(v->vec, nuevaCap * v->tamElem);

    if (!nVec)
    {
        return false;
    }

    v->vec = nVec;

    v->cap = nuevaCap;
    return true;
}

void vectorMostrarDatos(const Vector *vector)
{
    const Datos *ult = vector->vec + vector->ce * vector->tamElem;
    printf("Anio\tMes\tCodEmpresa\tCodProducto\tPrecio\tNumeroFormulario\n");
    for (Datos *i = vector->vec; i < ult; i++)
        printf("%d\t%d\t%-10d\t%-10d\t%-10g\t%-10d\n", i->anio, i->mes, i->codEmpresa, i->codProd, i->precio, i->numForm);
    putchar('\n');
}

void vectorMostrarEsp(const Vector *vector)
{
    const Especificaciones *ult = vector->vec + (vector->ce * vector->tamElem);
    for (Especificaciones *i = vector->vec; i < ult; i++)
        printf("%d\t%s\t%s\n", i->codProd, i->nombreProd, i->especifProd);
}
// PUNTO 1
void vectorOrdenarSeleccion(const Vector *vector, const Cmp cmp)
{
    const void *ult = vector->vec + (vector->ce * vector->tamElem) - vector->tamElem;
    void *m = malloc(vector->tamElem);

    for (void *i = vector->vec; i < ult; i += vector->tamElem)
        memcpy(m, _buscarMenor(i, ult, cmp, vector->tamElem), vector->tamElem);

    free(m);
}
void *_buscarMenor(void *ini, const void *fin, const Cmp cmp, const size_t size)
{
    void *men = ini;
    for (void *j = ini + size; j <= fin; j += size)
        if (cmp(j, men) < 0)
            intercambiar(men, j, size);

    return men;
}
void intercambiar(void *ant, void *sig, const size_t size)
{
    void *aTemp = malloc(size);
    memcpy(aTemp, ant, size);
    memcpy(ant, sig, size);
    memcpy(sig, aTemp, size);
    free(aTemp);
}
// PUNTO 2
void vectorOrdenarInsercion(const Vector *vector, const Cmp cmpEsp)
{
    const void *ult = vector->vec + (vector->ce - 1) * vector->tamElem;
    void *elemAIns = malloc(vector->tamElem);
    void *i, *j;

    for (i = vector->vec + vector->tamElem; i <= ult; i += vector->tamElem)
    {
        j = i - vector->tamElem;
        memcpy(elemAIns, i, vector->tamElem);

        while (j >= vector->vec && 0 > cmpEsp(elemAIns, j))
        {
            memcpy(j + vector->tamElem, j, vector->tamElem);
            j -= vector->tamElem;
        }
        memcpy(j + vector->tamElem, elemAIns, vector->tamElem);
    }
    free(elemAIns);
}
// PUNTO 3
void normalizarCadena(char *cad)
{
    int flag = 0;
    while (*cad)
    {
        if (flag == 0)
        {
            flag = 1;
            if (ES_MINUS(*cad))
                *cad -= ('a' - 'A');
        }

        else if (ES_MAYUS(*cad))
            *cad += ('a' - 'A');

        else if (*cad == '.' && *(cad + 1) == ' ')
        {
            flag = 0;
            cad++;
        }
        cad++;
    }
}
// PUNTO 4
int crearArchSinPrecio(const Vector *vecDatos, const Vector *vecEsp)
{
    FILE *pfsinprecio = fopen("sinprecios.dat", "wb");
    if (!pfsinprecio)
        return ERROR_ARCH;

    Especificaciones *esp, *ultEsp;
    esp = vecEsp->vec;
    ultEsp = esp + vecEsp->ce;

    while (esp < ultEsp)
    {
        if (buscarSinPrecio(esp, vecDatos) == 0)                   ////BUSCA SI EL PRODUCTO DE LA LINEA QUE SE LEE TIENE EL PRECIO EN EL ARCHIVO DE DATOS
            fwrite(esp, sizeof(Especificaciones), 1, pfsinprecio); ////ESCRIBE EN EL ARCHIVO DE SIN PRECIO LAS ESPECIFICACIONES DEL PRODUCTO
        esp++;
    }
    fclose(pfsinprecio);
    return TODO_OK;
}
int buscarSinPrecio(const void *i, const Vector *vector)
{
    Datos *vecD = (Datos *)vector->vec;
    const Datos *ultDat = vecD + (vector->ce);
    const Especificaciones *esp = i;

    while (vecD->codProd <= esp->codProd && vecD < ultDat) // Como vecD ya esta ordenado por codProd, si es mayor al buscado no deberia encontrarlo
    {
        if (esp->codProd == vecD->codProd)
            return 1; ////EL PRODUCTO TIENE PRECIO
        else
            vecD++;
    }
    return TODO_OK; ////NO ENCONTRÓ EL PRODUCTO, NO TIENE PRECIO
}
// PUNTO 5
int calcularPromedio(const Vector *vecDatos, const Vector *vecEsp, const int vecPunto5[], Cmp cmpDatos)
{
    FILE *pf = fopen("Punto5.txt", "wt");
    if (!pf)
        return ERROR_ARCH;

    vectorOrdenarSeleccion(vecDatos, cmpDatos);
    int indice = 0;
    float matriz[12][2] = {0};
    bool flag = 0;
    char nombreProd[TAM_NOM];

    const Datos *ult = vecDatos->vec + vecDatos->ce * vecDatos->tamElem - vecDatos->tamElem;
    for (Datos *i = vecDatos->vec; i <= ult; i++) // Recorro todo el vector
    {
        if (i->codProd == vecPunto5[indice])
        {
            matriz[i->mes - 1][0] += i->precio;
            matriz[i->mes - 1][1]++;
            flag = 1;
        }
        if ((i->codProd != vecPunto5[indice] || i + 1 > ult) && flag) // Si no coinciden los codigos, o si no hay proximo en el vector
        {
            if (i + 1 <= ult)
                i--; // Vuelve uno atras para hacer la impresion por pantalla
            buscarNombre(vecEsp, i->codProd, nombreProd);
            flag = 0;                    // Habilita la busqueda de nombre
            indice++;                    // Aumenta el indice para la proxima vuelta
            for (int j = 0; j < 12; j++) // Hasta que j sea menor a 12 por los 12 meses del año
            {
                if (matriz[j][1] > 0) // Imprime solo si hay algo en la matriz
                {
                    matriz[j][0] = matriz[j][0] / matriz[j][1];                                                 // Calculo del promedio
                    fprintf(pf, "%d|%d|%g|%g|%s\n", j + 1, i->codProd, matriz[j][0], matriz[j][1], nombreProd); // Carga la linea al archivo
                    // Resetea la posicion
                    matriz[j][0] = 0;
                    matriz[j][1] = 0;
                }
            }
        }
    }
    fclose(pf);
    _mostrarPunto5(); // muestro el archivo
    return TODO_OK;
}
int _mostrarPunto5()
{
    FILE *pf = fopen("Punto5.txt", "rt");
    if (!pf)
        return ERROR_ARCH;

    char linea[TAM_LINEA];

    fgets(linea, TAM_LINEA, pf);
    while (!feof(pf))
    {
        printf("%s", linea);
        fgets(linea, TAM_LINEA, pf);
    }
    fclose(pf);
    return TODO_OK;
}
// PUNTO 6
int calcularVarianzayDesvio(const Vector *vec, Cmp cmpMesyNumForm)
{
    vectorOrdenarSeleccion(vec, cmpMesyNumForm); // Ordena por mes y numero de formulario
    Datos *ini, *u, *i, *limsup;
    ini = (Datos *)vec->vec;
    u = ini + vec->ce;
    float sumacuadr = 0, prom = 0, varianzaMes = 0, desv;
    int mesact = ini->mes, cont = 0, k, *numFormActual;

    Vector vecNF; // Vector que me va a almacenar todos los Numeros de formularios que posea cada mes, y la cant de NF
    vectorCrear(&vecNF, sizeof(int));

    while (ini < u)
    {
        // me devuelve el vector cargado y hasta donde tiene que recorrer(Cuando cambia de mes)
        limsup = BuscarNuevoMes(ini, u, mesact, &vecNF);
        numFormActual = vecNF.vec;

        ////CantNF = cantidad de veces que tiene que recorrer para calcular todos los NF de cada mes
        for (k = 0; k < vecNF.ce; k++)
        {
            sumacuadr = 0;
            cont = 0;
            i = ini;

            prom = calcularPromedio2(mesact, *numFormActual, i);

            while (i < limsup)
            {
                if (*(numFormActual) == i->numForm)
                {
                    sumacuadr += pow(i->precio - prom, 2);
                    cont++;
                }

                i++;
            }

            varianzaMes = sumacuadr / (cont);
            desv = sqrt(varianzaMes);
            printf("Mes:%-10d\tNumForm:%-10d\tVarianza:%.2f\t\tDesvio estandar:%.2f\n", mesact, *(numFormActual), varianzaMes, desv);
            numFormActual++;
        }
        ////Cuando cambio de mes, puedo tener distintos numeros de formularios////
        vectorVaciar(&vecNF);
        ini = limsup;
        mesact = ini->mes;
    }
    vectorEliminar(&vecNF);
    return TODO_OK;
}
Datos *BuscarNuevoMes(Datos *ini, Datos *u, int mesact, Vector *vecNF)
{
    Datos *i = ini;

    while (i < u && i->mes == mesact)
    {
        ////Me recorre hasta donde estoy parada, si el num de form ya existia antes devuelve un 1, si devuelve 0 es un nuevo NF///
        if (BuscarSiYaExistia(i->numForm, ini, i) == 0)
        {
            vectorInsertar(vecNF, &(i->numForm));
        }

        i++;
    }

    return i;
}
int BuscarSiYaExistia(int dato, Datos *limInf, Datos *limSup)
{
    int band = 0;
    Datos *i = limInf;

    while (i < limSup && band == 0)
    {
        if (i->numForm == dato)
        {
            band = 1;
        }
        i++;
    }

    return band;
}
float calcularPromedio2(int mesact, int formact, Datos *i)
{
    float sumaprecios = 0, cont = 0, promedio;

    while (mesact == i->mes)
    {
        if (formact == i->numForm)
        {
            sumaprecios += i->precio;

            cont++;
        }

        i++;
    }
    if (cont > 0)
    {
        promedio = sumaprecios / cont;
    }

    return promedio;
}
// PUNTO 7
void calcularMediaGeometrica(const Vector *vecDatos, const Vector *vecEsp, Cmp cmpDatos) // Recibe el vector de datos ordenado por codProd
{
    vectorOrdenarSeleccion(vecDatos, cmpDatos);
    const Datos *ult = vecDatos->vec + vecDatos->ce * vecDatos->tamElem - vecDatos->tamElem;
    char nombre[TAM_NOM];
    float acum = 0;
    int codAux = 0, cont = 0;
    printf("\nMedida geometrica: \n");
    for (Datos *i = vecDatos->vec; i <= ult; i++) // Recorro todo el vector
    {
        if (!codAux || codAux == i->codProd) // Si es la primera vuelta o si es el mismo codigo que el de la primera vuelta
        {
            if (!codAux)
                codAux = i->codProd;
            acum += log(i->precio); // sumo los logaritmos de los precios
            cont++;                 // sumo la cantidad de productos con el mismo codigo
        }
        if (codAux != i->codProd || i + 1 > ult) // Si no es el mismo codProd o si el siguiente al ultimo se pasa del vector
        {
            buscarNombre(vecEsp, codAux, nombre);                           // busco el nombre y lo copio en nombre
            printf("%-10d\t%-50s\t%g\n", codAux, nombre, exp(acum / cont)); // muestro por pantalla haciendo e ^ Σln(n) / n (formula de media geometrica)
            acum = 0;
            cont = 0;
            codAux = 0;
            if (i + 1 <= ult)
                i--;
        }
    }
}

void buscarNombre(const Vector *vec, const int codBuscar, char *nombreExp)
{
    Especificaciones *iEsp = vec->vec;
    Especificaciones *ultEsp = vec->vec + vec->ce * vec->tamElem;
    while (iEsp <= ultEsp)
    {
        if (codBuscar == iEsp->codProd)
        {
            strcpy(nombreExp, iEsp->nombreProd);
            return; // Cuando encuentra el nombre retorna 1
        }
        iEsp++;
    }
}