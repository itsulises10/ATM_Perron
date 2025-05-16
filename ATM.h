#ifndef ATM_H_INCLUDED
#define ATM_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLIENTES 100

typedef struct {
    int numClientes;
    char nombre[50];
    float saldo;
    int pin;
    float historico;
} Cliente;

// Verificar monto positivo
    int verificacion(float X) {
    if (X > 0) return 1;
    if (X == 0) {
        printf("ERROR: La cantidad es 0.\n");
        return 0;
    }
    printf("ERROR: Cantidad negativa. Ingresa un numero positivo.\n");
    return 0;
}

// Preguntar si desea otra operacion
static int OtraOperacion(void) {
    int opcion;
    printf("Hay algo mas que podamos hacer por ti?\n");
    printf("1. Si, volver al menu\n");
    printf("2. No, salir\n");
    scanf("%d", &opcion);
    return (opcion == 1);
}

// Verificar PIN
static int PIN(int pinCorrecto) {
    int pinIngresado;
    for (int intento = 0; intento < 3; intento++) {
        printf("Ingresa tu PIN:\n");
        scanf("%d", &pinIngresado);
        if (pinIngresado == pinCorrecto) return 1;
        printf("PIN incorrecto. Intentos restantes: %d\n", 2 - intento);
    }
    printf("Limite de intentos superado.\n");
    exit(0);
}

// Limpiar pantalla
static void LimpiarPantalla(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Consultar saldo
static int ConsultarSaldo(float saldo) {
    printf("Saldo actual: $%.2f\n", saldo);
    return OtraOperacion();
}

// Depositar dinero
 int Depositardinero(float *saldo) {
    float cantidad;
    printf("Monto a depositar: $");
    scanf("%f", &cantidad);
    while (!verificacion(cantidad)) scanf("%f", &cantidad);
    *saldo += cantidad;
    printf("Nuevo saldo: $%.2f\n", *saldo);
    return OtraOperacion();
}

// Retirar dinero
static int RetirarDinero(float *saldo) {
    float cantidad;
    printf("Monto a retirar: $");
    scanf("%f", &cantidad);
    while (!verificacion(cantidad) || cantidad > *saldo) {
        if (cantidad > *saldo) printf("No puedes retirar mas de tu saldo ($%.2f).\n", *saldo);
        printf("Intenta de nuevo: $");
        scanf("%f", &cantidad);
    }
    *saldo -= cantidad;
    printf("Retiro exitoso. Saldo: $%.2f\n", *saldo);
    return OtraOperacion();
}

// Buscar cliente por nombre
static int Buscar(Cliente clientes[], int numClientes, const char *nombreBuscado) {
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].nombre, nombreBuscado) == 0) return i;
    }
    return -1;
}

// Crear nueva cuenta
static int NuevaCuenta(Cliente clientes[], int *numClientes) {
    if (*numClientes >= MAX_CLIENTES) {
        printf("Limite de cuentas alcanzado.\n");
        return 0;
    }
    char nombre[50];
    int pin, confPin;
    float deposito;
    getchar();
    printf("Ingresa Nombre y Apellido:\n");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';
    do {
        printf("Crea un NIP de 4 digitos: "); scanf("%d", &pin);
        printf("Confirmar NIP: "); scanf("%d", &confPin);
        if (pin != confPin) printf("NIPs no coinciden.\n");
    } while (pin != confPin);
    do {
        printf("Deposito minimo $300: $"); scanf("%f", &deposito);
    } while (deposito < 300);
    Cliente c = {*numClientes + 1, "", deposito, pin};
    strncpy(c.nombre, nombre, sizeof(c.nombre)-1);
    clientes[*numClientes] = c;
    printf("Cuenta creada. Numero: %03d\n", c.numClientes);
    (*numClientes)++;
    return OtraOperacion();
}

// Persistencia e historico en un solo CSV
// Lineas con 4 campos: id,nombre,saldo,pin => clientes
// Lineas con 3 campos: id,tipo(+/-),monto => historico
static int CargarClientes(Cliente clientes[], int *numClientes, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;
    int count = 0;
    char line[128];
    while (count < MAX_CLIENTES && fgets(line, sizeof(line), f)) {
        int commas = 0;
        for (char *p = line; *p; p++) if (*p == ',') commas++;
        if (commas == 3) {
            sscanf(line, "%d,%49[^,],%f,%d", &clientes[count].numClientes,
                   clientes[count].nombre,
                   &clientes[count].saldo,
                   &clientes[count].pin);
            count++;
        }
    }
    fclose(f);
    *numClientes = count;
    return 1;
}

static int GuardarClientes(Cliente clientes[], int numClientes, const char *filename) {
    // Leer historico prev
    FILE *f = fopen(filename, "r");
    char *hist[1000]; int hcount = 0;
    char line[128];
    if (f) {
        while (fgets(line, sizeof(line), f)) {
            int commas = 0;
            for (char *p = line; *p; p++) if (*p == ',') commas++;
            if (commas == 2) hist[hcount++] = strdup(line);
        }
        fclose(f);
    }
    // Reescribir
    f = fopen(filename, "w");
    if (!f) return 0;
    for (int i = 0; i < numClientes; i++) {
        fprintf(f, "%d,%s,%.2f,%d\n",
                clientes[i].numClientes,
                clientes[i].nombre,
                clientes[i].saldo,
                clientes[i].pin);
    }
    for (int i = 0; i < hcount; i++) {
        fputs(hist[i], f);
        free(hist[i]);
    }
    fclose(f);
    return 1;
}

static void RegistrarHistorico(int numCliente, float monto, char tipo, const char *filename) {
    FILE *f = fopen(filename, "a"); if (!f) return;
    fprintf(f, "%d,%c,%.2f\n", numCliente, tipo, monto);
    fclose(f);
}

static void MostrarHistorico(const char *filename, int numCliente) {
    FILE *f = fopen(filename, "r");
    if (!f) { printf("No se puede abrir archivo.\n"); return; }
    char line[128];
    printf("Historico cuenta %d:\n", numCliente);
    while (fgets(line, sizeof(line), f)) {
        int commas = 0; for (char *p=line;*p;p++) if (*p==',') commas++;
        if (commas==2) {
            int id; char tipo; float monto;
            sscanf(line, "%d,%c,%f", &id, &tipo, &monto);
            if (id==numCliente) printf("%c %.2f\n", tipo, monto);
        }
    }
    fclose(f);
}

#endif // ATM_H_INCLUDED
