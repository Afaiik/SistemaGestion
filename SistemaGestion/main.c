#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "gotoxy.h"

#define archivoUsuarios "arUsuarios.dat"
#define archivoProductos "arProductos.dat"
#define archivoVentas "arVentas.dat"

#define ESC 27

#define CODADMIN 123
#define TIPOADMIN 1
#define TIPONORMAL 0

typedef struct Usuario
{
    int Id;
    int Tipo;
    int Activo;
    char Nombre[30];
    char Contra[30];
}Usuario;

typedef struct Producto
{
    int Rubro;
    int Stock;
    int Activo;
    char Descripcion[50];
}Producto;

typedef struct Factura
{
    int VededorId;
    char Fecha;
    int NumeroFactura;
    Producto Productos[100];
    float PrecioTotal;
}Factura;

///Carga el archivo de Usuarios
void cargaArchivoUsuarios();

///Guarda un usuario (Usada por cargaArchivoUsuarios)
void guardarUsuario(Usuario usua);

///Muestra un usuario
void mostrarUnUsuario(Usuario usua);

///Mostrar un archivo de usuarios (usa MostrarUnUsuario)
void mostrarArchivoUsuarios(char arUsuarios[]);

///Muestra un arreglo de usuarios (usa MostrarUnUsuario)
void mostrarArregloUsuarios(Usuario usua[], int val);

///Busca el mayor ID dentro de los usuarios
int getUltimoIdUsuario(char archivo[]);

///Busca si existe un ID indicado, devuelve 1 o 0
int checkExisteUsuarioId(int idBuscado, char arUsuarios[]);
///IDEM pero busca por nombre
int checkExisteUsuarioNombre(char nombreBuscado[], char arUsuarios[]);

///Busca un usuario por ID y checkea si es ADMIN, devuelve -1 error no abrio archivo, 0 no es admin, 1 es admin
int checkTipoUsuarioById(int usuaId, char arUsuarios[]);

///Devuelve el usuario buscado por ID
Usuario getUsuarioById(int idBuscado, char arUsuarios[]);
///IDEM pero busca por nombre
Usuario getUsuarioByNombre(char nombreBuscado[], char arUsuarios[]);

///Baja Logica de usuario buscandolo por ID (Activo = 0)
int eliminarUsuarioById(int usuaId, char arUsuarios[]);



void showTituloLogin();
void showTituloBienvenido();


void menuLogin();
void doLogin();


int main()
{
    Usuario efra[50];
    Usuario usuaAux;

    int val = 0;
    int dim = 50;
    int testBusqueda;
    int testAdmin;
    char nombreAux[30];
    char nombreTest[30];
    //cargaArchivoUsuarios();

    //mostrarArchivoUsuarios(archivoUsuarios);

    menuLogin();

    return 0;
}

void showTituloBienvenido(){
    color(10);
    printf("\n########  #### ######## ##    ## ##     ## ######## ##    ## #### ########   #######  \n##     ##  ##  ##       ###   ## ##     ## ##       ###   ##  ##  ##     ## ##     ## \n##     ##  ##  ##       ####  ## ##     ## ##       ####  ##  ##  ##     ## ##     ## \n########   ##  ######   ## ## ## ##     ## ######   ## ## ##  ##  ##     ## ##     ## \n##     ##  ##  ##       ##  ####  ##   ##  ##       ##  ####  ##  ##     ## ##     ## \n##     ##  ##  ##       ##   ###   ## ##   ##       ##   ###  ##  ##     ## ##     ## \n########  #### ######## ##    ##    ###    ######## ##    ## #### ########   #######  \n");

}

void showTituloLogin(){
    color(10);
    printf("\n\t\t##        #######   ######   #### ##    ## \n\t\t##       ##     ## ##    ##   ##  ###   ## \n\t\t##       ##     ## ##         ##  ####  ## \n\t\t##       ##     ## ##   ####  ##  ## ## ## \n\t\t##       ##     ## ##    ##   ##  ##  #### \n\t\t##       ##     ## ##    ##   ##  ##   ### \n\t\t########  #######   ######   #### ##    ## \n");
}

void doLogin()
{
    Usuario usua;
    char nombreUsua[30];
    char pass[30];
    int existe;
    int login = 0;
    while(login == 0)
    {
        system("cls");
        showTituloLogin();
        printf("\n\t\tNombre de usuario.......: ");
        fflush(stdin);
        scanf("%s", nombreUsua);
        printf("\n\t\tPassword................: ");
        fflush(stdin);
        scanf("%s", pass);
        existe = checkExisteUsuarioNombre(nombreUsua, archivoUsuarios);
        if(existe == 1)
        {
            usua = getUsuarioByNombre(nombreUsua,archivoUsuarios);
            if(usua.Id != -1)
            {
                if(strcmp(strlwr(usua.Contra), strlwr(pass)) == 0)
                {
                    login = 1;
                }
            }
        }
        if(login != 1)
        {
            system("cls");
            printf("\a");
            gotoxy(30,10);
            color(4);
            printf("DATOS INCORRECTOS");
            getch();
            gotoxy(0,0);
            system("cls");

        }else
        {
            system("cls");
            gotoxy(30,10);
            color(10);
            printf("-.LOGIN OK.-");
            getch();
            gotoxy(0,0);
            system("cls");
            enDesarrollo();
        }
    }
}

void menuLogin()
{
    int opcion;
    system("cls");
    showTituloBienvenido();
    printf("\n");
    printf("\n1- Crear Usuario");
    printf("\n2- Loguearse");
    printf("\n3- *inserte una opcion loca por aqui(?)*\n ");

    opcion = getch();
    printf("\n %d", opcion);

    switch(opcion) {

       case 49:
          crearUnUsuario();
          break;

       case 50:
          doLogin();
          break;

       case 51:
          enDesarrollo();
          break;

       default :
       printf("fucku");
    }


}
void enDesarrollo()
{
    system("cls");
    printf("SECCION EN DESARROLLO");
    getch();
    system("cls");
    menuLogin();
}


void cargaArchivoUsuarios(){

    Usuario usua;
    int codAdmin;
    int valido = 0;
    char nombreAux[30];
    usua.Activo = 1;
    usua.Id = getUltimoIdUsuario(archivoUsuarios)+1;
    system("cls");

    while(valido == 0)
    {
        printf("\n\t\t<<<<<<<<<< CREACION DE USUARIO >>>>>>>>>>");
        printf("\nNombre de Usuario..............:");
        fflush(stdin);
        scanf("%s", &nombreAux);
        if(checkExisteUsuarioNombre(nombreAux,archivoUsuarios) == 0)
        {
            valido = 1;
            strcpy(usua.Nombre, nombreAux);
        }else
        {
            system("cls");
            printf("\a");
            gotoxy(15,5);
            color(4);
            printf("Ese usuario ya se encuentra en uso!");
            getch();
            gotoxy(0,0);
            system("cls");
        }
    }
    printf("\nPassword.....................:");
    fflush(stdin);
    gets(usua.Contra);

    printf("\nSi posee codigo de Administrador, Ingreselo..:");
    scanf("%d", &codAdmin);
    if(codAdmin == CODADMIN)
        usua.Tipo = TIPOADMIN;
    else///TIPO DE USUARIO 1 ADMIN 0 USUARIO REGULAR
        usua.Tipo = TIPONORMAL;

    guardarUsuario(usua);

}

void crearUnUsuario()
{
    Usuario usua;
    int codAdmin;
    int valido = 0;
    char nombreAux[30];
    usua.Activo = 1;
    usua.Id = getUltimoIdUsuario(archivoUsuarios)+1;
    system("cls");

    while(valido == 0)
    {
        color(10);
        printf("\n\t\t<<<<<<<<<< CREACION DE USUARIO >>>>>>>>>>");
        printf("\nNombre de Usuario..............:");
        fflush(stdin);
        scanf("%s", &nombreAux);
        if(checkExisteUsuarioNombre(nombreAux,archivoUsuarios) == 0)
        {
            valido = 1;
            strcpy(usua.Nombre, nombreAux);
        }else
        {
            system("cls");
            printf("\a");
            gotoxy(15,5);
            printf("Ese usuario ya se encuentra en uso!");
            getch();
            gotoxy(0,0);
            system("cls");
        }
    }
    printf("\nPassword.....................:");
    fflush(stdin);
    gets(usua.Contra);

    printf("\nSi posee codigo de Administrador, Ingreselo..:");
    scanf("%d", &codAdmin);
    if(codAdmin == CODADMIN)
        usua.Tipo = TIPOADMIN;
    else///TIPO DE USUARIO 1 ADMIN 0 USUARIO REGULAR
        usua.Tipo = TIPONORMAL;

    guardarUsuario(usua);
}

void guardarUsuario(Usuario usua){
    FILE *pArchUsuarios = fopen(archivoUsuarios,"ab");
    if(pArchUsuarios != NULL){
        fwrite(&usua, sizeof(Usuario),1,pArchUsuarios);
        fclose(pArchUsuarios);
    }
}

void mostrarUnUsuario(Usuario usua)
{
    if(usua.Activo == 1)
    {
        printf("\n ID: %d", usua.Id);
        printf("\n Activo: %d", usua.Activo);
        printf("\n Tipo: %d", usua.Tipo);
        printf("\n Nombre: %s", usua.Nombre);
        printf("\n contrase�a: %s", usua.Contra);
        printf("\n____________________");
    }
}

void mostrarArregloUsuarios(Usuario usua[], int val)
{
    for(int i = 0; i < val; i++)
    {
        mostrarUnUsuario(usua[i]);
    }
}

void mostrarArchivoUsuarios(char arUsuarios[])
{
    FILE *archi;
    archi = fopen(arUsuarios, "rb");
    Usuario usuaAux;

    if(archi != NULL)
    {
        printf("\nContenido del archivo:\n\n");
        while(fread(&usuaAux, sizeof(Usuario),1,archi)>0)
        {
            if(usuaAux.Activo == 1)
            {
                mostrarUnUsuario(usuaAux);
            }
        }
        fclose(archi);
    }
}

int getUltimoIdUsuario(char archivo[])
{
    int mayor = 0;
    FILE *archi;
    archi = fopen(archivo, "rb");
    Usuario usuaAux;

    if(archi != NULL)
    {
        while(!feof(archi)) ///Se cambio de fread > 0 a !feof porque no funcionaba
        {
            fread(&usuaAux,sizeof(Usuario),1,archi);
            if(!feof(archi))
            {
                if(usuaAux.Id > mayor)
                    mayor = usuaAux.Id;
            }
        }
        fclose(archi);
    }
    return mayor;
}

int checkExisteUsuarioId(int idBuscado, char arUsuarios[])
{
    Usuario usua;
    int flag = -1; /// -1 error no abrio archivo, 0 no encontro ID, 1 encontro ID
    FILE *pArch;
    pArch = fopen(arUsuarios, "rb");

    if(pArch != NULL)
    {
        while(fread(&usua, sizeof(Usuario), 1, pArch)>0)
        {
            if((usua.Id == idBuscado) && (usua.Activo == 1))
            {
                flag = 1;
            }
        }
        if(flag != 1)
        {
            flag = 0;
        }
        fclose(pArch);
    }

   return flag;
}

int checkExisteUsuarioNombre(char nombreBuscado[], char arUsuarios[])
{
    Usuario usua;
    int flag = -1; /// -1 error no abrio archivo, 0 no encontro ID, 1 encontro ID
    FILE *pArch;
    pArch = fopen(arUsuarios, "rb");

    if(pArch != NULL)
    {
        while(fread(&usua, sizeof(Usuario), 1, pArch)>0)
        {
            if((strcmp(strlwr(usua.Nombre), strlwr(nombreBuscado)) == 0) && (usua.Activo == 1))
            {
                flag = 1;
            }
        }
        if(flag != 1)
        {
            flag = 0;
        }
        fclose(pArch);
    }
   return flag;
}

int checkTipoUsuarioById(int usuaId, char arUsuarios[])
{

    Usuario usua;
    int flag = -1; /// -1 error no abrio archivo, 0 no es admin, 1 es admin
    FILE *pArch;
    pArch = fopen(arUsuarios, "rb");
    if(usuaId <= getUltimoIdUsuario(arUsuarios))
    {
        if(pArch != NULL)
        {
            while(fread(&usua, sizeof(Usuario), 1, pArch)>0)
            {
                if(usua.Id == usuaId)
                {
                    if(usua.Tipo == 1)
                        flag = 1;
                }
            }
            if(flag != 1)
            {
                flag = 0;
            }
            fclose(pArch);
        }
    }else
    {
        system("cls");
        printf("\a");
        gotoxy(15,5);
        printf("No existe ningun usuario con ese id.");
        getch();
        gotoxy(0,0);
        system("cls");

    }
   return flag;
}

Usuario getUsuarioById(int idBuscado, char arUsuarios[])
{
    Usuario usua;
    int flag = 0;
    char stringAux[30] = "Usuario no encontrado";
    FILE *pArch;
    pArch = fopen(arUsuarios, "rb");

    if(idBuscado <= getUltimoIdUsuario(arUsuarios))
    {
        if(pArch != NULL)
        {
            while((flag == 0) && (fread(&usua, sizeof(Usuario), 1, pArch)>0))
            {
                if(usua.Id == idBuscado)
                {
                    flag = 1;
                }
            }
            if((flag == 0) || (usua.Activo == 0))
            {
                usua.Id = -1;
                strcpy(usua.Nombre, stringAux);
            }

            fclose(pArch);
        }
    }else
    {
        system("cls");
        printf("\a");
        gotoxy(15,5);
        printf("No existe ningun usuario con ese id.");
        getch();
        gotoxy(0,0);
        system("cls");

    }

   return usua;
}

Usuario getUsuarioByNombre(char nombreBuscado[], char arUsuarios[])
{
    Usuario usua;
    int flag = 0;
    char stringAux[30] = "Usuario no encontrado";
    FILE *pArch;
    pArch = fopen(arUsuarios, "rb");

        if(pArch != NULL)
        {
            while((flag == 0) && (fread(&usua, sizeof(Usuario), 1, pArch)>0))
            {
                if(strcmp(strlwr(usua.Nombre), strlwr(nombreBuscado)) == 0)
                {
                    flag = 1;
                }
            }
            if((flag == 0) || (usua.Activo == 0))
            {
                usua.Id = -1;
                strcpy(usua.Nombre, stringAux);
            }

            fclose(pArch);
        }
   return usua;
}

int eliminarUsuarioById(int usuaId, char arUsuarios[])
{
    Usuario usua;
    int flag = -1; /// -1 error no abrio archivo, 0 no es admin, 1 es admin
    FILE *pArch;
    pArch = fopen(arUsuarios, "rb");

    if(pArch != NULL)
    {
        while(fread(&usua, sizeof(Usuario), 1, pArch)>0)
        {
            if(usua.Id == usuaId)
            {
                if(usua.Tipo == 1)
                    flag = 1;
            }
        }
        if(flag != 1)
        {
            flag = 0;
        }
        fclose(pArch);
    }
   return flag;
}


