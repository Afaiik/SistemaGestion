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

void GuardaUnProducto(Producto Prod);
void CargarProductos();
void MostrarUnProducto(Producto produc);
void MostrarArregloProdCargados(Producto Produc[], int val);
void MostrarArchivoProductos(char arProductos[]);

int main()
{
    CargarProductos();
    char arProductos;
    MostrarArchivoProductos(arProductos);
    return 0;
}

void CargarProductos()
{
    int opcion = 0;
    Producto Prod;
    while(opcion != ESC)
    {
        system("cls");
        Prod.Id = 1;
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
        printf("\t\tESC PARA SALIR");
        opcion = getch();
    }
     GuardaUnProducto(Prod);
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
        printf("ID: %d\n", produc.Id);
        printf("Activo: %d\n", produc.Activo);
        printf("Rubros: %d\n", produc.Rubro);
        printf("Stock: %d\n", produc.Stock);
        printf("Descripcion %s\n", produc.Descripcion);
        printf("Precio: %f\n", produc.Precio*DOLAR);
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
