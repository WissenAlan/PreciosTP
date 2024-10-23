#include "Funciones.h"

void* _buscarMenor(void*ini, void*fin, Cmp cmp, size_t size);

int pasarDatosTxtABin(const char* nombreArch, Vector* vec)
{
    char linea[TAM_LINEA];

    FILE* pf2 = fopen("Datos.dat","wb");
    if(!pf2)
        return ERROR_ARCH;

    FILE* pf = fopen(nombreArch,"rt");
    Datos datos;
    if(!pf)
        return ERROR_ARCH;

    fgets(linea,TAM_LINEA,pf);
    while(!feof(pf))
    {
        leerLinea(&datos,linea); ////Me carga y separa la linea///
        vectorInsertar(vec,&datos);////Inserto en el vector////
        fwrite(&datos,sizeof(Datos),1,pf2);
        fgets(linea,TAM_LINEA,pf);
    }

    fclose(pf);
    fclose(pf2);
    return TODO_OK;
}

void leerLinea(Datos *datos,char* linea)
{
    char codAux[TAM_COD];

    char *act = strchr(linea,'\n');
    *act = '\0';
    act = strrchr(linea,'|');
    sscanf(act + 1,"%d",&datos->numForm);
    *act = '\0';

    act = strrchr(linea,'|');
    strncpy(codAux,act + 1,TAM_COD);
    int i = 0;
    int flag = 0;
    while(flag != 1 && codAux[i] != '\0')  {
        if (codAux[i] == ',') {
            codAux[i] = '.';
            datos->precio = strtof(codAux, NULL);
            flag = 1;
        }
        i++;
    }
    if(flag == 0)
        datos->precio = atoi(codAux);
    *act = '\0';

    act = strrchr(linea,'"');
    *act = '\0';
    act = strrchr(linea,'"');
    strncpy(codAux,act + 1,TAM_COD);
    datos->codProd = atoi(codAux);
    *act = '\0';

    act = strrchr(linea,'"');
    *act = '\0';
    act = strrchr(linea,'"');
    strncpy(codAux,act + 1,TAM_COD);
    datos->codEmpresa = atoi(codAux);
    *act = '\0';

    act = strrchr(linea,'|');
    *act = '\0';
    act = strrchr(linea,'|');
    sscanf(act + 1,"%d",&datos->mes);
    *act = '\0';

    sscanf(linea,"%d",&datos->anio);
}

int mostrarArchDatos(const char* nombreArchBin)
{
    FILE* pf = fopen(nombreArchBin,"rb");
    if(!pf)
        return ERROR_ARCH;

    Datos datos;

    fread(&datos,sizeof(Datos),1,pf);
    while(!feof(pf))
    {
        printf("%4d\t%1d\t%d\t%d\t%8.2f\t%1d\n",datos.anio,datos.mes,datos.codEmpresa,datos.codProd,datos.precio,datos.numForm);
        fread(&datos,sizeof(Datos),1,pf);
    }

    fclose(pf);
    return TODO_OK;
}

int pasarEspecificacionesTxtABin(const char* nombreArch, Vector* vec)
{
    FILE* pf2 = fopen("especificaciones.dat","wb");
    if(!pf2)
        return ERROR_ARCH;

    FILE* pf = fopen(nombreArch,"rt");

    if(!pf)
    {
        fclose(pf2);
        return ERROR_ARCH;
    }

    char linea[TAM_LINEA];
    Especificaciones esp;

    fgets(linea,TAM_LINEA,pf);
    while(!feof(pf))
    {
        ////Me carga y separa la linea///
        leerLineaEsp(&esp, linea);
        /////Escribo en el archivo bin/////
        fwrite(&esp,sizeof(Especificaciones),1,pf2);
        vectorInsertar(vec,&esp);
        fgets(linea,TAM_LINEA,pf);
    }
    ////Archivos ya pasados a bin y vector cargado/////
    fclose(pf);
    fclose(pf2);

    return TODO_OK;
}

void leerLineaEsp(Especificaciones *esp,char* linea)
{
    char codAux[TAM_COD];
    char *act;

    act = strchr(linea,'\n');
    *act = '\0';

    act = strrchr(linea,'"');
    *act = '\0';
    act = strrchr(linea,'|');
    strncpy(esp->especifProd,act+2,TAM_ESP);
    *act = '\0';

    normalizarCadena(esp->especifProd);

    act = strrchr(linea,'"');
    *act = '\0';
    act = strrchr(linea,'|');
    strncpy(esp->nombreProd,act + 2,TAM_NOM);
    *act = '\0';

    normalizarCadena(esp->nombreProd);

    act = strrchr(linea,'"');
    *act = '\0';
    act = strrchr(linea,'"');
    strncpy(codAux,act + 1,TAM_COD);
    esp->codProd = atoi(codAux);
}

void mostrarArchEsp(const char* nombreArch)
{
    FILE* pf = fopen(nombreArch,"rb");
    if(!pf)
        printf("Error al abrir el archivo");

    Especificaciones esp;

    fread(&esp,sizeof(Especificaciones),1,pf);
    while(!feof(pf))
    {
        printf("%-10d\t%-30s\t%-40s\n",esp.codProd,esp.nombreProd,esp.especifProd);
        fread(&esp,sizeof(Especificaciones),1,pf);
    }
    fclose(pf);
}

bool vectorCrear(Vector *vec, size_t tamanioElemento)
{
    vec->vec = malloc(CAP_INI * tamanioElemento);

    if(vec->vec == NULL)
    {
        printf("\n[ERROR] No se pudo crear el vector");
        return false;
    }

    vec->ce = 0;
    vec->cap = CAP_INI;
    vec->tamElem= tamanioElemento;

    return true;
}

int vectorInsertar(Vector* vec,void* elem)
{
    if(vec->ce == vec->cap)
    {
        size_t nCap = vec->cap * FACTOR_INCR;
        void* nVec = realloc(vec->vec, nCap * vec->tamElem);

        if(!nVec)
            return SIN_MEM;

        vec->vec = nVec;
        vec->cap = nCap;
    }

    void* nuevaPosicion = vec->vec + (vec->ce * vec->tamElem);

    memcpy(nuevaPosicion,elem,vec->tamElem);

    vec->ce++;

    return TODO_OK;
}

void vectorMostrarDatos(const Vector* vector)
{
    Datos* ult = vector->vec + vector->ce - 1;
    printf("Anio\tMes\tCodEmpresa\tCodProducto\tPrecio\tNumeroFormulario\n");
    for(Datos* i = vector->vec; i < ult; i++)
        printf("%d\t%d\t%-10d\t%-10d\t%-10g\t%-10d\n",i->anio,i->mes,i->codEmpresa,i->codProd,i->precio,i->numForm);
    putchar('\n');
}

void vectorMostrarEsp(const Vector* vector)
{
    Especificaciones *ult= vector->vec + (vector->ce * vector->tamElem);
    for(Especificaciones *i = vector->vec; i < ult ;i++)
        printf("%d\t%s\t%s\n",i->codProd,i->nombreProd,i->especifProd);
}
//PUNTO 1
void vectorOrdenarSeleccion(Vector* vector, Cmp cmp)
{
    void* ult = vector->vec + (vector->ce * vector->tamElem) - vector->tamElem;
    void* m = malloc(vector->tamElem);

    for(void* i = vector->vec; i < ult; i += vector->tamElem)
        memcpy(m, _buscarMenor(i, ult,cmp,vector->tamElem),vector->tamElem);

    free(m);
}
void* _buscarMenor(void*ini, void*fin, Cmp cmp,size_t size)
{
    void* men = ini;
    for(void* j = ini + size; j <= fin; j += size)
        if(cmp(j,men) < 0)
            intercambiar(men,j,size);

    return men;
}
void intercambiar(void* ant, void* sig, size_t size)
{
    void* aTemp = malloc(size);
    memcpy(aTemp, ant, size);
    memcpy(ant, sig, size);
    memcpy(sig, aTemp, size);
    free(aTemp);
}
//PUNTO 2
void vectorOrdenarInsercion(Vector *vector,Cmp cmpEsp)
{
    void *ult = vector->vec + (vector->ce - 1) * vector->tamElem;
    void *elemAIns = malloc(vector->tamElem);
    void *i,*j;

    for(i = vector->vec + vector->tamElem; i <= ult; i += vector->tamElem)
    {
        j = i - vector->tamElem;
        memcpy(elemAIns, i, vector->tamElem);

        while(j >= vector->vec && 0 > cmpEsp(elemAIns, j))
        {
            memcpy(j + vector->tamElem, j, vector->tamElem);
            j -= vector->tamElem;
        }
        memcpy(j + vector->tamElem, elemAIns, vector->tamElem);
    }
    free(elemAIns);
}
//PUNTO 3
void normalizarCadena(char* cad)
{
    int flag = 0;
    while(*cad)
    {
        if(flag == 0)
        {
            flag = 1;
            if(ES_MINUS(*cad))
                *cad -= ('a' - 'A');
            cad++;
        }

        else if(ES_MAYUS(*cad))
        {
            *cad += ('a' - 'A');
            cad++;
        }


        else if(*cad == '.' && *(cad + 1) == ' ')
        {
            flag = 0;
            cad += 2;
        }
        else
            cad++;
    }
}
//PUNTO 4
int crearArchSinPrecio(Vector *vecDatos ,Vector * vecEsp)
{
    FILE* pfsinprecio = fopen("sinprecios.dat","wb");
    if(!pfsinprecio)
        return ERROR_ARCH;

    Especificaciones *esp,*ultEsp;
    esp = vecEsp->vec;
    ultEsp = esp + vecEsp->ce;

    while(esp < ultEsp)
    {
        if(buscarSinPrecio(esp,vecDatos) == 0) ////BUSCA SI EL PRODUCTO DE LA LINEA QUE SE LEE TIENE EL PRECIO EN EL ARCHIVO DE DATOS
            fwrite(esp,sizeof(Especificaciones),1,pfsinprecio); ////ESCRIBE EN EL ARCHIVO DE SIN PRECIO LAS ESPECIFICACIONES DEL PRODUCTO
        esp++;
    }
    fclose(pfsinprecio);
    return TODO_OK;
}
int buscarSinPrecio(void* i, Vector *vector)
{
    Datos *vecD = (Datos*)vector->vec;
    Datos *ultDat = vecD + (vector->ce);
    Especificaciones *esp = i;

    while(vecD < ultDat)
    {
        if(esp->codProd == vecD->codProd)
            return 1; ////EL PRODUCTO TIENE PRECIO
        else
            vecD++;
    }
    return TODO_OK; ////NO ENCONTRÓ EL PRODUCTO, NO TIENE PRECIO
}
//PUNTO 5
int calcularPromedio(const Vector* vecDatos,const Vector* vecEsp, const int vecPunto5[])
{
    FILE* pf = fopen("Punto5.txt","wt");
    if(!pf)
        return ERROR_ARCH;

    int indice = 0;
    float matriz[12][2] = {0};
    bool flag = 1, flag2 = 0;
    char nombreProd[TAM_NOM];

    void* i = vecDatos->vec;
    void* ult = vecDatos->vec + (vecDatos->ce * vecDatos->tamElem) - vecDatos->tamElem;
    while(i <= ult)
    {
        if(((Datos*)i)->codProd == vecPunto5[indice])
        {
            matriz[((Datos*)i)->mes][0] += ((Datos*)i)->precio;
            matriz[((Datos*)i)->mes][1]++;
            if(flag == 1)
            {
                void* iEsp = vecEsp->vec;
                void* ultEsp = vecEsp->vec + (vecEsp->ce * vecEsp->tamElem) - vecEsp->tamElem;
                while(iEsp <= ultEsp && flag2 == 0)
                {
                    if(((Datos*)i)->codProd == ((Especificaciones*)iEsp)->codProd)
                    {
                        strcpy(nombreProd,((Especificaciones*)iEsp)->nombreProd);
                        flag2 = 1;
                    }
                    iEsp += vecEsp->tamElem;
                }
            }
            flag = 0;
        }
        else if(((Datos*)i)->codProd != vecPunto5[indice] || i + vecDatos->tamElem > ult)
        {
            if(flag == 0)
            {
                flag = 1;
                indice++;
                i -= vecDatos->tamElem;
            }
            for(int j = 0;j < 12;j++)
            {
                if(matriz[j][1] > 0)
                {
                    matriz[j][0] = matriz[j][0] / matriz[j][1];
                    fprintf(pf,"%d|%d|%g|%g|%s\n",j,((Datos*)i)->codProd,matriz[j][0],matriz[j][1],nombreProd);
                    matriz[j][0] = 0;
                    matriz[j][1] = 0;
                }
            }
            flag2 = 0;
        }
        i += vecDatos->tamElem;
    }
    fclose(pf);
    return TODO_OK;
}
int mostrarPunto5()
{
    FILE* pf = fopen("Punto5.txt","rt");
    if(!pf)
        return ERROR_ARCH;

    char linea[TAM_LINEA];

    fgets(linea,TAM_LINEA,pf);
    while(!feof(pf)){
        printf("%s",linea);
        fgets(linea,TAM_LINEA,pf);
    }
    fclose(pf);
    return TODO_OK;
}
