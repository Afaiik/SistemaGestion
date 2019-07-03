#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

#include "gotoxy.h"
#include "FuncionesProductos.h"
#include "FuncionesUsuarios.h"

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


typedef struct Factura
{
    int VendedorId;
    char Fecha[30];
    int NumeroFactura;
    Producto Productos[100];
    int Validos;
    float PrecioTotal;
}Factura;

///Crea usuarios en loop
void cargaArchivoUsuarios();

///Crear un usuario
void crearUnUsuario();

///Guarda un usuario (Usada por cargaArchivoUsuarios)
void guardarUsuario(Usuario usua);

///Muestra un usuario
void mostrarUnUsuario(Usuario usua);

///Mostrar un archivo de usuarios (usa MostrarUnUsuario)
void mostrarArchivoUsuarios(char arUsuarios[]);
///Mostrar Administradores
void mostrarUsuariosAdministradores(char arUsuarios[]);

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
int eliminarUsuarioById(int usuaId);
///Interfaz de eliminacion de usuario
void eliminacionDeUsuario(Usuario usuaLogueado);
///Eliminacion del propio usuario
void eliminarMiUsuario(Usuario usuaLogueado);
///Eliminar usuarios
void menuEliminarUsuarios(Usuario usuaLogueado);


///TITULOS
void showTituloLogin();
void showTituloBienvenido();

///Funcion auxiliar
void enDesarrollo();

///MENU LOGIN
void menuLogin();
void doLogin();

///MENU PRINCIPAL Toma como parametro el usuario que se logueo para saber que opciones mostrar
void menuPrincipal(Usuario usuaLogueado);

///MENU GESTION USUARIOS
void menuGestionUsuarios(Usuario usuaLogueado);

///Modificar Password de Usuario
void modificarPassword(Usuario usuaLogueado, char newPass[30]);
void menuModificarPassword(Usuario usuaLogueado);


///Cuenta los registros del archivo sin recorrerlo
int contarRegistrosUsuarios(char rutaArchivo[]);
int contarRegistrosProductos(char rutaArchivo[]);

///
void menuGestionProductos(Usuario usuaLogueado);
void showTituloProductos();


///VALIDAR QUE EXISTA EL ID DE PRODUCTO
int checkExisteProductoId(int idBuscado);

///CALCULA EL PRECIO TOTAL DE UNA FACTURA
float calculaPrecioTotal(Producto productos[], int validos);

///FUNCIONES NECESARIAS PARA LA CARGA DE UNA FACTURA
void crearFactura(Usuario usuaLogueado);
void guardaUnaFactura(Factura factu);
int getUltimoIdFactura();
void mostrarArchivoVentas();

int main()
{
    system("color F0");
    menuLogin();
    return 0;
}

void modificarPassword(Usuario usuaLogueado, char newPass[30])
{
    Usuario usuaAux;
    int flag = 0;
    FILE *pArch= fopen(archivoUsuarios, "r+b");
    if(pArch != NULL)
    {
        while((flag != 1) && (fread(&usuaAux, sizeof(Usuario), 1, pArch) > 0))
        {
            if(usuaAux.Id == usuaLogueado.Id)
            {
                flag = 1;
            }
        }
    strcpy(usuaLogueado.Contra, newPass);
    fseek(pArch,sizeof(Usuario) * (-1), SEEK_CUR);
    fwrite(&usuaLogueado,sizeof(Usuario), 1, pArch);
    fclose(pArch);
    }

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
            //color(4);
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
    int checkExist = 2;
    usua.Activo = 1;
    usua.Id = getUltimoIdUsuario(archivoUsuarios)+1;
    system("cls");

    while(valido == 0)
    {
        //color(10);
        printf("\n\t\t<<<<<<<<<< CREACION DE USUARIO >>>>>>>>>>");
        printf("\nNombre de Usuario..............:");
        fflush(stdin);
        scanf("%s", &nombreAux);
        checkExist = checkExisteUsuarioNombre(nombreAux, archivoUsuarios);
        if((checkExisteUsuarioNombre(nombreAux, archivoUsuarios) == 0) || (checkExisteUsuarioNombre(nombreAux, archivoUsuarios) == -1))
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
    menuLogin();
}

void guardarUsuario(Usuario usua){
    FILE *pArchUsuarios = fopen(archivoUsuarios,"a+b");
    if(pArchUsuarios != NULL){
        fseek(pArchUsuarios, sizeof(Usuario), SEEK_END);
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

void mostrarUsuariosAdministradores(char arUsuarios[])
{
    FILE *archi;
    archi = fopen(arUsuarios, "rb");
    Usuario usuaAux;

    if(archi != NULL)
    {
        while(fread(&usuaAux, sizeof(Usuario),1,archi)>0)
        {
            if((usuaAux.Activo == 1) && (usuaAux.Tipo == TIPOADMIN))
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
    FILE *pArch = fopen(arUsuarios, "rb");

    if(pArch != NULL)
    {
        while(fread(&usua, sizeof(Usuario), 1, pArch) > 0)
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
    pArch = fopen(archivoUsuarios, "rb");

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


int eliminarUsuarioById(int usuaId)
{
    Usuario usua;
    int flag = 0;
    FILE *pArch;
    pArch = fopen(archivoUsuarios, "r+b");

    if(pArch != NULL)
    {
        while((flag == 0) && (fread(&usua, sizeof(Usuario), 1, pArch)>0))
        {
            if(usua.Id == usuaId)
            {
                if(usua.Activo == 1)
                {
                    flag = 1;
                }
            }
        }
    usua.Activo = 0;
    fseek(pArch, sizeof(Usuario)*(-1), SEEK_CUR);
    fwrite(&usua, sizeof(Usuario), 1, pArch);
    fclose(pArch);
    }
   return flag;
}

void eliminarMiUsuario(Usuario usuaLogueado)
{
    Usuario usua;
    int flag = -1;
    FILE *pArch;
    char opcion;
    system("cls");
    printf("\n\t\t<<<<<<<<<ELIMINACION DE USUARIO>>>>>>>>>");
    printf("\n\n\t\tESTA SEGURO DE ELIMINAR SU USUARIO ?? S/N \n");
    fflush(stdin);
    scanf("%c", &opcion);
    if((opcion == 'S') || (opcion == 's'))
    {
        printf("\nMI USUARIO:");
        mostrarUnUsuario(usuaLogueado);
        getch();
        flag = eliminarUsuarioById(usuaLogueado.Id);
        if(flag == 1)
        {
            system("cls");
            printf("\a");
            gotoxy(15,5);
            printf("\nEliminacion exitosa.");
            getch();
            gotoxy(0,0);
            system("cls");
            menuLogin();
        }else
        {

            system("cls");
            printf("\a");
            gotoxy(15,5);
            printf("\nNo se pudo completar la operacion.");
            getch();
            gotoxy(0,0);
            system("cls");
            menuLogin();
        }
    }
}




void showTituloBienvenido(){
    ///color(10);
    printf("\n########  #### ######## ##    ## ##     ## ######## ##    ## #### ########   #######  \n##     ##  ##  ##       ###   ## ##     ## ##       ###   ##  ##  ##     ## ##     ## \n##     ##  ##  ##       ####  ## ##     ## ##       ####  ##  ##  ##     ## ##     ## \n########   ##  ######   ## ## ## ##     ## ######   ## ## ##  ##  ##     ## ##     ## \n##     ##  ##  ##       ##  ####  ##   ##  ##       ##  ####  ##  ##     ## ##     ## \n##     ##  ##  ##       ##   ###   ## ##   ##       ##   ###  ##  ##     ## ##     ## \n########  #### ######## ##    ##    ###    ######## ##    ## #### ########   #######  \n");

}

void showTituloLogin(){
    ///color(10);
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
            usua = getUsuarioByNombre(nombreUsua, archivoUsuarios);
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
            //color(4);
            printf("DATOS INCORRECTOS");
            getch();
            gotoxy(0,0);
            system("cls");
            doLogin();
        }else
        {
            system("cls");
            gotoxy(30,10);
            //color(10);
            printf("-.LOGIN OK.-");
            getch();
            gotoxy(0,0);
            system("cls");
            menuPrincipal(usua);
        }
    }
}

void menuLogin()
{
    int opcion;
    int opcionValida = 0;
    system("cls");
    showTituloBienvenido();
    printf("\n");
    printf("\n\t1- Crear Usuario");
    printf("\n\t2- Loguearse\n\t");

    opcion = getch();
    printf("\n %d", opcion);
    while(opcionValida != 1)
    {
        switch(opcion) {

            case 49:
                opcionValida = 1;
                crearUnUsuario();
                break;

            case 50:
                opcionValida = 1;
                doLogin();
                break;

            default:
                system("cls");
                gotoxy(30,10);
                //color(4);
                printf("OPCION INVALIDA");
                getch();
                gotoxy(0,0);
                system("cls");

                break;
        }
    }
}

void menuPrincipal(Usuario usuaLogueado)
{
    int opcion;
    int opcionValida = 0;
    system("cls");
    showTituloBienvenido();

    if(usuaLogueado.Tipo == TIPOADMIN){
        printf("\n");
        printf("\n\t1- Gestionar productos");
        printf("\n\t2- Gestionar usuarios");
        printf("\n\t3- Facturar ");
        printf("\n\t4- Mostrar Facturas\n ");
    }else{
        printf("\n");
        printf("\n\t1- Facturar");
        printf("\n\t2- Listar productos");
        printf("\n\t3- Cambiar password ");
        printf("\n\t4- Mostrar Facturas\n ");
    }
    opcion = getch();
    printf("\n %d", opcion);
    while(opcionValida != 1)
    {
        switch(opcion) {

           case 49:
               opcionValida = 1;
               if(usuaLogueado.Tipo == TIPOADMIN)
               {
                    menuGestionProductos(usuaLogueado);///gestionar productos
               }else
               {
                   crearFactura(usuaLogueado);///Facturar
                   menuPrincipal(usuaLogueado);
               }
              break;

           case 50:
               opcionValida = 1;
                if(usuaLogueado.Tipo == TIPOADMIN)
               {
                    menuGestionUsuarios(usuaLogueado);
               }else
               {
                   enDesarrollo(usuaLogueado);///listar productos
               }
              break;

           case 51:
               opcionValida = 1;
                if(usuaLogueado.Tipo == TIPOADMIN)
               {
                    crearFactura(usuaLogueado);///facturar
                    menuPrincipal(usuaLogueado);
               }else
               {
                   menuModificarPassword(usuaLogueado);
               }
              break;
            case 52:
                opcionValida = 1;
                mostrarArchivoFacturas();
                menuPrincipal(usuaLogueado);
                break;  
           default :
            printf("OPCION INVALIDA");
        }
    }


}
void enDesarrollo(Usuario usuaLogueado)
{
    system("cls");
    printf("SECCION EN DESARROLLO");
    getch();
    system("cls");
    menuPrincipal(usuaLogueado);
}

void menuGestionUsuarios(Usuario usuaLogueado)
{
    int opcion;
    int opcionValida = 0;
    system("cls");
    showTituloBienvenido();

    if(usuaLogueado.Tipo == TIPOADMIN){
        printf("\n");
        printf("\n\t1- Modificar mi password");
        printf("\n\t2- Mostrar usuarios");
        printf("\n\t3- Mostrar administradores");
        printf("\n\t4- Eliminar usuarios");
        printf("\n\t5- Volver al menu anterior");
    }else{
        printf("\n");
        printf("\n\t1- Modificar mi password");
        printf("\n\t2- Eliminar mi usuario");
        printf("\n\t3- Volver al menu anterior");
    }

    while(opcionValida != 1)
    {
        opcion = getch();

        switch(opcion) {

           case 49:
                opcionValida = 1;
                menuModificarPassword(usuaLogueado);
              break;

           case 50:
               opcionValida = 1;
              if(usuaLogueado.Tipo == TIPOADMIN)
               {
                    mostrarArchivoUsuarios(archivoUsuarios);///mostrar usuarios
                    getch();
                    menuGestionUsuarios(usuaLogueado);
               }else
               {
                   enDesarrollo(usuaLogueado);///eliminar mi usuario
               }
              break;

           case 51:
              if(usuaLogueado.Tipo == TIPOADMIN)
               {
                    opcionValida = 1;
                    mostrarUsuariosAdministradores(archivoUsuarios);
                    getch();
                    menuGestionUsuarios(usuaLogueado);
               }else
               {
                   menuPrincipal(usuaLogueado);
               }
               
              break;
           case 52:
              if(usuaLogueado.Tipo == TIPOADMIN)
               {
                    opcionValida = 1;
                    menuEliminarUsuarios(usuaLogueado);
               }
              break;
            case 53:
                menuPrincipal(usuaLogueado);
            break;
           default :
            printf("OPCION INVALIDA");
        }
    }
}

void showTituloProductos()
{
    printf("######   ##    ####  ##### #    # #####    ##    ####  #  ####  #    # \n#       #  #  #    #   #   #    # #    #  #  #  #    # # #    # ##   # \n#####  #    # #        #   #    # #    # #    # #      # #    # # #  # \n#      ###### #        #   #    # #####  ###### #      # #    # #  # # \n#      #    # #    #   #   #    # #   #  #    # #    # # #    # #   ## \n#      #    #  ####    #    ####  #    # #    #  ####  #  ####  #    #");
}

void menuGestionProductos(Usuario usuaLogueado)
{
    int opcion;

    system("cls");
    showTituloProductos();
    printf("\n\nElija como quiere buscar el producto:\n 1. BUSCAR POR NOMBRE  DEL PRODUCTO.\n 2. BUSCAR POR RUBRO.\n 3. BUSCAR POR PRECIO DEL PRODUCTO.\n 4. CARGAR PRODUCTOS \n 5. VOLVER AL MENU ANTERIOR\n");
     scanf("%d", &opcion);

     switch (opcion){
         case 1 : 
         MostrarUnProducto(buscarProductoPorNombre ());
         getch();
         menuGestionProductos(usuaLogueado);
            break;
         case 2 : 
            buscarProductoPorRubro();
            getch();
            menuGestionProductos(usuaLogueado);
            break;
         case 3 : 
            buscarProductoPorPrecio();
            getch();
            menuGestionProductos(usuaLogueado);
            break;
         case 4:
            CargarProductos();
            getch();
            menuGestionProductos(usuaLogueado);
         case 5 : 
            menuPrincipal(usuaLogueado);
            break;
         default : printf("\n NUMERO INCORRECTO, POR FAVOR SELECCIONE UNA OPCION SEGUN EL NUMERO QUE CORRESPONDA.");
            break;
     }
}

void menuGestionProductosAdmin(Usuario usuaLogueado)
{
    int opcion;
    if(usuaLogueado.Tipo == TIPOADMIN)
    {
        
    }else
    {
        printf("\n\nElija como quiere buscar el producto:\n 1. BUSCAR POR NOMBRE  DEL PRODUCTO.\n 2. BUSCAR POR RUBRO.\n 3. BUSCAR POR PRECIO DEL PRODUCTO.\n 4. VOLVER AL MENU PRINCIPAL\n5. VOLVER AL MENU ANTERIOR\n");
     scanf("%d", &opcion);

     switch (opcion){
         case 1 : MostrarUnProducto(buscarProductoPorNombre ());
            break;
         case 2 : buscarProductoPorRubro();
            break;
         case 3 : buscarProductoPorPrecio();
            break;
        case 4: menuPrincipal(usuaLogueado);
        break;
        case 5 : menuPrincipal(usuaLogueado);
        break;
         default : printf("\n NUMERO INCORRECTO, POR FAVOR SELECCIONE UNA OPCIÓN SEGÚN EL NUMERO QUE CORRESPONDA.");
            break;
     }
    }
}

void menuModificarPassword(Usuario usuaLogueado)
{
    char newPass[30];
    char newPassAux[30];
    char passActual[30];
    int valido = 0;

    while(valido != 1)
        {

        while(valido != 1)
        {
            system("cls");
            printf("\n\t\t<<<<<<<<<<Modificacion de Password>>>>>>>>>>>");
            printf("\nIngrese su password actual......:   ");
            fflush(stdin);
            scanf("%s", passActual);
            if(strcmp(strlwr(passActual),strlwr(usuaLogueado.Contra)) != 0)
            {
                system("cls");
                printf("\a");
                gotoxy(15,5);
                printf("Ese no es su password !!");
                getch();
                gotoxy(0,0);
                system("cls");
            }else
            {
                valido = 1;
            }
        }
        valido = 0;
        printf("\nIngrese su nuevo password........:   ");
        fflush(stdin);
        scanf("%s", newPass);
        printf("Repita su nuevo password...........:   ");
        scanf("%s", newPassAux);
        if(strcmp(strlwr(newPass), strlwr(newPassAux)) == 0)
        {
            valido = 1;

            modificarPassword(usuaLogueado, newPass);

            system("cls");
            printf("\a");
            gotoxy(15,5);
            printf("SU CONTRASEÑA HA SIDO CAMBIADA ! :)");
            getch();
            gotoxy(0,0);
            system("cls");
        }else
        {
            system("cls");
            printf("\a");
            gotoxy(15,5);
            printf("DATOS INCORRECTOS !");
            getch();
            gotoxy(0,0);
            system("cls");
        }
    }
    menuLogin();
}

void eliminacionDeUsuario(Usuario usuaLogueado)
{
    Usuario usua;
    int flag = -1;
    FILE *pArch;
    char opcion;
    char nombreUsua[30];
    system("cls");
    printf("\n\t\t<<<<<<<<<ELIMINACION DE USUARIO>>>>>>>>>");
    printf("Introduzca el nombre del usuario que desea eliminar");
    fflush(stdin);
    gets(nombreUsua);
    usua = getUsuarioByNombre(nombreUsua, archivoUsuarios);

    printf("\n\n\t\tESTA SEGURO DE ELIMINAR EL SIGUIENTE USUARIO ?? S/N \n");
    mostrarUnUsuario(usua);
    fflush(stdin);
    scanf("%c", &opcion);
    if((opcion == 'S') || (opcion == 's'))
    {
        flag = eliminarUsuarioById(usua.Id);
        if(flag == 1)
        {
            system("cls");
            printf("\a");
            gotoxy(15,5);
            printf("\nEliminacion exitosa.");
            getch();
            gotoxy(0,0);
            system("cls");
            menuLogin();
        }else
        {

            system("cls");
            printf("\a");
            gotoxy(15,5);
            printf("\nNo se pudo completar la operacion.");
            getch();
            gotoxy(0,0);
            system("cls");
            menuLogin();
        }
    }else
    {
        menuGestionUsuarios(usuaLogueado);
    }
    
}


void menuEliminarUsuarios(Usuario usuaLogueado)
{

    int opcion;
    int opcionValida = 0;
    system("cls");

    if(usuaLogueado.Tipo == TIPOADMIN )
    {
        printf("\n\t\t<<<<<<<<<ELIMINACION DE USUARIOS>>>>>>>>>");
        printf("\n1-Eliminar usuario por Id");
        printf("\n2-Eliminar usuario por Nombre");
        printf("\n3-Eliminar mi usuario");
        printf("\n4-Listar usuarios por nombre");
        printf("\n5-Listar usuarios por ID");
    }else
    {
        printf("\n\t\t<<<<<<<<<ELIMINACION DE USUARIOS>>>>>>>>>");
        printf("\n1-Eliminar mi usuario");
    }

    while(opcionValida != 1)
    {
        opcion = getch();

        switch(opcion) {

            case 49:
                opcionValida = 1;
                if(usuaLogueado.Tipo == TIPOADMIN)
               {
                    eliminacionDeUsuario(usuaLogueado); ///cambiar por id
               }else
               {
                   eliminarMiUsuario(usuaLogueado);
               }
              break;

            case 50:
              if(usuaLogueado.Tipo == TIPOADMIN)
               {
                    opcionValida = 1;
                    eliminacionDeUsuario(usuaLogueado);///ELIMINAR USUARIO POR NOMBRE
               }
              break;

            case 51:
              if(usuaLogueado.Tipo == TIPOADMIN)
               {
                    opcionValida = 1;
                    eliminarMiUsuario(usuaLogueado);
               }
              break;
            case 52:
              if(usuaLogueado.Tipo == TIPOADMIN)
               {
                    opcionValida = 1;
                    menuEliminarUsuarios(usuaLogueado);///LISTAR USUARIOS POR NOMBRE
               }
              break;
            case 53:
              if(usuaLogueado.Tipo == TIPOADMIN)
               {
                    opcionValida = 1;
                    menuEliminarUsuarios(usuaLogueado);///LISTAR USUARIOS POR ID
               }
              break;

           default :
            printf("OPCION INVALIDA");
        }
    }
}

int contarRegistrosUsuarios(char rutaArchivo[])
{
    int bytes;
    int cant;
    FILE *pArch = fopen(rutaArchivo, "rb");
    if(pArch != NULL)
    {
        fseek(pArch, 1, SEEK_END);
        bytes = ftell(pArch);
        cant = bytes/sizeof(Usuario);
        fclose(pArch);
    }
    return cant;
}

int contarRegistrosProductos(char rutaArchivo[])
{
    int bytes;
    int cant;
    FILE *pArch = fopen(rutaArchivo, "rb");
    if(pArch != NULL)
    {
        fseek(pArch, 1, SEEK_END);
        bytes = ftell(pArch);
        cant = bytes/sizeof(Producto);
        fclose(pArch);
    }
    return cant;
}


void crearFactura(Usuario usuaLogueado)
{   

    

    int prodId;
    Factura factuAux;
    int opcion = 0;
    int valido = 0;

    factuAux.Validos = 0;
    factuAux.VendedorId = usuaLogueado.Id;
    factuAux.NumeroFactura = getUltimoIdFactura()+1;

    while(opcion != ESC)
    {
        while(valido != 1)
        {
            system("cls");
            showTituloProductos();
            printf("\nIngrese el ID del producto");
            scanf("%d", &prodId);
            valido = checkExisteProductoId(prodId);
            if(valido == 0)
            {
                system("cls");
                printf("\a");
                gotoxy(15,5);
                printf("\nESE PRODUCTO NO EXISTE");
                getch();
                gotoxy(0,0);
                system("cls");
            }
        }
        
        factuAux.Productos[factuAux.Validos] = buscarProductoPorId(prodId);
        
        printf("El siguiente producto fue agregado");
        MostrarUnProducto(factuAux.Productos[factuAux.Validos]);
        factuAux.Validos++;
        printf("\n\t\t ESC PARA SALIR");
        opcion = getch();
        valido = 0;
    }
    

    factuAux.PrecioTotal = calculaPrecioTotal(factuAux.Productos, factuAux.Validos);

    guardaUnaFactura(factuAux);

}


float calculaPrecioTotal(Producto productos[], int validos)
{
    float total = 0;
    int i;
    for (i = 0; i < validos; i++)
    {
        total = total + productos[i].Precio;
    }
    
    return total;
}

void guardaUnaFactura(Factura factu)
{
    FILE *pArch = fopen(archivoVentas, "a+b");
    if(pArch != NULL)
    {
        fwrite(&factu, sizeof(Factura), 1, pArch);
        fclose(pArch);
    }
}


int checkExisteProductoId(int idBuscado)
{
    Producto prod;
    int flag = -1; /// -1 error no abrio archivo, 0 no encontro ID, 1 encontro ID
    FILE *pArch = fopen(archivoProductos, "rb");

    if(pArch != NULL)
    {
        while(fread(&prod, sizeof(Producto), 1, pArch) > 0)
        {
            if((prod.Id == idBuscado) && (prod.Activo == 1))
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


int getUltimoIdFactura()
{
    int mayor = 0;
    FILE *archi;
    archi = fopen(archivoVentas, "rb");
    Factura factuAux;

    if(archi != NULL)
    {
        while(!feof(archi)) ///Se cambio de fread > 0 a !feof porque no funcionaba
        {
            fread(&factuAux,sizeof(Usuario),1,archi);
            if(!feof(archi))
            {
                if(factuAux.NumeroFactura > mayor)
                    mayor = factuAux.NumeroFactura;
            }
        }
        fclose(archi);
    }
    return mayor;
}

void mostrarArchivoVentas()
{
    FILE *archi;
    archi = fopen(archivoVentas, "rb");
    Factura factuAux;

    if(archi != NULL)
    {
        while(fread(&factuAux, sizeof(Factura),1,archi)>0)
        {


            mostrarUnaFactura(factuAux);

        }
        fclose(archi);
    }
}

void mostrarUnaFactura(Factura factu)
{
    Usuario usua;
    usua = getUsuarioById(factu.VendedorId, archivoUsuarios);
    system("cls");
    puts("_____________________");
    printf("\nFACTURA N:   %d", factu.NumeroFactura);
    printf("\nMONTO TOTAL: %.2f", factu.PrecioTotal);
    printf("\nVENDEDOR:  %s", usua.Nombre);
    printf("\n-----PRODUCTOS VENDIDOS-----\n");
    int i;
    for(i = 0; i < factu.Validos; i++)
    {
        MostrarUnProducto(factu.Productos[i]);
    }
}

void mostrarArchivoFacturas()
{
    Factura factuAux;
    FILE *pArchi = fopen(archivoVentas, "rb");
    if(pArchi != NULL)
    {
        while(fread(&factuAux, sizeof(Factura),1,pArchi)>0)
        {
            
            mostrarUnaFactura(factuAux);
            getch();
        }
    fclose(pArchi);
    }

}