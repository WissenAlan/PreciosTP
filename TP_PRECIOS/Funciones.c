#include "Funciones.h"

void* _buscarMenor(void*ini, const void*fin, const Cmp cmp,const size_t size);

int pasarDatosTxtABin(const char* nombreArch, Vector* vec, ErrorFatal errorFatal)
{
    int ret = TODO_OK;
    char linea[TAM_LINEA];

    FILE* pf2 = fopen("Datos.dat","wb");
    if(!pf2)
        return ERROR_ARCH;

    FILE* pf = fopen(nombreArch,"rt");
    Datos datos;

    if(!pf)
        return ERROR_ARCH;

    while(fgets(linea,TAM_LINEA,pf) && !errorFatal(ret))
    {
        ret = leerLinea(&datos,linea); ////Me carga y separa la linea///
        if(ret == TODO_OK)
        {
            vectorInsertar(vec,&datos);////Inserto en el vector////
            fwrite(&datos,sizeof(Datos),1,pf2);
        }
    }

    fclose(pf);
    fclose(pf2);
    return TODO_OK;
}

int leerLinea(Datos *datos,char* linea)
{
    char codAux[TAM_COD];

    char *act = strchr(linea,'\n');

    if(!act)
        return ERR_LINEA_LARGA;

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

    return TODO_OK;
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

int pasarEspecificacionesTxtABin(const char* nombreArch, Vector* vec,ErrorFatal errorFatal)
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
    int ret = TODO_OK;

    while(fgets(linea,TAM_LINEA,pf) && !errorFatal(ret))
    {
        ////Me carga y separa la linea///
        ret = leerLineaEsp(&esp, linea);
        if(ret == TODO_OK)
        {
            /////Escribo en el archivo bin/////
            fwrite(&esp,sizeof(Especificaciones),1,pf2);
            vectorInsertar(vec,&esp);
        }
    }
    ////Archivos ya pasados a bin y vector cargado/////
    fclose(pf);
    fclose(pf2);

    return TODO_OK;
}

int leerLineaEsp(Especificaciones *esp,char* linea)
{
    char codAux[TAM_COD];
    char *act;

    act = strchr(linea,'\n');
    if(!act)
        return ERR_LINEA_LARGA;
    *act = '\0';

    act = strrchr(linea,'"');
    *act = '\0';
    act = strrchr(linea,'|');
    strncpy(esp->especifProd,act+2,TAM_ESP);
    esp->especifProd[TAM_ESP] = '\0';
    *act = '\0';

    normalizarCadena(esp->especifProd);

    act = strrchr(linea,'"');
    *act = '\0';
    act = strrchr(linea,'|');
    strncpy(esp->nombreProd,act + 2,TAM_NOM);
    esp->nombreProd[TAM_NOM-1] = '\0';
    *act = '\0';

    normalizarCadena(esp->nombreProd);

    act = strrchr(linea,'"');
    *act = '\0';
    act = strrchr(linea,'"');
    strncpy(codAux,act + 1,TAM_COD);
    esp->codProd = atoi(codAux);

    return TODO_OK;
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
    const Datos* ult = vector->vec + vector->ce * vector->tamElem;
    printf("Anio\tMes\tCodEmpresa\tCodProducto\tPrecio\tNumeroFormulario\n");
    for(Datos* i = vector->vec; i < ult; i++)
        printf("%d\t%d\t%-10d\t%-10d\t%-10g\t%-10d\n",i->anio,i->mes,i->codEmpresa,i->codProd,i->precio,i->numForm);
    putchar('\n');
}

void vectorMostrarEsp(const Vector* vector)
{
    const Especificaciones *ult= vector->vec + (vector->ce * vector->tamElem);
    for(Especificaciones *i = vector->vec; i < ult ;i++)
        printf("%d\t%s\t%s\n",i->codProd,i->nombreProd,i->especifProd);
}
//PUNTO 1
void vectorOrdenarSeleccion(const Vector* vector,const Cmp cmp)
{
    const void* ult = vector->vec + (vector->ce * vector->tamElem) - vector->tamElem;
    void* m = malloc(vector->tamElem);

    for(void* i = vector->vec; i < ult; i += vector->tamElem)
        memcpy(m, _buscarMenor(i, ult,cmp,vector->tamElem),vector->tamElem);

    free(m);
}
void* _buscarMenor(void*ini, const void*fin, const Cmp cmp,const size_t size)
{
    void* men = ini;
    for(void* j = ini + size; j <= fin; j += size)
        if(cmp(j,men) < 0)
            intercambiar(men,j,size);

    return men;
}
void intercambiar(void* ant, void* sig, const size_t size)
{
    void* aTemp = malloc(size);
    memcpy(aTemp, ant, size);
    memcpy(ant, sig, size);
    memcpy(sig, aTemp, size);
    free(aTemp);
}
//PUNTO 2
void vectorOrdenarInsercion(const Vector *vector,const Cmp cmpEsp)
{
    const void *ult = vector->vec + (vector->ce - 1) * vector->tamElem;
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
        }

        else if(ES_MAYUS(*cad))
            *cad += ('a' - 'A');

        else if(*cad == '.' && *(cad + 1) == ' ')
        {
            flag = 0;
            cad++;
        }
        cad++;
    }
}
//PUNTO 4
int crearArchSinPrecio(const Vector *vecDatos,const Vector * vecEsp)
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
int buscarSinPrecio(const void* i,const Vector *vector)
{
    Datos *vecD = (Datos*)vector->vec;
    const Datos *ultDat = vecD + (vector->ce);
    const Especificaciones *esp = i;

    while(vecD->codProd <= esp->codProd && vecD < ultDat) //Como vecD ya esta ordenado por codProd, si es mayor al buscado no deberia encontrarlo
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
    bool flag = 0;
    char nombreProd[TAM_NOM];

    Datos* i = vecDatos->vec;
    const Datos* ult = vecDatos->vec + vecDatos->ce*vecDatos->tamElem - vecDatos->tamElem;
    while(i <= ult)
    {
        if(i->codProd == vecPunto5[indice])
        {
            matriz[i->mes-1][0] += i->precio;
            matriz[i->mes-1][1]++;
            flag=1;
        }
        if((i->codProd != vecPunto5[indice] || i + 1 > ult) && flag)//Si no coinciden los codigos, o si no hay proximo en el vector
        {
            if(i + 1 <= ult)
                i--;//Vuelve uno atras para hacer la impresion por pantalla
            buscarNombre(vecEsp, i->codProd, nombreProd);
            flag = 0;//Habilita la busqueda de nombre
            indice++;//Aumenta el indice para la proxima vuelta
            for(int j = 0;j < 12;j++)//Hasta que j sea menor a 12 por los 12 meses del año
            {
                if(matriz[j][1] > 0)//Imprime solo si hay algo en la matriz
                {
                    matriz[j][0] = matriz[j][0] / matriz[j][1];//Calculo del promedio
                    fprintf(pf,"%d|%d|%g|%g|%s\n",j+1,i->codProd,matriz[j][0],matriz[j][1],nombreProd);//Carga la linea al archivo
                    //Resetea la posicion
                    matriz[j][0] = 0;
                    matriz[j][1] = 0;
                }
            }
        }
        i++;
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
//PUNTO 6
int calcularVarianzayDesvio(const Vector* vec)
{
    Datos *ini,*u,*i,*limsup;
    ini = (Datos*)vec->vec;
    u = ini + vec->ce;
    float sumacuadr = 0,prom = 0,varianzaMes = 0,desv;
    int  mesact = ini->mes,cont = 0,k,numFormSiguiente,band=0, *numFormActual;;
    Vector vecNF;

    ////Vector que me va a almacenar todos los Numeros de formularios que posea cada mes, y la cant de NF////
    vectorCrear(&vecNF,sizeof(int));


    while(ini < u)
    {
        //me devuelve el vector cargado y hasta donde tiene que recorrer(Cuando cambia de mes)
        limsup=BuscarNuevoMes(ini,u,mesact,&vecNF);
        numFormActual = vecNF.vec;


        ////CantNF = cantidad de veces que tiene que recorrer para calcular todos los NF de cada mes
        for(k=0;k<vecNF.ce;k++)
        {
             sumacuadr=0;
             cont=0;
             i=ini;
             band=0;

             prom=calcularPromedio2(mesact,*numFormActual,i);

              while(i<limsup)
              {
                        if(*(numFormActual) == i->numForm)
                        {
                            sumacuadr += pow(i->precio - prom,2);
                            cont++;
                        }

                    i++;
                }

                    varianzaMes = sumacuadr/(cont);
                    desv = sqrt(varianzaMes);
                    printf("Mes:%-10d\tNumForm:%-10d\tVarianza:%.2f\t\tDesvio estandar:%.2f\n", mesact,*(numFormActual),varianzaMes, desv);
                    numFormActual ++;
         }
        ////Cuando cambio de mes, puedo tener distintos numeros de formularios////
         vectorVaciar(&vecNF);
         ini = limsup;
         mesact = ini->mes;
     }
    vectorEliminar(&vecNF);
    return TODO_OK;
}

void vectorVaciar(Vector* v)
{
    v->ce = 0;
    redimensionarVector(v, CAP_INI);
}

bool redimensionarVector(Vector* v, size_t nuevaCap)
{
    void* nVec = realloc(v->vec, nuevaCap * v->tamElem);

    if(!nVec)
    {
       return false;
    }

    v->vec = nVec;



    v->cap = nuevaCap;
    return true;
}


Datos* BuscarNuevoMes(Datos* ini,Datos* u, int mesact, Vector *vecNF)
{
    Datos *i=ini;

    while(i<u && i->mes==mesact)
    {
        ////Me recorre hasta donde estoy parada, si el num de form ya existia antes devuelve un 1, si devuelve 0 es un nuevo NF///
       if(BuscarSiYaExistia(i->numForm,ini,i)==0)
        {
            vectorInsertar(vecNF,&(i->numForm));
        }

        i++;
    }

    return i;
}

int BuscarSiYaExistia(int dato,Datos* limInf, Datos* limSup)
{
    int band=0;
    Datos* i=limInf;

    while(i<limSup && band==0)
    {
          if(i->numForm==dato)
          {
              band=1;
          }
          i++;
    }

    return band;
}


float calcularPromedio2(int mesact,int formact,Datos* i)
{
    float sumaprecios=0,cont=0,promedio;

    while(mesact == i->mes)
    {
        if(formact==i->numForm)
        {
            sumaprecios += i->precio;

            cont++;
        }

        i++;
    }
        if(cont > 0)
        {
            promedio=sumaprecios/cont;
        }

    return promedio;
}


//PUNTO 7
void calcularMedidaGeometrica(const Vector* vecDatos,const Vector* vecEsp)
{
    const Datos *ult = vecDatos->vec + vecDatos->ce*vecDatos->tamElem - vecDatos->tamElem;
    char nombre[TAM_NOM];
    float acum = 0;
    int codAux = 0,cont = 0;
    for(Datos *i = vecDatos->vec; i <= ult; i++)
    {
        if(!codAux || codAux == i->codProd)
        {
            if(!codAux)
                codAux = i->codProd;
            acum += log(i->precio);
            cont++;
        }
        if(codAux != i->codProd || i + 1 > ult)
        {
            buscarNombre(vecEsp,codAux,nombre);
            printf("%-10d\t%-50s\t%g\n",codAux,nombre,exp(acum/cont));
            acum = 0;
            cont = 0;
            codAux = 0;
            if(i + 1 <= ult)
                i--;
        }
    }
}

void buscarNombre(const Vector* vec,const int codBuscar, char* nombreExp)
{
    Especificaciones* iEsp = vec->vec;
    Especificaciones* ultEsp = vec->vec + vec->ce*vec->tamElem;
    while(iEsp <= ultEsp)
    {
        if(codBuscar == iEsp->codProd)
        {
            strcpy(nombreExp,iEsp->nombreProd);
            return; //Cuando encuentra el nombre retorna 1
        }
        iEsp++;
    }
}

void vectorEliminar(Vector* vec)
{
    free(vec->vec);
    vec->vec = NULL;
    vec->ce = 0;
    vec->cap = 0;
    vec->tamElem = 0;
}

