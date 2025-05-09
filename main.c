#include "ATM.h"

int main() {
    cargarClientes();

    printf("===== Seleccione su numero de cliente (0 a %d) =====\n", NUM_CLIENTES - 1);
    printf("Numero de cliente: ");
    scanf("%d", &cuentaSeleccionada);

    if (cuentaSeleccionada < 0 || cuentaSeleccionada >= NUM_CLIENTES) {
        printf("Cliente no valido. Saliendo del programa.\n");
        return 0;
    }

    if (!verificarNIP()) {
        return 0;
    }

    int opcion;
    do {
        printf("\n========== MENU DE CAJERO ==========\n");
        printf("1. Consultar saldo\n");
        printf("2. Depositar dinero\n");
        printf("3. Retirar dinero\n");
        printf("4. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                consultarSaldo();
                break;
            case 2:
                depositar();
                break;
            case 3:
                retirar();
                break;
            case 4:
                guardarClientes();
                printf("\nGracias por usar el cajero. Se va por la sombrita.\n");
                break;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 4);

    return 0;
}
