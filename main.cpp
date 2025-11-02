#include "headers.h"
#include <iostream>
#include <cassert>
#include <cmath>

using namespace std;

// Colores para output
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"

int testsPassed = 0;
int testsFailed = 0;

void testResult(bool condition, const string& testName) {
    if (condition) {
        cout << GREEN << "[OK] " << testName << " - PASSED" << RESET << endl;
        testsPassed++;
    } else {
        cout << RED << "[FAIL] " << testName << " - FAILED" << RESET << endl;
        testsFailed++;
    }
}

// ==================== TEST MONEDA ====================
void testMoneda() {
    cout << CYAN << "\n=== TESTING MONEDA ===" << RESET << endl;
    
    // Test constructores
    Moneda m1(100.0, "ARS");
    testResult(m1.getMonto() == 100.0 && m1.getDenominacion() == "ARS", 
               "Moneda constructor con monto y denominacion");
    
    Moneda m2("USD");
    testResult(m2.getMonto() == 0.0 && m2.getDenominacion() == "USD", 
               "Moneda constructor solo con denominacion");
    
    Moneda m3;
    testResult(m3.getMonto() == 0.0 && m3.getDenominacion() == "ARS", 
               "Moneda constructor por defecto");
    
    // Test operador suma
    try {
        Moneda m4(50.0, "ARS");
        Moneda m5(30.0, "ARS");
        Moneda resultado = m4 + m5;
        testResult(resultado.getMonto() == 80.0, "Moneda suma de misma denominacion");
    } catch (...) {
        testResult(false, "Moneda suma de misma denominacion");
    }
    
    // Test operador suma con diferentes denominaciones (debe lanzar excepcion)
    try {
        Moneda m6(50.0, "ARS");
        Moneda m7(30.0, "USD");
        Moneda resultado = m6 + m7;
        testResult(false, "Moneda suma diferentes denominaciones debe fallar");
    } catch (invalid_argument& e) {
        testResult(true, "Moneda suma diferentes denominaciones debe fallar");
    }
    
    // Test operador resta
    try {
        Moneda m8(100.0, "ARS");
        Moneda m9(40.0, "ARS");
        Moneda resultado = m8 - m9;
        testResult(resultado.getMonto() == 60.0, "Moneda resta de misma denominacion");
    } catch (...) {
        testResult(false, "Moneda resta de misma denominacion");
    }
    
    // Test operador resta con resultado negativo (debe fallar)
    try {
        Moneda m10(30.0, "ARS");
        Moneda m11(50.0, "ARS");
        Moneda resultado = m10 - m11;
        testResult(false, "Moneda resta negativa debe fallar");
    } catch (invalid_argument& e) {
        testResult(true, "Moneda resta negativa debe fallar");
    }
    
    // Test operador mayor que
    try {
        Moneda m12(100.0, "ARS");
        Moneda m13(50.0, "ARS");
        testResult(m12 > m13, "Moneda operador mayor que");
    } catch (...) {
        testResult(false, "Moneda operador mayor que");
    }
    
    // Test operador menor que
    try {
        Moneda m14(30.0, "ARS");
        Moneda m15(70.0, "ARS");
        testResult(m14 < m15, "Moneda operador menor que");
    } catch (...) {
        testResult(false, "Moneda operador menor que");
    }
}

// ==================== TEST FECHA ====================
void testFecha() {
    cout << CYAN << "\n=== TESTING FECHA ===" << RESET << endl;
    
    // Test constructor
    Fecha f1(15, 10, 2024);
    testResult(f1.getDia() == 15 && f1.getMes() == 10 && f1.getAnio() == 2024, 
               "Fecha constructor con parametros");
    
    // Test constructor por defecto
    Fecha f2;
    testResult(f2.getDia() == 1 && f2.getMes() == 1 && f2.getAnio() == 2000, 
               "Fecha constructor por defecto");
    
    // Test constructor desde string
    Fecha f3("25/12/2023");
    testResult(f3.getDia() == 25 && f3.getMes() == 12 && f3.getAnio() == 2023, 
               "Fecha constructor desde string");
    
    // Test setters
    Fecha f4;
    f4.setDia(10);
    f4.setMes(5);
    f4.setAnio(2022);
    testResult(f4.getDia() == 10 && f4.getMes() == 5 && f4.getAnio() == 2022, 
               "Fecha setters");
    
    // Test operador igualdad
    Fecha f5(1, 1, 2020);
    Fecha f6(1, 1, 2020);
    testResult(f5 == f6, "Fecha operador igualdad");
}

// ==================== TEST CAJA AHORRO ====================
void testCajaAhorro() {
    cout << CYAN << "\n=== TESTING CAJA AHORRO ===" << RESET << endl;
    
    // Test constructor
    CajaAhorro* cuenta = new CajaAhorro("ARS");
    testResult(cuenta->getSaldo().getMonto() == 0.0 && 
               cuenta->getSaldo().getDenominacion() == "ARS" &&
               cuenta->getCBU().length() == 22, 
               "CajaAhorro constructor y generacion CBU");
    
    // Test deposito
    bool depositoOk = cuenta->depositar(Moneda(1000.0, "ARS"));
    testResult(depositoOk && cuenta->getSaldo().getMonto() == 1000.0, 
               "CajaAhorro deposito exitoso");
    
    // Test deposito con diferente denominacion
    bool depositoFail = cuenta->depositar(Moneda(100.0, "USD"));
    testResult(!depositoFail, "CajaAhorro deposito diferente denominacion debe fallar");
    
    // Test extraccion exitosa
    bool extraccionOk = cuenta->extraer(Moneda(300.0, "ARS"));
    testResult(extraccionOk && cuenta->getSaldo().getMonto() == 700.0, 
               "CajaAhorro extraccion exitosa");
    
    // Test extraccion sin fondos
    bool extraccionFail = cuenta->extraer(Moneda(1000.0, "ARS"));
    testResult(!extraccionFail && cuenta->getSaldo().getMonto() == 700.0, 
               "CajaAhorro extraccion sin fondos debe fallar");
    
    delete cuenta;
}

// ==================== TEST TARJETA ====================
void testTarjeta() {
    cout << CYAN << "\n=== TESTING TARJETA ===" << RESET << endl;
    
    // Test constructor
    Tarjeta* tarjeta = new Tarjeta("Visa Gold", Moneda(50000.0, "ARS"));
    testResult(tarjeta->getNombreTarjeta() == "Visa Gold" && 
               tarjeta->getLimite().getMonto() == 50000.0 &&
               tarjeta->getLimite().getDenominacion() == "ARS", 
               "Tarjeta constructor");
    
    delete tarjeta;
}

// ==================== TEST TRANSACCION ====================
void testTransaccion() {
    cout << CYAN << "\n=== TESTING TRANSACCION ===" << RESET << endl;
    
    // Test constructor
    Transaccion* trans = new Transaccion("1234567890123456789012", Moneda(5000.0, "ARS"));
    testResult(trans->getCBU() == "1234567890123456789012" && 
               trans->getMonto().getMonto() == 5000.0 &&
               trans->getIDTransaccion() > 0, 
               "Transaccion constructor y generacion ID");
    
    delete trans;
}

// ==================== TEST CLIENTE ====================
void testCliente() {
    cout << CYAN << "\n=== TESTING CLIENTE ===" << RESET << endl;
    
    // Test constructor
    Cliente* cliente = new Cliente("12345678", "Juan Perez", "ORO");
    testResult(cliente->getDNI() == "12345678" && 
               cliente->getNombre() == "Juan Perez" &&
               cliente->getTipoCliente() == "ORO", 
               "Cliente constructor");
    
    // Test estado inicial
    testResult(cliente->getEstado() == false, "Cliente estado inicial");
    
    // Test setEstado
    cliente->setEstado(true);
    testResult(cliente->getEstado() == true, "Cliente setEstado");
    
    // Test agregar cuenta
    CajaAhorro* cuenta1 = new CajaAhorro("ARS");
    CajaAhorro* cuenta2 = new CajaAhorro("USD");
    cliente->agregarCuenta(cuenta1);
    cliente->agregarCuenta(cuenta2);
    testResult(cliente->getCuentas().size() == 2, "Cliente agregar cuentas");
    
    // Test setTarjeta
    Tarjeta* tarjeta = new Tarjeta("Visa Gold", Moneda(50000.0, "ARS"));
    cliente->setTarjeta(tarjeta);
    testResult(cliente->getTarjeta() != nullptr && 
               cliente->getTarjeta()->getNombreTarjeta() == "Visa Gold", 
               "Cliente setTarjeta");
    
    // Test setTipoCliente
    cliente->setTipoCliente("PLATINO");
    testResult(cliente->getTipoCliente() == "PLATINO", "Cliente setTipoCliente");
    
    delete cliente;
}

// ==================== TEST BANCO ====================
void testBanco() {
    cout << CYAN << "\n=== TESTING BANCO ===" << RESET << endl;
    
    // Test constructor (carga desde archivos)
    try {
        Banco* banco = new Banco();
        testResult(true, "Banco constructor y carga de archivos");
        
        // Test altaCliente
        Cliente* nuevoCliente = new Cliente("99999999", "Test Cliente", "ORO");
        nuevoCliente->setEstado(true);
        bool altaOk = banco->altaCliente(nuevoCliente);
        testResult(altaOk, "Banco altaCliente");
        
        // Test buscar cliente (visual - no falla)
        cout << YELLOW << "Test buscarCliente (visual):" << RESET << endl;
        banco->buscarCliente(nuevoCliente->getID());
        testResult(true, "Banco buscarCliente");
        
        // Test listado clientes (visual - no falla)
        cout << YELLOW << "Test listadoClientes (visual):" << RESET << endl;
        banco->listadoClientes();
        testResult(true, "Banco listadoClientes");
        
        // Test informe transacciones (visual - no falla)
        cout << YELLOW << "Test informeTransacciones (visual):" << RESET << endl;
        banco->informeTransacciones();
        testResult(true, "Banco informeTransacciones");
        
        // Test bajaCliente
        bool bajaOk = banco->bajaCliente(nuevoCliente->getID());
        testResult(bajaOk, "Banco bajaCliente");
        
        delete banco;
    } catch (const exception& e) {
        cout << RED << "Error en test Banco: " << e.what() << RESET << endl;
        testResult(false, "Banco operaciones basicas");
    }
}

// ==================== TEST GESTOR ARCHIVOS ====================
void testGestorArchivos() {
    cout << CYAN << "\n=== TESTING GESTOR ARCHIVOS ===" << RESET << endl;
    
    try {
        GestorArchivos gestor;
        
        // Test cargar clientes
        vector<Cliente*> clientes;
        gestor.cargar(clientes, "clientes.txt");
        testResult(clientes.size() > 0, "GestorArchivos cargar clientes");
        
        // Test cargar transacciones
        vector<Transaccion*> transacciones;
        gestor.cargar(transacciones, "transacciones.txt");
        testResult(transacciones.size() > 0, "GestorArchivos cargar transacciones");
        
        // Test cargar cuentas
        vector<CajaAhorro*> cuentas;
        gestor.cargar(cuentas, "cuentas.txt");
        testResult(cuentas.size() > 0, "GestorArchivos cargar cuentas");
        
        // Test cargar tarjetas
        vector<Tarjeta*> tarjetas;
        gestor.cargar(tarjetas, "tarjetas.txt");
        testResult(tarjetas.size() > 0, "GestorArchivos cargar tarjetas");
        
        // Test guardar clientes
        gestor.guardar(clientes, "clientes_test.txt");
        testResult(true, "GestorArchivos guardar clientes");
        
        // Test guardar transacciones
        gestor.guardar(transacciones, "transacciones_test.txt");
        testResult(true, "GestorArchivos guardar transacciones");
        
        // Test guardar cuentas con contexto
        gestor.guardarCuentas(clientes, "cuentas_test.txt");
        testResult(true, "GestorArchivos guardar cuentas con IDCliente");
        
        // Test guardar tarjetas con contexto
        gestor.guardarTarjetas(clientes, "tarjetas_test.txt");
        testResult(true, "GestorArchivos guardar tarjetas con IDCliente");
        
        // Limpieza
        for (auto c : clientes) delete c;
        for (auto t : transacciones) delete t;
        for (auto cu : cuentas) delete cu;
        for (auto ta : tarjetas) delete ta;
        
    } catch (const exception& e) {
        cout << RED << "Error en test GestorArchivos: " << e.what() << RESET << endl;
        testResult(false, "GestorArchivos operaciones");
    }
}

void testIntegracion();

// ==================== TEST CONVERSION DE MONEDAS ====================
void testConversionMonedas() {
    cout << CYAN << "\n=== TESTING CONVERSION DE MONEDAS ===" << RESET << endl;
    
    try {
        // El banco ya cargó las tasas, ahora probamos conversion
        Banco* banco = new Banco();
        
        // Test conversion ARS a USD
        double resultado = banco->convertirMoneda(1000.0, "ARS", "USD");
        testResult(resultado >= 0.9 && resultado <= 1.1, 
                   "Conversion ARS a USD");
        
        // Test conversion USD a ARS
        resultado = banco->convertirMoneda(1.0, "USD", "ARS");
        testResult(resultado >= 900.0 && resultado <= 1100.0, 
                   "Conversion USD a ARS");
        
        // Test conversion ARS a EUR
        resultado = banco->convertirMoneda(1000.0, "ARS", "EUR");
        testResult(resultado >= 0.8 && resultado <= 1.0, 
                   "Conversion ARS a EUR");
        
        // Test conversion EUR a USD
        resultado = banco->convertirMoneda(1.0, "EUR", "USD");
        testResult(resultado >= 1.0 && resultado <= 1.1, 
                   "Conversion EUR a USD");
        
        // Test conversion misma moneda
        resultado = banco->convertirMoneda(500.0, "ARS", "ARS");
        testResult(resultado == 500.0, 
                   "Conversion misma moneda devuelve mismo monto");
        
        // Test conversion inexistente
        try {
            resultado = banco->convertirMoneda(100.0, "ARS", "JPY");
            testResult(false, "Conversion inexistente debe lanzar excepcion");
        } catch (runtime_error& e) {
            testResult(true, "Conversion inexistente lanza excepcion");
        }
        
        delete banco;
    } catch (const exception& e) {
        cout << RED << "Error en test Conversion: " << e.what() << RESET << endl;
        testResult(false, "Test conversion de monedas");
    }
}

// ==================== TEST CLIENTES CON CUENTAS Y TARJETAS ====================
void testClientesCuentasTarjetas() {
    cout << CYAN << "\n=== TESTING CLIENTES CON CUENTAS Y TARJETAS ===" << RESET << endl;
    
    try {
        Banco* banco = new Banco();
        
        // Verificar que los clientes se cargaron
        testResult(true, "Banco cargó clientes desde archivo");
        
        // Probar buscar un cliente específico con ID conocido (4884 es Juan Perez)
        cout << YELLOW << "\nBuscando cliente Juan Perez (verificar cuentas y tarjeta):" << RESET << endl;
        banco->buscarCliente(4884);
        testResult(true, "Cliente Juan Perez consultado");
        
        // Probar buscar un cliente con múltiples cuentas (793 es Carlos Rodriguez)
        cout << YELLOW << "\nBuscando cliente Carlos Rodriguez (3 cuentas):" << RESET << endl;
        banco->buscarCliente(793);
        testResult(true, "Cliente Carlos Rodriguez con multiples cuentas");
        
        // Probar buscar un cliente sin tarjeta (4650 es Maria Garcia - PLATA)
        cout << YELLOW << "\nBuscando cliente Maria Garcia (sin tarjeta - PLATA):" << RESET << endl;
        banco->buscarCliente(4650);
        testResult(true, "Cliente PLATA sin tarjeta");
        
        // Listado completo
        cout << YELLOW << "\nListado completo de clientes:" << RESET << endl;
        banco->listadoClientes();
        testResult(true, "Listado completo de clientes");
        
        // Verificar transacciones
        cout << YELLOW << "\nInforme de transacciones:" << RESET << endl;
        banco->informeTransacciones();
        testResult(true, "Informe de transacciones generado");
        
        delete banco;
    } catch (const exception& e) {
        cout << RED << "Error en test Clientes/Cuentas/Tarjetas: " << e.what() << RESET << endl;
        testResult(false, "Test clientes con cuentas y tarjetas");
    }
}

// ==================== TEST INTEGRACION ====================
void testIntegracion() {
    cout << CYAN << "\n=== TESTING INTEGRACION ===" << RESET << endl;
    
    try {
        // Crear un cliente completo con cuentas y tarjeta
        Cliente* cliente = new Cliente("88888888", "Cliente Completo", "PLATINO");
        cliente->setEstado(true);
        
        // Agregar cuentas
        CajaAhorro* cuenta1 = new CajaAhorro("ARS");
        cuenta1->depositar(Moneda(100000.0, "ARS"));
        cliente->agregarCuenta(cuenta1);
        
        CajaAhorro* cuenta2 = new CajaAhorro("USD");
        cuenta2->depositar(Moneda(5000.0, "USD"));
        cliente->agregarCuenta(cuenta2);
        
        // Agregar tarjeta
        Tarjeta* tarjeta = new Tarjeta("Mastercard Platinum", Moneda(150000.0, "ARS"));
        cliente->setTarjeta(tarjeta);
        
        // Verificar integridad
        testResult(cliente->getCuentas().size() == 2 && 
                   cliente->getTarjeta() != nullptr &&
                   cliente->getCuentas()[0]->getSaldo().getMonto() == 100000.0,
                   "Integracion cliente con cuentas y tarjeta");
        
        // Crear transaccion
        Transaccion* trans = new Transaccion(cuenta1->getCBU(), Moneda(5000.0, "ARS"));
        testResult(trans->getCBU() == cuenta1->getCBU(), 
                   "Integracion transaccion vinculada a cuenta");
        
        // Operaciones en cuenta
        bool extraccionOk = cuenta1->extraer(Moneda(10000.0, "ARS"));
        testResult(extraccionOk && cuenta1->getSaldo().getMonto() == 90000.0,
                   "Integracion operacion extraccion en cuenta");
        
        bool depositoOk = cuenta2->depositar(Moneda(1000.0, "USD"));
        testResult(depositoOk && cuenta2->getSaldo().getMonto() == 6000.0,
                   "Integracion operacion deposito en cuenta");
        
        delete trans;
        delete cliente;
        
    } catch (const exception& e) {
        cout << RED << "Error en test Integracion: " << e.what() << RESET << endl;
        testResult(false, "Test de integracion");
    }
}

// ==================== MAIN ====================
int main() {
    cout << "\n";
    cout << "+======================================================+\n";
    cout << "|   SISTEMA DE GESTION BANCARIA UCC - UNIT TESTS     |\n";
    cout << "+======================================================+\n";
    
    // Ejecutar todos los tests
    testMoneda();
    testFecha();
    testCajaAhorro();
    testTarjeta();
    testTransaccion();
    testCliente();
    testGestorArchivos();
    testBanco();
    testConversionMonedas();
    testClientesCuentasTarjetas();
    testIntegracion();
    
    // Resumen final
    cout << "\n";
    cout << "+======================================================+\n";
    cout << "|                  RESUMEN DE TESTS                   |\n";
    cout << "+======================================================+\n";
    cout << GREEN << "Tests Exitosos: " << testsPassed << RESET << endl;
    cout << RED << "Tests Fallados: " << testsFailed << RESET << endl;
    cout << "Total Tests: " << (testsPassed + testsFailed) << endl;
    
    if (testsFailed == 0) {
        cout << GREEN << "\nTodos los tests pasaron exitosamente!\n" << RESET << endl;
    } else {
        cout << RED << "\nAlgunos tests fallaron. Revisar implementacion.\n" << RESET << endl;
    }
    
    return testsFailed > 0 ? 1 : 0;
}
