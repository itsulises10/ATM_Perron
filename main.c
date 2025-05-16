#include "ATM.h"

int main(void) {
    Cliente clientes[MAX_CLIENTES];
    int numClientes = 0;
    const char *dataFile = "clientes.csv";

    CargarClientes(clientes, &numClientes, dataFile);

    int opcionPrincipal, opcion, seguir = 1, cuentaSeleccionada, clienteExiste;

    do {
        printf("====== MENU PRINCIPAL ======\n");
        printf("1. Iniciar sesion (cuenta existente)\n");
        printf("2. Crear nueva cuenta\n");
        printf("3. Buscar numero de cuenta por nombre\n");
        printf("4. Salir\n");
        printf("Selecciona una opcion: ");
        scanf("%d", &opcionPrincipal);
        LimpiarPantalla();

        switch (opcionPrincipal) {
            case 1:
                printf("Ingresa tu numero de cliente: ");
                scanf("%d", &cuentaSeleccionada);
                clienteExiste = 0;
                for (int i = 0; i < numClientes; i++) {
                    if (clientes[i].numClientes == cuentaSeleccionada) {
                        clienteExiste = 1;
                        seguir = PIN(clientes[i].pin);
                        if (seguir) printf("Has seleccionado %s\n", clientes[i].nombre);
                        break;
                    }
                }
                if (!clienteExiste) {
                    printf("No existe una cuenta con ese numero de cliente.\n");
                    seguir = 0;
                }
                if (seguir) {
                    do {
                        printf("Bienvenido a mi cajero\n\n");
                        printf("1. Consultar Saldo\n");
                        printf("2. Depositar dinero\n");
                        printf("3. Retirar Dinero\n");
                        printf("4. Consultar Historico\n");
                        printf("5. Cerrar sesion\n");
                        printf("Porfavor selecciona una opcion\n");
                        scanf("%d", &opcion);
                        LimpiarPantalla();

                        switch (opcion) {
                            case 1:
                                seguir = ConsultarSaldo(clientes[cuentaSeleccionada - 1].saldo);
                                break;
                            case 2:
                                if (Depositardinero(&clientes[cuentaSeleccionada - 1].saldo)) {
                                    RegistrarHistorico(cuentaSeleccionada,
                                        clientes[cuentaSeleccionada - 1].saldo, '+', dataFile);
                                    GuardarClientes(clientes, numClientes, dataFile);
                                    seguir = 1;
                                } else seguir = 0;
                                break;
                            case 3:
                                if (RetirarDinero(&clientes[cuentaSeleccionada - 1].saldo)) {
                                    RegistrarHistorico(cuentaSeleccionada,
                                        clientes[cuentaSeleccionada - 1].saldo, '-', dataFile);
                                    GuardarClientes(clientes, numClientes, dataFile);
                                    seguir = 1;
                                } else seguir = 0;
                                break;
                            case 4:
                                MostrarHistorico(dataFile, cuentaSeleccionada);
                                seguir = 1;
                                break;
                            case 5:
                                printf("Sesion cerrada.\n");
                                seguir = 0;
                                break;
                            default:
                                printf("Opcion no valida.\n");
                        }
                    } while (seguir);
                }
                break;
            case 2:
                if (NuevaCuenta(clientes, &numClientes))
                    GuardarClientes(clientes, numClientes, dataFile);
                break;
            case 3: {
                char nombre[50];
                getchar();
                printf("Ingresa el nombre del cliente: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre,"\n")] = '\0';
                int idx = Buscar(clientes, numClientes, nombre);
                if (idx < 0) printf("No se encontro un cliente con ese nombre.\n");
                else printf("El numero de cuenta de %s es: %03d\n",
                         clientes[idx].nombre, clientes[idx].numClientes);
            } break;
            case 4:
                printf("Gracias por usar mi cajero.\n");
                printf("Que tengas un buen dia.\n");
                exit(0);
            default:
                printf("Opcion no valida.\n");
        }
    } while (1);

    return 0;
}
