#ifndef ATM_H_INCLUDED
#define ATM_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_INTENTOS 3
#define NUM_CLIENTES 3

typedef struct {
    int numeroCliente;
    char nombre[50];
    char nip[5]; // 4 digitos + '\0'
    float saldo;
} Cliente;

Cliente clientes[NUM_CLIENTES];
int cuentaSeleccionada;

// Validar que el NIP tenga exactamente 4 digitos numericos
int nipValido(const char *nip) {
    if (strlen(nip) != 4) return 0;
    for (int i = 0; i < 4; i++) {
        if (!isdigit(nip[i])) return 0;
    }
    return 1;
}

// Verifica si una cantidad es valida
int verifica(float x) {
    if (x < 0) {
        printf("La cantidad no es valida. Debe ser mayor a 0.\n");
        return 0;
    }
    return 1;
}

// Mostrar el saldo actual
void consultarSaldo() {
    printf("Saldo de %s (Cliente #%d): $%.2f\n",
        clientes[cuentaSeleccionada].nombre,
        clientes[cuentaSeleccionada].numeroCliente,
        clientes[cuentaSeleccionada].saldo);
}

// Depositar dinero
void depositar() {
    float cantidad;
    printf("Ingrese la cantidad a depositar: ");
    scanf("%f", &cantidad);

    if (verifica(cantidad)) {
        clientes[cuentaSeleccionada].saldo += cantidad;
        printf("Deposito exitoso. Nuevo saldo: $%.2f\n", clientes[cuentaSeleccionada].saldo);
    } else {
        printf("No se pudo realizar el deposito.\n");
    }
}

// Retirar dinero
void retirar() {
    float cantidad;
    printf("Ingrese la cantidad a retirar: ");
    scanf("%f", &cantidad);

    if (verifica(cantidad)) {
        if (cantidad > clientes[cuentaSeleccionada].saldo) {
            printf("Fondos insuficientes. Saldo actual: $%.2f\n", clientes[cuentaSeleccionada].saldo);
        } else {
            clientes[cuentaSeleccionada].saldo -= cantidad;
            printf("Retiro exitoso. Nuevo saldo: $%.2f\n", clientes[cuentaSeleccionada].saldo);
        }
    } else {
        printf("No se pudo realizar el retiro.\n");
    }
}

// Guardar clientes en archivo
void guardarClientes() {
    FILE *archivo = fopen("clientes.txt", "w");
    if (archivo == NULL) {
        printf("Error al guardar los datos.\n");
        return;
    }

    for (int i = 0; i < NUM_CLIENTES; i++) {
        fprintf(archivo, "%d %s %s %.2f\n",
            clientes[i].numeroCliente,
            clientes[i].nombre,
            clientes[i].nip,
            clientes[i].saldo);
    }

    fclose(archivo);
}

// Cargar clientes o crearlos si no existen
void cargarClientes() {
    FILE *archivo = fopen("clientes.txt", "r");
    if (archivo == NULL) {
        printf("Archivo de clientes no encontrado. Creando datos iniciales...\n");

        // Crear datos iniciales si el archivo no existe
        for (int i = 0; i < NUM_CLIENTES; i++) {
            clientes[i].numeroCliente = i;

            printf("Ingrese el nombre del cliente #%d: ", i);
            scanf("%s", clientes[i].nombre);

            // Pedir y validar el NIP
            do {
                printf("Cree un NIP de 4 digitos para %s: ", clientes[i].nombre);
                scanf("%s", clientes[i].nip);

                if (!nipValido(clientes[i].nip)) {
                    printf("NIP invalido. Debe tener exactamente 4 digitos numericos.\n");
                }

            } while (!nipValido(clientes[i].nip));

            clientes[i].saldo = 0.0;
        }

        guardarClientes();
    } else {
        for (int i = 0; i < NUM_CLIENTES; i++) {
            fscanf(archivo, "%d %s %s %f",
                &clientes[i].numeroCliente,
                clientes[i].nombre,
                clientes[i].nip,
                &clientes[i].saldo);
        }
        fclose(archivo);
    }
}

// Funcion para cargar el NIP desde archivo o pedir uno nuevo si esta vacio
void cargarNIP() {
    FILE *archivo = fopen("nip.txt", "r");
    if (archivo == NULL) {
        // Si el archivo no existe, lo creamos
        archivo = fopen("nip.txt", "w");
        if (archivo == NULL) {
            printf("Error al crear archivo para NIP.\n");
            return;
        }
        fclose(archivo);
    }

    // Cargar el NIP desde el archivo
    archivo = fopen("nip.txt", "r");
    char nipExistente[5];
    fscanf(archivo, "%s", nipExistente);

    if (strcmp(nipExistente, "0000") == 0 || strlen(nipExistente) == 0) {
        // Si el NIP esta vacio o es "0000", pedimos un nuevo NIP
        char nuevoNIP[5];
        printf("No se encontro un NIP valido. Ingrese un nuevo NIP de 4 digitos: ");
        scanf("%s", nuevoNIP);

        // Validar que el nuevo NIP sea valido
        while (!nipValido(nuevoNIP)) {
            printf("NIP invalido. Debe tener exactamente 4 digitos numericos.\n");
            printf("Ingrese un nuevo NIP de 4 digitos: ");
            scanf("%s", nuevoNIP);
        }

        // Guardamos el nuevo NIP en el archivo
        archivo = fopen("nip.txt", "w");
        fprintf(archivo, "%s", nuevoNIP);
        fclose(archivo);
        printf("NIP registrado correctamente.\n");
    } else {
        // Si ya hay un NIP, lo usamos
        printf("Por favor ingrese su NIP: ");
        char nipIngresado[5];
        int intentos = 0;
        while (intentos < MAX_INTENTOS) {
            scanf("%s", nipIngresado);
            if (strcmp(nipIngresado, nipExistente) == 0) {
                printf("NIP correcto.\n");
                fclose(archivo);
                return;
            } else {
                intentos++;
                printf("NIP incorrecto. Intentos restantes: %d\n", MAX_INTENTOS - intentos);
            }
        }
        printf("Demasiados intentos fallidos. Saliendo del programa.\n");
        fclose(archivo);
        exit(0); // Finalizar el programa si se exceden los intentos
    }
}

// Verificar que el NIP ingresado coincida
int verificarNIP() {
    cargarNIP();  // Verificar si ya existe un NIP guardado o pedir uno nuevo
    return 1; // Si llegamos aqui, el NIP fue correcto
}

#endif // ATM_H_INCLUDED
