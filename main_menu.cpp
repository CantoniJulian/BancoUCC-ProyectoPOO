#include "headers.h"
#include <iostream>
#include <limits>
#include <algorithm>

using namespace std;

// Colores para la consola
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pausar() {
    cout << "\nPresione ENTER para continuar...";
    cin.get();
}

void mostrarEncabezado(string titulo) {
    cout << "\n" << CYAN << BOLD;
    cout << "+============================================================+\n";
    cout << "|  " << titulo;
    for(int i = titulo.length(); i < 56; i++) cout << " ";
    cout << "|\n";
    cout << "+============================================================+\n";
    cout << RESET;
}

void mostrarMenuPrincipal() {
    cout << "\n" << BOLD << BLUE;
    cout << "+============================================================+\n";
    cout << "|            SISTEMA DE GESTION BANCARIA UCC                |\n";
    cout << "+============================================================+\n";
    cout << RESET;
    cout << YELLOW << "\n[1]" << RESET << " Gestion de Clientes\n";
    cout << YELLOW << "[2]" << RESET << " Registro de Transacciones\n";
    cout << YELLOW << "[3]" << RESET << " Consultas y Reportes\n";
    cout << YELLOW << "[4]" << RESET << " Conversion de Monedas\n";
    cout << YELLOW << "[0]" << RESET << " Salir\n";
    cout << "\n" << CYAN << "Seleccione una opcion: " << RESET;
}

void menuGestionClientes(Banco* banco) {
    int opcion;
    do {
        mostrarEncabezado("GESTION DE CLIENTES");
        cout << YELLOW << "\n[1]" << RESET << " Alta de Cliente\n";
        cout << YELLOW << "[2]" << RESET << " Baja de Cliente (Cambiar estado a BAJA)\n";
        cout << YELLOW << "[3]" << RESET << " Buscar Cliente por ID\n";
        cout << YELLOW << "[4]" << RESET << " Listado de Todos los Clientes\n";
        cout << YELLOW << "[5]" << RESET << " Crear Cuenta para Cliente\n";
        cout << YELLOW << "[6]" << RESET << " Eliminar Cuenta de Cliente\n";
        cout << YELLOW << "[0]" << RESET << " Volver al Menu Principal\n";
        cout << "\n" << CYAN << "Seleccione una opcion: " << RESET;
        cin >> opcion;
        limpiarBuffer();
        switch(opcion) {
            case 1: {
                // Alta de cliente
                cout << "\n" << GREEN << "=== ALTA DE CLIENTE ===" << RESET << "\n";
                int dni;
                string nombre, tipoStr;
                
                cout << "DNI: ";
                cin >> dni;
                limpiarBuffer();
                
                cout << "Nombre completo: ";
                getline(cin, nombre);
                
                cout << "Tipo de Cliente (PLATA/ORO/PLATINO): ";
                getline(cin, tipoStr);
                
                transform(tipoStr.begin(), tipoStr.end(), tipoStr.begin(), ::toupper);
                
                if(tipoStr != "PLATA" && tipoStr != "ORO" && tipoStr != "PLATINO") {
                    cout << RED << "Tipo de cliente invalido!" << RESET << endl;
                    pausar();
                    break;
                }
                
                try {
                    // Crear cliente nuevo (fecha de ingreso automatica del sistema)
                    Cliente* nuevoCliente = new Cliente(to_string(dni), nombre, tipoStr);
                    if(banco->altaCliente(nuevoCliente)) {
                        cout << GREEN << "\n[OK] Cliente dado de alta exitosamente! ID: " << nuevoCliente->getID() << RESET << endl;
                        // Asignar tarjeta segun tipo (AUTOMATICO)
                        if(tipoStr == "ORO") {
                            Tarjeta* tarjeta = new Tarjeta("Credix", Moneda(250000, "ARS"));
                            nuevoCliente->setTarjeta(tarjeta);
                            banco->guardarTarjetas();
                            cout << GREEN << "[OK] Tarjeta Credix asignada automaticamente (Limite: $250,000)" << RESET << endl;
                        } else if(tipoStr == "PLATINO") {
                            Tarjeta* tarjeta = new Tarjeta("Premium", Moneda(500000, "ARS"));
                            nuevoCliente->setTarjeta(tarjeta);
                            banco->guardarTarjetas();
                            cout << GREEN << "[OK] Tarjeta Premium asignada automaticamente (Limite: $500,000)" << RESET << endl;
                        } else {
                            cout << YELLOW << "[INFO] Cliente PLATA no tiene acceso a tarjeta de credito" << RESET << endl;
                        }
                    } else {
                        cout << RED << "[ERROR] Error al dar de alta el cliente" << RESET << endl;
                    }
                } catch(exception& e) {
                    cout << RED << "Error: " << e.what() << RESET << endl;
                }
                pausar();
                break;
            }
            case 2: {
                // Baja de cliente
                cout << "\n" << YELLOW << "=== BAJA DE CLIENTE ===" << RESET << "\n";
                double id;
                cout << "ID del Cliente: ";
                cin >> id;
                limpiarBuffer();
                if(banco->bajaCliente(id)) {
                    cout << GREEN << "\n[OK] Cliente dado de baja exitosamente (Estado: BAJA)" << RESET << endl;
                } else {
                    cout << RED << "[ERROR] Error: Cliente no encontrado" << RESET << endl;
                }
                pausar();
                break;
            }
            case 3: {
                // Buscar cliente
                cout << "\n" << CYAN << "=== BUSCAR CLIENTE ===" << RESET << "\n";
                double id;
                cout << "ID del Cliente: ";
                cin >> id;
                limpiarBuffer();
                banco->buscarCliente(id);
                pausar();
                break;
            }
            case 4: {
                // Listado de Clientes
                banco->listadoClientes();
                pausar();
                break;
            }
            case 5: {
                // Crear cuenta para cliente
                cout << "\n" << GREEN << "=== CREAR CUENTA PARA CLIENTE ===" << RESET << "\n";
                double idCliente;
                string denominacion;
                
                cout << "ID del Cliente: ";
                cin >> idCliente;
                limpiarBuffer();
                
                cout << "Denominacion de la cuenta (ARS/USD/EUR): ";
                getline(cin, denominacion);
                
                // Convertir a mayusculas
                transform(denominacion.begin(), denominacion.end(), denominacion.begin(), ::toupper);
                
                try {
                    if (banco->crearCuentaParaCliente(idCliente, denominacion)) {
                        cout << GREEN << "\n[OK] Cuenta creada exitosamente!" << RESET << endl;
                        cout << "La cuenta ha sido asignada al cliente." << endl;
                    }
                } catch(exception& e) {
                    cout << RED << "Error: " << e.what() << RESET << endl;
                }
                pausar();
                break;
            }
            case 6: {
                // Eliminar cuenta de cliente
                cout << "\n" << YELLOW << "=== ELIMINAR CUENTA DE CLIENTE ===" << RESET << "\n";
                double idCliente;
                string cbu;
                
                cout << "ID del Cliente: ";
                cin >> idCliente;
                limpiarBuffer();
                
                cout << "CBU de la cuenta a eliminar: ";
                getline(cin, cbu);
                
                try {
                    if (banco->eliminarCuentaDeCliente(idCliente, cbu)) {
                        cout << GREEN << "\n[OK] Cuenta eliminada exitosamente!" << RESET << endl;
                    }
                } catch(exception& e) {
                    cout << RED << "Error: " << e.what() << RESET << endl;
                }
                pausar();
                break;
            }
            case 0:
                break;
            default:
                cout << RED << "Opción inválida" << RESET << endl;
                pausar();
        }
    } while(opcion != 0);
}

void menuTransacciones(Banco* banco) {
    int opcion;
    do {
        mostrarEncabezado("REGISTRO DE TRANSACCIONES");
        cout << YELLOW << "\n[1]" << RESET << " Realizar Deposito\n";
        cout << YELLOW << "[2]" << RESET << " Realizar Extraccion\n";
        cout << YELLOW << "[0]" << RESET << " Volver al Menu Principal\n";
        cout << "\n" << CYAN << "Seleccione una opcion: " << RESET;
        cin >> opcion;
        limpiarBuffer();
        switch(opcion) {
            case 1: {
                // Deposito
                cout << "\n" << GREEN << "=== DEPOSITO ===" << RESET << "\n";
                string cbu, denominacion;
                double monto, idCliente;
                
                cout << "Su ID de Cliente: ";
                cin >> idCliente;
                limpiarBuffer();
                
                cout << "CBU de la Cuenta: ";
                getline(cin, cbu);
                
                cout << "Monto a depositar: ";
                cin >> monto;
                limpiarBuffer();
                
                cout << "Denominacion (ARS/USD/EUR): ";
                getline(cin, denominacion);
                
                // Convertir denominacion a mayusculas
                transform(denominacion.begin(), denominacion.end(), denominacion.begin(), ::toupper);
                
                try {
                    banco->realizarDeposito(cbu, monto, denominacion, idCliente);
                    cout << GREEN << "\n[OK] Deposito realizado exitosamente!" << RESET << endl;
                    cout << "Saldo actualizado correctamente." << endl;
                } catch(exception& e) {
                    cout << RED << "Error: " << e.what() << RESET << endl;
                }
                pausar();
                break;
            }
            case 2: {
                // Extraccion
                cout << "\n" << YELLOW << "=== EXTRACCION ===" << RESET << "\n";
                string cbu, denominacion;
                double monto, idCliente;
                
                cout << "Su ID de Cliente: ";
                cin >> idCliente;
                limpiarBuffer();
                
                cout << "CBU de la Cuenta: ";
                getline(cin, cbu);
                
                cout << "Monto a extraer: ";
                cin >> monto;
                limpiarBuffer();
                
                cout << "Denominacion (ARS/USD/EUR): ";
                getline(cin, denominacion);
                
                // Convertir denominacion a mayusculas
                transform(denominacion.begin(), denominacion.end(), denominacion.begin(), ::toupper);
                
                try {
                    banco->realizarExtraccion(cbu, monto, denominacion, idCliente);
                    cout << GREEN << "\n[OK] Extraccion realizada exitosamente!" << RESET << endl;
                    cout << "Saldo actualizado correctamente." << endl;
                } catch(exception& e) {
                    cout << RED << "Error: " << e.what() << RESET << endl;
                }
                pausar();
                break;
            }
            case 0:
                break;
            default:
                cout << RED << "Opción inválida" << RESET << endl;
                pausar();
        }
    } while(opcion != 0);
}

void menuConsultas(Banco* banco) {
    int opcion;
    do {
        mostrarEncabezado("CONSULTAS Y REPORTES");
        cout << YELLOW << "\n[1]" << RESET << " Transacciones por Cliente\n";
        cout << YELLOW << "[2]" << RESET << " Informe de Transacciones por Periodo\n";
        cout << YELLOW << "[3]" << RESET << " Informe de Transacciones por Mes\n";
        cout << YELLOW << "[4]" << RESET << " Informe de Transacciones por Anio\n";
        cout << YELLOW << "[5]" << RESET << " Informe de Todas las Transacciones\n";
        cout << YELLOW << "[0]" << RESET << " Volver al Menu Principal\n";
        cout << "\n" << CYAN << "Seleccione una opcion: " << RESET;
        cin >> opcion;
        limpiarBuffer();
        switch(opcion) {
            case 1: {
                // Transacciones por cliente
                double id;
                cout << "\nID del Cliente: ";
                cin >> id;
                limpiarBuffer();
                banco->informeTransacciones(id);
                pausar();
                break;
            }
            case 2: {
                // Transacciones por período
                string fechaDesde, fechaHasta;
                cout << "\nFecha desde (DD/MM/AAAA): ";
                getline(cin, fechaDesde);
                cout << "Fecha hasta (DD/MM/AAAA): ";
                getline(cin, fechaHasta);
                try {
                    banco->informeTransacciones(Fecha(fechaDesde), Fecha(fechaHasta));
                } catch(exception& e) {
                    cout << RED << "Error: " << e.what() << RESET << endl;
                }
                pausar();
                break;
            }
            case 3: {
                // Transacciones por mes
                int mes, anio;
                cout << "\nMes (1-12): ";
                cin >> mes;
                cout << "Año: ";
                cin >> anio;
                limpiarBuffer();
                try {
                    Fecha desde(1, mes, anio);
                    Fecha hasta(31, mes, anio);
                    banco->informeTransacciones(desde, hasta);
                } catch(exception& e) {
                    cout << RED << "Error: " << e.what() << RESET << endl;
                }
                pausar();
                break;
            }
            case 4: {
                // Transacciones por año
                int anio;
                cout << "\nAño: ";
                cin >> anio;
                limpiarBuffer();
                try {
                    Fecha desde(1, 1, anio);
                    Fecha hasta(31, 12, anio);
                    banco->informeTransacciones(desde, hasta);
                } catch(exception& e) {
                    cout << RED << "Error: " << e.what() << RESET << endl;
                }
                pausar();
                break;
            }
            case 5: {
                // Todas las transacciones
                banco->informeTransacciones();
                pausar();
                break;
            }
            case 0:
                break;
            default:
                cout << RED << "Opción inválida" << RESET << endl;
                pausar();
        }
    } while(opcion != 0);
}

void menuConversion(Banco* banco) {
    mostrarEncabezado("CONVERSIÓN DE MONEDAS");
    double monto;
    string desde, hacia;
    cout << "\nMonto a convertir: ";
    cin >> monto;
    limpiarBuffer();
    cout << "Moneda origen (ARS/USD/EUR): ";
    getline(cin, desde);
    cout << "Moneda destino (ARS/USD/EUR): ";
    getline(cin, hacia);
    try {
        double resultado = banco->convertirMoneda(monto, desde, hacia);
        cout << GREEN << "\n[OK] Conversion exitosa:" << RESET << endl;
        cout << BOLD << monto << " " << desde << " = " << resultado << " " << hacia << RESET << endl;
    } catch(exception& e) {
        cout << RED << "Error: " << e.what() << RESET << endl;
    }
    pausar();
}

int main() {
    cout << GREEN << BOLD;
    cout << "\n+============================================================+\n";
    cout << "|          BANCO UCC - SISTEMA DE GESTIÓN BANCARIA          |\n";
    cout << "|                    Cargando sistema...                    |\n";
    cout << "+============================================================+\n";
    cout << RESET;
    Banco* banco = new Banco();
    cout << GREEN << "[OK] Sistema cargado exitosamente\n" << RESET;
    int opcion;
    do {
        mostrarMenuPrincipal();
        cin >> opcion;
        limpiarBuffer();
        switch(opcion) {
            case 1:
                menuGestionClientes(banco);
                break;
            case 2:
                menuTransacciones(banco);
                break;
            case 3:
                menuConsultas(banco);
                break;
            case 4:
                menuConversion(banco);
                break;
            case 0:
                cout << GREEN << "\nGracias por usar el Sistema Bancario UCC!\n" << RESET << endl;
                break;
            default:
                cout << RED << "Opción inválida" << RESET << endl;
                limpiarBuffer();
        }
    } while(opcion != 0);
    delete banco;
    return 0;
}
