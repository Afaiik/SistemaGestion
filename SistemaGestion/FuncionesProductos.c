#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define archivoProductos "arProductos.dat"
#define ESC 27
#define TIPOADMIN 1
#define DOLAR 40

typedef struct Producto
{
    int Id;
    int Rubro;
    int Stock;
    int Activo;
    float Precio;
    char Descripcion[50];
} Producto;


void CargarProductos()
{
    int opcion = 0;
    Producto Prod;
    while(opcion != ESC)
    {
        system("cls");
        Prod.Id = getUltimoIdProducto(archivoProductos)+1;
        Prod.Activo = 1;

        printf("Ingrese la descripcion del producto...:\n");
        fflush(stdin);
        gets(Prod.Descripcion);
        printf("Ingrese el precio del producto...:\n");
        scanf("%f", &Prod.Precio);
        printf("Ingrese stock del producto...:\n");
        scanf("%d", &Prod.Stock);
        printf("Ingrese el rubro de producto...:\n");
        scanf("%d", &Prod.Rubro);
        GuardaUnProducto(Prod);
        printf("\t\tESC PARA SALIR");
        opcion = getch();
    }
     
}

void GuardaUnProducto(Producto Prod)
{
    FILE *pArchivo;
    pArchivo = fopen(archivoProductos, "a+b");
    if(pArchivo != NULL)
    {
        fwrite(&Prod, sizeof(Producto), 1, pArchivo);
        fclose(pArchivo);
    }
}

void MostrarUnProducto(Producto produc)
{
    if(produc.Activo == 1)
    {
        printf("\n");
        printf("ID: %d\n", produc.Id);
        printf("Activo: %d\n", produc.Activo);
        printf("Rubros: %d\n", produc.Rubro);
        printf("Stock: %d\n", produc.Stock);
        printf("Descripcion %s\n", produc.Descripcion);
        printf("Precio: %.2f\n", produc.Precio*DOLAR);
        printf("\n____________________");
    }
}


void MostrarArregloProdCargados(Producto Prod[], int val)
{
    int i;
    for(i = 0; i < val; i++)
    {
        MostrarUnProducto(Prod[i]);
    }
}

void MostrarArchivoProductos(char arProductos[])
{
    FILE *archi;
    archi = fopen(arProductos, "r+b");
    Producto Prod;
    if(archi != NULL)
    {
        printf("\nContenido del archivo:\n\n");
        while(fread(&Prod, sizeof(Producto),1,archi)>0)
        {
            if(Prod.Activo == 1)
            {
                MostrarUnProducto(Prod);
            }
        }
        fclose(archi);
    }
}

int getUltimoIdProducto(char archivo[])
{
    int mayor = 0;
    FILE *archi;
    archi = fopen(archivo, "rb");
    Producto prodAux;

    if(archi != NULL)
    {
        while(!feof(archi)) ///Se cambio de fread > 0 a !feof porque no funcionaba
        {
            fread(&prodAux,sizeof(Producto),1,archi);
            if(!feof(archi))
            {
                if(prodAux.Id > mayor)
                    mayor = prodAux.Id;
            }
        }
        fclose(archi);
    }
    return mayor;
}

Producto buscarProductoPorNombre(){
char buscarNomAux[50];
Producto prodAux;
int corta = 0;
printf("INGRESE EL NOMBRE DEL PRODUCTO:\n");
fflush(stdin);
scanf("%s",&buscarNomAux);

FILE *pArch = fopen(archivoProductos, "rb");

    if(pArch != NULL){
       while((corta != 1) && (fread(&prodAux, sizeof(Producto), 1, pArch) > 0)){
               if(strcmpi(prodAux.Descripcion, buscarNomAux)== 0){
                    corta = 1;
               }
        }
    fclose(pArch);
    }
 return prodAux;
}

Producto buscarProductoPorRubro(){
int buscarRubroAux;
Producto prodAux;
printf("INGRESE EL RUBRO DEL PRODUCTO:\n");
fflush(stdin);
scanf("%d",&buscarRubroAux);

FILE *pArch = fopen(archivoProductos, "rb");

    if(pArch != NULL){
       while(fread(&prodAux, sizeof(Producto), 1, pArch) > 0){
               if(prodAux.Rubro == buscarRubroAux){
                    MostrarUnProducto(prodAux);
               }
        }
    fclose(pArch);
    }
 return prodAux;
}

Producto buscarProductoPorPrecio(){
float buscarPrecioAux;
Producto prodAux;
printf("INGRESE EL PRECIO DEL PRODUCTO:\n");
fflush(stdin);
scanf("%f",&buscarPrecioAux);
buscarPrecioAux = buscarPrecioAux / DOLAR;

FILE *pArch = fopen(archivoProductos, "rb");

    if(pArch != NULL){
       while(fread(&prodAux, sizeof(Producto), 1, pArch) > 0){
               if(prodAux.Precio == buscarPrecioAux){
                    MostrarUnProducto(prodAux);
               }
        }
    fclose(pArch);
    }
 return prodAux;
}

Producto buscarProductoPorId(int id){

    Producto prodAux;
    int corta = 0;
    FILE *pArch = fopen(archivoProductos, "rb");

        if(pArch != NULL){
            while((corta != 1) && (fread(&prodAux, sizeof(Producto), 1, pArch) > 0)){
                    if(prodAux.Id == id){
                            corta = 1;
                    }
                }
            fclose(pArch);
        }
    return prodAux;
}