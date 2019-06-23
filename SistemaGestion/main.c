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

///Eliminar usuarios
void interfazEliminarUsuarioById(Usuario usuaLogueado);
void menuEliminarUsuarios(Usuario usuaLogueado);
void eliminarMiUsuario(Usuario usuaLogueado);


int main()
{
    Usuario usuarioLogueado;

    mostrarArchivoUsuarios(archivoUsuarios);
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
            menuPrincipal(usua);
        }
    }
}

void menuLogin()
{
    int opcion;
    int opcionValida = 0;
    ///system("cls");
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
                color(4);
                printf("OPCION INVALIDA");
                getch();
                gotoxy(0,0);
                system("cls");
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
        printf("\n\t3- Facturar\n ");
    }else{
        printf("\n");
        printf("\n\t1- Facturar");
        printf("\n\t2- Listar productos");
        printf("\n\t3- Cambiar password\n ");
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
                    enDesarrollo(usuaLogueado);///gestionar productos
               }else
               {
                   enDesarrollo(usuaLogueado);///Facturar
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
                    enDesarrollo(usuaLogueado);///facturar
               }else
               {
                   menuModificarPassword(usuaLogueado);
               }
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
    }else{
        printf("\n");
        printf("\n\t1- Modificar mi password");
        printf("\n\t2- Eliminar mi usuario");
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
                    enDesarrollo(usuaLogueado);///mostrar usuarios
               }else
               {
                   enDesarrollo(usuaLogueado);///eliminar mi usuario
               }
              break;

           case 51:
              if(usuaLogueado.Tipo == TIPOADMIN)
               {
                    opcionValida = 1;
                    enDesarrollo(usuaLogueado);
               }
              break;
           case 52:
              if(usuaLogueado.Tipo == TIPOADMIN)
               {
                    opcionValida = 1;
                    menuEliminarUsuarios(usuaLogueado);
               }
              break;

           default :
            printf("OPCION INVALIDA");
        }
    }
}

void modificarPassword(Usuario usuaLogueado, char newPass[30])
{
    Usuario usuaAux;
    FILE *pArch= fopen(archivoUsuarios, "r+b");
    if(pArch != NULL)
    {
        while(fread(&usuaAux, sizeof(Usuario), 1, pArch) > 0)
        {
            if(usuaAux.Id == usuaLogueado.Id)
            {
                strcpy(usuaLogueado.Contra, newPass);
                fseek(pArch,sizeof(Usuario) * (-1), SEEK_CUR);
                fwrite(&usuaLogueado,sizeof(Usuario), 1, pArch);
            }
        }
    fclose(pArch);
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
    int checkExist = 2;
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
        checkExist = checkExisteUsuarioNombre(nombreAux, archivoUsuarios);
        printf("CHECKEXIST ----->>>>> %d", checkExist);
        system("pause");
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

void interfazEliminarUsuarioById(Usuario usuaLogueado)
{
    int usuaId = -1;
    int valido = 0;
    Usuario usuaAux;
    char opcion;
    int flag = 0;

    while(valido == 0)
    {
        system("cls");
        printf("\nIntroduzca el ID del usuario a ELIMINAR");
        scanf("%d", &usuaId);
        if(getUltimoIdUsuario(archivoUsuarios)<= usuaId)
        {
            if(checkExisteUsuarioId(usuaId, archivoUsuarios) == 0)
            {
                valido = 1;
                usuaAux = getUsuarioById(usuaId, archivoUsuarios);
                printf("\nEsta seguro que desea eliminar al siguiente usuario ?");
                mostrarUnUsuario(usuaAux);
                printf("\nS/N");
                fflush(stdin);
                scanf("%c", opcion);
                if((opcion == 'S') || (opcion == 's'))
                {
                    flag = eliminarUsuarioById(usuaId);
                    if(flag == 1)
                    {
                        system("cls");
                        printf("\a");
                        gotoxy(15,5);
                        printf("\nEliminacion exitosa.");
                        getch();
                        gotoxy(0,0);
                        system("cls");
                        menuGestionUsuarios(usuaLogueado);
                    }else
                    {
                        
                        system("cls");
                        printf("\a");
                        gotoxy(15,5);
                        printf("\nNo se pudo completar la operacion.");
                        getch();
                        gotoxy(0,0);
                        system("cls");
                        menuGestionUsuarios(usuaLogueado);
                    }
                }

            }else
            {
                if(checkExisteUsuarioId(usuaId, archivoUsuarios) == -1)
                {
                    printf("MENOS UNOOOO");
                    getch();
                }
                system("cls");
                printf("\a");
                gotoxy(15,5);
                printf("\nEl usuario no existe !!");
                getch();
                gotoxy(0,0);
                system("cls");
            }
        }
    }
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
                    usua.Activo = 0;
                    fseek(pArch, sizeof(Usuario)*(-1), SEEK_CUR);
                    fwrite(&usua, sizeof(Usuario), 1, pArch);
                }

            }
        }
        fclose(pArch);
    }
   return flag;
}

void eliminarMiUsuario(Usuario usuaLogueado)
{
    Usuario usua;
    int flag = 0;
    FILE *pArch;
    char opcion;
    printf("cls");
    printf("\nESTA SEGURO DE ELIMINAR SU USUARIO ??");
    fflush(stdin);
    scanf("%c", opcion);
    if((opcion == 'S') || (opcion == 's'))
    {
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
                    interfazEliminarUsuarioById(usuaLogueado);
               }else
               {
                   eliminarMiUsuario(usuaLogueado);
               }
              break;

            case 50:
               
              if(usuaLogueado.Tipo == TIPOADMIN)
               {
                    opcionValida = 1;
                    enDesarrollo(usuaLogueado);///ELIMINAR USUARIO POR NOMBRE
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