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

