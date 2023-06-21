#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct
{
    char nombre[20];
    char descripcion[300];
    int precio;
    int stock;
} stProducto;

stProducto cargarProductos();
void cargarArchivos(char nombre[]);
void mostrarArchivo(char nombre[]);
void comprarProducto(char nombre[]);
void cobrarConTarjeta();

int main()
{
    system("color 3F");
    char nombre[20];
    strcpy(nombre, "productos.bin");
    cargarArchivos(nombre);

    mostrarArchivo(nombre);

    comprarProducto(nombre);
    return 0;
}

stProducto cargarProductos() /// PRINTFS PARA CARGA DE PRODUCTOS
{

    stProducto aux;

    printf("Ingrese el nombre del producto: \n");
    fflush(stdin);
    gets(aux.nombre);

    printf("Ingrese la descripcion del producto: \n");
    fflush(stdin);
    gets(aux.descripcion);

    printf("Ingrese el precio del producto: \n");
    scanf("%i", &aux.precio);

    printf("Ingrese la cantidad disponible del producto: \n");
    scanf("%i", &aux.stock);

    return aux;
}

void cargarArchivos(char nombre[]) /// CARGA DE PRODUCTOS EN EL ARCHIVO
{
    FILE * buffer = fopen(nombre, "ab");
    char control = 's';
    stProducto p;

    printf("Desea cargar productos? \n");
    fflush(stdin);
    scanf("%c", &control);

    if(buffer)
    {
        while(control == 's')
        {
            p = cargarProductos();
            fwrite(&p, sizeof(stProducto), 1, buffer);
            printf("Desea seguir cargando productos? \n");
            fflush(stdin);
            scanf("%c", &control);
        }
        fclose(buffer);
    }
}

void mostrarArchivo(char nombre[]) /// MUESTRA LOS PRODUCTOS
{
    FILE * buffer = fopen(nombre, "rb");
    stProducto p;
    printf("         PRODUCTOS:  \n \n");
    if(buffer)
    {
        while(fread(&p, sizeof(stProducto), 1, buffer) > 0)
        {
            printf("Nombre: %s \n", p.nombre);
            printf("Descripcion: %s \n", p.descripcion);
            printf("Precio: $%i \n", p.precio);
            printf("Cantidad disponible: %i \n", p.stock);
            printf("------------------------------------\n");
        }
        fclose(buffer);
    }
}

void comprarProducto(char nombre[]) /// COMPRA DE PRODUCTOS, VERIFICACION DE DATOS DE PAGO
{

    FILE * buffer = fopen(nombre, "r+b");
    stProducto comprar;
    char control = 's';
    char nombreComprar[20];
    int cantidadComprar = 0;
    int formaDePago = 0;
    printf("Desea comprar algun producto? \n");
    fflush(stdin);
    scanf("%c", &control);


    if(control == 's')
    {
        printf("Dime el nombre del producto que desea comprar: \n");
        fflush(stdin);
        gets(nombreComprar);
        int flag = 1;

        if(buffer)
        {
            while(flag == 1 && fread(&comprar, sizeof(stProducto), 1, buffer) > 0)
            {
                if(strcmpi(nombreComprar, comprar.nombre) == 0)
                {
                    printf("Dime la cantidad de ' %s ' que quieres comprar: \n", comprar.nombre);
                    scanf("%i", &cantidadComprar);
                    while(cantidadComprar > comprar.stock)
                    {
                        printf("Dime una cantidad valida de ' %s ' que quieres comprar: \n", comprar.nombre);
                        scanf("%i", &cantidadComprar);
                    }
                    comprar.stock = (comprar.stock - cantidadComprar);
                    fseek(buffer, sizeof(stProducto)*-1, 1);
                    fwrite(&comprar, sizeof(stProducto), 1, buffer);
                    flag = 0;

                    int total = cantidadComprar * comprar.precio;
                    system("cls");
                    printf("El total a abonar es: $%i \n", total);
                    printf("Como desea abonar su pago? \n Ingrese 1 para abonar de manera online. \n Ingrese 2 para abonar de manera presencial. \n");
                    scanf("%i", &formaDePago);
                    if(formaDePago == 1)
                    {
                        cobrarConTarjeta();

                        char control2 = 's';
                        printf("Confirmar compra? (s/n) \n");
                        scanf("%c", &control2);

                        if(control2 == 'S' || control2 == 's')
                        {
                            system("cls");
                            printf("COMPRA REALIZADA CON EXITO! \n");
                            printf("----------------------------\n");
                            printf("|   COMPROBANTE DE PAGO:   |\n");
                            printf("|   RETIRO EN: Mitre 3128   |\n");
                            printf("| PRODUCTO: %s           |\n", comprar.nombre);
                            printf("| TOTAL: %i              |\n", total);
                            printf("----------------------------");
                        }
                    }
                    else if(formaDePago == 2)
                    {
                        printf("PEDIDO REALIZADO CON EXITO! \n");
                        printf("----------------------------\n");
                        printf("|   RETIRO EN: Mitre 3128   |\n");
                        printf("| PRODUCTO: %s           |\n", comprar.nombre);
                        printf("| TOTAL: %i              |\n", total);
                        printf("----------------------------");
                    }

                }
            }
            fclose(buffer);
        }
    }
}

void cobrarConTarjeta() /// VERIFICACION DE LOS DATOS DE PAGO
{
    char tarjetaNum[30];
    char tarjetaTitular[30];
    char tarjetaCodigo[10];

    printf("Ingrese los datos de pago: \n");

    printf("Nombre y apellido del titular de la tarjeta: \n");
    fflush(stdin);
    gets(tarjetaTitular);

    while(strchr(tarjetaTitular, ' ') == NULL)
    {
        printf("Ingrese un nombre y apellido valido: \n");
        fflush(stdin);
        gets(tarjetaTitular);
    }
    printf("Numero de tarjeta: \n");
    fflush(stdin);
    gets(tarjetaNum);

    int verificacion = strlen(tarjetaNum);

    while(verificacion > 16 || verificacion < 16)
    {
        printf("Numero de tarjeta inexistente, revisa los datos y vuelve a ingresarlos. \n");
        fflush(stdin);
        gets(tarjetaNum);
        verificacion = strlen(tarjetaNum);
    }

    printf("Codigo de seguridad: \n");
    fflush(stdin);
    gets(tarjetaCodigo);

    int verificacion2 = strlen(tarjetaCodigo);

    while(verificacion2 > 3 || verificacion2 < 3)
    {
        printf("Codigo de seguridad invalido. Verifique el codigo y vuelva a ingresarlo. \n");
        fflush(stdin);
        gets(tarjetaCodigo);
        verificacion2 = strlen(tarjetaCodigo);
    }
}
