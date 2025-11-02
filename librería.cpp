#include "headers.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <random>
#include <stdexcept>
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

// ==================== MONEDA ====================

Moneda :: Moneda(double m, string d) : monto(m), denominacion(d) {}

Moneda :: Moneda(string d) : monto(0.0), denominacion(d) {}

Moneda :: Moneda() : monto(0.0), denominacion("ARS") {}

double Moneda :: getMonto()
{
    return monto;
}

string Moneda :: getDenominacion()
{
    return denominacion;
}

Moneda operator+(const Moneda& m1, const Moneda& m2){
    if(m1.denominacion != m2.denominacion){
        throw invalid_argument("No se pueden sumar monedas de diferentes denominaciones.");
    }
    if (m1.monto + m2.monto < 0){
        throw invalid_argument("El resultado de la suma no puede ser negativo.");
    }
    return Moneda(m1.monto + m2.monto, m1.denominacion);
}

Moneda operator-(const Moneda& m1, const Moneda& m2){
    if(m1.denominacion != m2.denominacion){
        throw invalid_argument("No se pueden restar monedas de diferentes denominaciones.");
    }
    if (m1.monto < m2.monto){
        throw invalid_argument("El resultado de la resta no puede ser negativo.");
    }
    return Moneda(m1.monto - m2.monto, m1.denominacion);
}

ostream& operator<<(ostream& os, const Moneda& m){
    os << m.monto << " " << m.denominacion;
    return os;
}

bool operator>(const Moneda& m1, const Moneda& m2){
    if(m1.denominacion != m2.denominacion){
        throw invalid_argument("No se pueden comparar monedas de diferentes denominaciones.");
    }
    return m1.monto > m2.monto;
}

bool operator<(const Moneda& m1, const Moneda& m2){
    if(m1.denominacion != m2.denominacion){
        throw invalid_argument("No se pueden comparar monedas de diferentes denominaciones.");
    }
    return m1.monto < m2.monto;
}

// ==================== FECHA ====================

Fecha :: Fecha() : dia(1), mes(1), anio(2000) {}

Fecha :: Fecha(int d, int m, int a) : dia(d), mes(m), anio(a) {}

Fecha :: Fecha(string fechaStr) {
    // Formato esperado: DD/MM/YYYY
    size_t pos1 = fechaStr.find('/');
    size_t pos2 = fechaStr.find('/', pos1 + 1);
    
    if (pos1 != string::npos && pos2 != string::npos) {
        dia = stoi(fechaStr.substr(0, pos1));
        mes = stoi(fechaStr.substr(pos1 + 1, pos2 - pos1 - 1));
        anio = stoi(fechaStr.substr(pos2 + 1));
    } else {
        dia = 1;
        mes = 1;
        anio = 2000;
    }
}

int Fecha :: getDia() {
    return dia;
}

int Fecha :: getMes() {
    return mes;
}

int Fecha :: getAnio() {
    return anio;
}

void Fecha :: setDia(int d) {
    dia = d;
}

void Fecha :: setMes(int m) {
    mes = m;
}

void Fecha :: setAnio(int a) {
    anio = a;
}

ostream& operator<<(ostream& os, const Fecha& f) {
    os << (f.dia < 10 ? "0" : "") << f.dia << "/"
       << (f.mes < 10 ? "0" : "") << f.mes << "/"
       << f.anio;
    return os;
}

bool operator==(const Fecha& f1, const Fecha& f2) {
    return f1.dia == f2.dia && f1.mes == f2.mes && f1.anio == f2.anio;
}

// ==================== TARJETA ====================

Tarjeta :: Tarjeta(string nombre, Moneda Lim) : nombreTarjeta(nombre), limite(Lim) {}

Moneda Tarjeta :: getLimite()
{
    return limite;
}

string Tarjeta :: getNombreTarjeta()
{
    return nombreTarjeta;
}

// ==================== CAJA AHORRO ====================

CajaAhorro :: CajaAhorro(string denominacion) : saldo(denominacion) {
    generarCBU();
}

void CajaAhorro :: generarCBU() {
    CBU = to_string(rand() % 1000000000);
    for (int i = CBU.length(); i < 22; i++){
        CBU = "0" + CBU;
    }
}

bool CajaAhorro :: depositar(Moneda monto) {
    if(monto.getMonto() <= 0) {
        return false;
    }
    if(monto.getDenominacion() == saldo.getDenominacion()) {
        saldo = saldo + monto;
        return true;
    }
    return false;
}

bool CajaAhorro :: extraer(Moneda monto) {
    if(monto.getMonto() <= 0) {
        return false;
    }
    if(monto.getDenominacion() == saldo.getDenominacion()) {
        if(saldo > monto) {
            saldo = saldo - monto;
            return true;
        }
    }
    return false;
}

Moneda CajaAhorro :: getSaldo() {
    return saldo;
}

string CajaAhorro :: getCBU() {
    return CBU;
}

// ==================== TRANSACCION ====================

Transaccion :: Transaccion(string cbu, Moneda m, int idCliente) : IDCliente(idCliente), CBU(cbu), monto(m)
{
    // Obtener fecha actual del sistema
    time_t t = time(NULL);
    struct tm* tiempo = localtime(&t);
    // tm_mon va de 0-11, sumamos 1. tm_year es años desde 1900, sumamos 1900
    fecha = Fecha(tiempo->tm_mday, tiempo->tm_mon + 1, tiempo->tm_year + 1900);
    generarIDTransaccion();
}

int Transaccion :: getIDCliente()
{
    return IDCliente;
}

int Transaccion :: getIDTransaccion()
{
    return IDTransaccion;
}

void Transaccion :: generarIDTransaccion()
{
    srand(time(NULL));
    IDTransaccion = fecha.getAnio() * 10000 + fecha.getMes() * 1000 + fecha.getDia() * 100 + rand() % 100 + 1;
}

Moneda Transaccion :: getMonto()
{
    return monto;
}

string Transaccion :: getCBU()
{
    return CBU;
}

Fecha Transaccion :: getFecha()
{
   return fecha; 
}

// ==================== PERSONA ====================

Persona :: Persona(string dni, string Nombre) : DNI(dni), nombre(Nombre) {
    // Fecha de ingreso automatica del sistema
    time_t t = time(NULL);
    struct tm* tiempo = localtime(&t);
    ingreso = Fecha(tiempo->tm_mday, tiempo->tm_mon + 1, tiempo->tm_year + 1900);
}

string Persona :: getDNI()
{
    return DNI;
}

string Persona :: getNombre()
{
    return nombre;
}

Fecha Persona :: getAnioIngreso()
{
    return ingreso;
}

// ==================== CLIENTE ====================

Cliente :: Cliente(string dni, string Nombre, string tipo) : Persona(dni, Nombre), tipoCliente(tipo) {
    estado = true; // Cliente nuevo esta ACTIVO por defecto
    tarjeta = nullptr;
    generarID();
}

double Cliente :: getID()
{
    return ID;
}

string Cliente :: getTipoCliente()
{
    return tipoCliente;
}

bool Cliente :: getEstado()
{
    return estado;
}

vector<CajaAhorro*> Cliente :: getCuentas()
{
    return cuentas;
}

Tarjeta* Cliente :: getTarjeta()
{
    return tarjeta;
}

void Cliente :: setEstado(bool estado)
{
    this->estado = estado;
}

void Cliente :: setTarjeta(Tarjeta* tarjeta)
{
    this->tarjeta = tarjeta;
}

void Cliente :: setTipoCliente(string tipo)
{
    tipoCliente = tipo;   
}

void Cliente :: agregarCuenta(CajaAhorro* cuenta)
{
    cuentas.push_back(cuenta);
}

void Cliente :: eliminarCuenta(string cbu)
{
    for (size_t i = 0; i < cuentas.size(); i++) {
        if (cuentas[i]->getCBU() == cbu) {
            delete cuentas[i];
            cuentas.erase(cuentas.begin() + i);
            break;
        }
    }
}

void Cliente :: generarID()
{
    ID = rand() % 10000 +1;
}

// ==================== GESTOR ARCHIVOS ====================

// =================== CLIENTE ====================

// Carga de datos desde clientes.txt y creación de objetos Cliente

template<>
void GestorArchivos::cargar<Cliente>(vector<Cliente*>& datos, const string& NombreArchivo) {
    ifstream archivo(NombreArchivo);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo abrir el archivo: " + NombreArchivo);
    }
    try {
        string linea;
        int lineaNum = 0;
        if (getline(archivo, linea)) {
            lineaNum++;
        }
        while (getline(archivo, linea)) {
            lineaNum++;
            if (linea.empty()) continue;
            try {
                stringstream ss(linea);
                string idClienteStr, dni, nombre, tipo, fechaStr, estadoStr;
                // Formato: IDCliente,DNI,Nombre,TipoCliente,FechaIngreso,Estado
                getline(ss, idClienteStr, ',');
                getline(ss, dni, ',');
                getline(ss, nombre, ',');
                getline(ss, tipo, ',');
                getline(ss, fechaStr, ',');
                getline(ss, estadoStr, ',');
                if (dni.empty() || nombre.empty() || tipo.empty()) {
                    throw invalid_argument("Datos incompletos en linea " + to_string(lineaNum));
                }
                Cliente* cliente = new Cliente(dni, nombre, tipo);
                // Restaurar el IDCliente del archivo
                if (!idClienteStr.empty()) {
                    cliente->ID = stod(idClienteStr);
                }
                cliente->setEstado(estadoStr == "ACTIVO" ? true : false);
                if (!fechaStr.empty()) {
                    cliente->ingreso = Fecha(fechaStr);
                }
                datos.push_back(cliente);
            } catch (const exception& e) {
                cerr << "Error en linea " << lineaNum << " de " << NombreArchivo 
                     << ": " << e.what() << endl;
            }
        }
        archivo.close();
        cout << "Cargados " << datos.size() << " clientes desde " << NombreArchivo << endl;
    } catch (const exception& e) {
        archivo.close();
        throw runtime_error("Error al cargar clientes: " + string(e.what()));
    }
}

// Guardado de datos de objetos Cliente en clientes.txt

template<>
void GestorArchivos::guardar<Cliente>(const vector<Cliente*>& datos, const string& NombreArchivo) {
    ofstream archivo(NombreArchivo);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo crear el archivo: " + NombreArchivo);
    }
    try {
        archivo << "IDCliente,DNI,Nombre,TipoCliente,FechaIngreso,Estado" << endl;
        for (const auto& cliente : datos) {
            if (cliente != nullptr) {
                archivo << cliente->getID() << ","
                       << cliente->getDNI() << ","
                       << cliente->getNombre() << ","
                       << cliente->getTipoCliente() << ","
                       << cliente->getAnioIngreso() << ","
                       << (cliente->getEstado() ? "ACTIVO" : "BAJA") << endl;
            }
        }
        archivo.close();
        cout << "Guardados " << datos.size() << " clientes en " << NombreArchivo << endl;
    } catch (const exception& e) {
        archivo.close();
        throw runtime_error("Error al guardar clientes: " + string(e.what()));
    }
}

// ==================== TRANSACCION ====================

// Carga de datos desde transacciones.txt y creación de objetos Transaccion

template<>
void GestorArchivos::cargar<Transaccion>(vector<Transaccion*>& datos, const string& NombreArchivo) {
    ifstream archivo(NombreArchivo);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo abrir el archivo: " + NombreArchivo);
    }
    try {
        string linea;
        int lineaNum = 0;
        if (getline(archivo, linea)) {
            lineaNum++;
        }
        while (getline(archivo, linea)) {
            lineaNum++;
            if (linea.empty()) continue;
            try {
                stringstream ss(linea);
                string idTransStr, idClienteStr, cbu, fechaStr, montoStr, denominacion;
                // Formato: IDTransaccion,IDCliente,CBU,Fecha,Monto,Denominacion
                getline(ss, idTransStr, ',');
                getline(ss, idClienteStr, ',');
                getline(ss, cbu, ',');
                getline(ss, fechaStr, ',');
                getline(ss, montoStr, ',');
                getline(ss, denominacion, ',');
                if (cbu.empty() || montoStr.empty() || denominacion.empty()) {
                    throw invalid_argument("Datos incompletos en linea " + to_string(lineaNum));
                }
                double monto = stod(montoStr);
                int idCliente = stoi(idClienteStr);
                Transaccion* transaccion = new Transaccion(cbu, Moneda(monto, denominacion), idCliente);
                transaccion->IDTransaccion = stoi(idTransStr);
                if (!fechaStr.empty()) {
                    transaccion->fecha = Fecha(fechaStr);
                }
                datos.push_back(transaccion);
            } catch (const exception& e) {
                cerr << "Error en linea " << lineaNum << " de " << NombreArchivo 
                     << ": " << e.what() << endl;
            }
        }
        archivo.close();
        cout << "Cargadas " << datos.size() << " transacciones desde " << NombreArchivo << endl;
    } catch (const exception& e) {
        archivo.close();
        throw runtime_error("Error al cargar transacciones: " + string(e.what()));
    }
}

// Guardado de datos de objetos Transaccion en transacciones.txt

template<>
void GestorArchivos::guardar<Transaccion>(const vector<Transaccion*>& datos, const string& NombreArchivo) {
    ofstream archivo(NombreArchivo);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo crear el archivo: " + NombreArchivo);
    }
    try {
        archivo << "IDTransaccion,IDCliente,CBU,Fecha,Monto,Denominacion" << endl;
        for (const auto& trans : datos) {
            if (trans != nullptr) {
                archivo << trans->getIDTransaccion() << ","
                       << trans->getIDCliente() << ","
                       << trans->getCBU() << ","
                       << trans->getFecha() << ","
                       << trans->getMonto().getMonto() << ","
                       << trans->getMonto().getDenominacion() << endl;
            }
        }
        archivo.close();
        cout << "Guardadas " << datos.size() << " transacciones en " << NombreArchivo << endl;
    } catch (const exception& e) {
        archivo.close();
        throw runtime_error("Error al guardar transacciones: " + string(e.what()));
    }
}

// ==================== TARJETA ====================

// Carga de datos desde tarjetas.txt y creación de objetos Tarjeta

template<>
void GestorArchivos::cargar<Tarjeta>(vector<Tarjeta*>& datos, const string& NombreArchivo) {
    ifstream archivo(NombreArchivo);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo abrir el archivo: " + NombreArchivo);
    }
    try {
        string linea;
        int lineaNum = 0;
        if (getline(archivo, linea)) {
            lineaNum++;
        }
        while (getline(archivo, linea)) {
            lineaNum++;
            if (linea.empty()) continue;
            try {
                stringstream ss(linea);
                string idClienteStr, nombreTarjeta, limiteStr, denominacion;
                // Formato: IDCliente,NombreTarjeta,Limite,Denominacion
                getline(ss, idClienteStr, ',');
                getline(ss, nombreTarjeta, ',');
                getline(ss, limiteStr, ',');
                getline(ss, denominacion, ',');
                if (nombreTarjeta.empty() || limiteStr.empty() || denominacion.empty()) {
                    throw invalid_argument("Datos incompletos en linea " + to_string(lineaNum));
                }
                double limite = stod(limiteStr);
                Tarjeta* tarjeta = new Tarjeta(nombreTarjeta, Moneda(limite, denominacion));
                datos.push_back(tarjeta);
            } catch (const exception& e) {
                cerr << "Error en linea " << lineaNum << " de " << NombreArchivo 
                     << ": " << e.what() << endl;
            }
        }
        archivo.close();
        cout << "Cargadas " << datos.size() << " tarjetas desde " << NombreArchivo << endl;
    } catch (const exception& e) {
        archivo.close();
        throw runtime_error("Error al cargar tarjetas: " + string(e.what()));
    }
}

// Guardado de datos de objetos Tarjeta en tarjetas.txt

// Método específico para guardar tarjetas con contexto de clientes

void GestorArchivos::guardarTarjetas(const vector<Cliente*>& clientes, const string& NombreArchivo) {
    ofstream archivo(NombreArchivo);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo crear el archivo: " + NombreArchivo);
    }
    try {
        archivo << "IDCliente,NombreTarjeta,Limite,Denominacion" << endl;
        int totalTarjetas = 0;
        for (const auto& cliente : clientes) {
            if (cliente != nullptr && cliente->getTarjeta() != nullptr) {
                Tarjeta* tarjeta = cliente->getTarjeta();
                archivo << cliente->getID() << ","
                       << tarjeta->getNombreTarjeta() << ","
                       << tarjeta->getLimite().getMonto() << ","
                       << tarjeta->getLimite().getDenominacion() << endl;
                totalTarjetas++;
            }
        }
        archivo.close();
        cout << "Guardadas " << totalTarjetas << " tarjetas en " << NombreArchivo << endl;
    } catch (const exception& e) {
        archivo.close();
        throw runtime_error("Error al guardar tarjetas: " + string(e.what()));
    }
}

// ==================== CAJA AHORRO ====================

// Método específico para guardar cuentas con contexto de clientes

void GestorArchivos::guardarCuentas(const vector<Cliente*>& clientes, const string& NombreArchivo) {
    ofstream archivo(NombreArchivo);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo crear el archivo: " + NombreArchivo);
    }
    try {
        archivo << "CBU,IDCliente,Denominacion,Saldo" << endl;
        int totalCuentas = 0;
        for (const auto& cliente : clientes) {
            if (cliente != nullptr) {
                for (const auto& cuenta : cliente->getCuentas()) {
                    if (cuenta != nullptr) {
                        archivo << cuenta->getCBU() << ","
                               << cliente->getID() << ","
                               << cuenta->getSaldo().getDenominacion() << ","
                               << cuenta->getSaldo().getMonto() << endl;
                        totalCuentas++;
                    }
                }
            }
        }
        archivo.close();
        cout << "Guardadas " << totalCuentas << " cuentas en " << NombreArchivo << endl;
    } catch (const exception& e) {
        archivo.close();
        throw runtime_error("Error al guardar cuentas: " + string(e.what()));
    }
}

// Cargado de datos desde cuentas.txt y creación de objetos CajaAhorro

template<>
void GestorArchivos::cargar<CajaAhorro>(vector<CajaAhorro*>& datos, const string& NombreArchivo) {
    ifstream archivo(NombreArchivo);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo abrir el archivo: " + NombreArchivo);
    }
    try {
        string linea;
        int lineaNum = 0;
        if (getline(archivo, linea)) {
            lineaNum++;
        }
        while (getline(archivo, linea)) {
            lineaNum++;
            if (linea.empty()) continue;
            try {
                stringstream ss(linea);
                string cbu, idClienteStr, denominacion, saldoStr;
                // Formato: CBU,IDCliente,Denominacion,Saldo
                getline(ss, cbu, ',');
                getline(ss, idClienteStr, ',');
                getline(ss, denominacion, ',');
                getline(ss, saldoStr, ',');
                if (cbu.empty() || denominacion.empty() || saldoStr.empty()) {
                    throw invalid_argument("Datos incompletos en linea " + to_string(lineaNum));
                }
                double saldo = stod(saldoStr);
                CajaAhorro* cuenta = new CajaAhorro(denominacion);
                cuenta->CBU = cbu;
                cuenta->saldo = Moneda(saldo, denominacion);
                datos.push_back(cuenta);
            } catch (const exception& e) {
                cerr << "Error en linea " << lineaNum << " de " << NombreArchivo 
                     << ": " << e.what() << endl;
            }
        }
        archivo.close();
        cout << "Cargadas " << datos.size() << " cuentas desde " << NombreArchivo << endl;
    } catch (const exception& e) {
        archivo.close();
        throw runtime_error("Error al cargar cuentas: " + string(e.what()));
    }
}

// ==================== BANCO ====================

Banco::Banco() {
    try {
        ifstream archivoTasas("tasas.txt");
        if (archivoTasas.is_open()) {
            string linea;
            getline(archivoTasas, linea);
            while (getline(archivoTasas, linea)) {
                if (linea.empty()) continue;
                stringstream ss(linea);
                string origen, destino, tasaStr;
                getline(ss, origen, ',');
                getline(ss, destino, ',');
                getline(ss, tasaStr, ',');
                try {
                    double tasa = stod(tasaStr);
                    tasasConversion[make_pair(origen, destino)] = tasa;
                } catch (...) {
                    cerr << "Error al leer tasa de conversion" << endl;
                }
            }
            archivoTasas.close();
            cout << "Cargadas " << tasasConversion.size() << " tasas de conversion desde tasas.txt" << endl;
        }
    } catch (const exception& e) {
        cerr << "Advertencia al cargar tasas: " << e.what() << endl;
    }
    try {
        gestor.cargar(clientes, "clientes.txt");
    } catch (const exception& e) {
        cerr << "Advertencia al cargar clientes: " << e.what() << endl;
    }
    try {
        vector<CajaAhorro*> cuentasTemp;
        gestor.cargar(cuentasTemp, "cuentas.txt");
        ifstream archivoCuentas("cuentas.txt");
        if (archivoCuentas.is_open()) {
            string linea;
            getline(archivoCuentas, linea);
            int idx = 0;
            while (getline(archivoCuentas, linea) && idx < cuentasTemp.size()) {
                if (linea.empty()) continue;
                stringstream ss(linea);
                string cbu, idClienteStr, denominacion, saldoStr;
                getline(ss, cbu, ',');
                getline(ss, idClienteStr, ',');
                getline(ss, denominacion, ',');
                getline(ss, saldoStr, ',');
                try {
                    // Matchear por IDCliente del archivo
                    double idCliente = stod(idClienteStr);
                    for (auto& cliente : clientes) {
                        if (cliente->getID() == idCliente) {
                            cliente->agregarCuenta(cuentasTemp[idx]);
                            break;
                        }
                    }
                } catch (...) {
                    cerr << "Error al vincular cuenta con cliente" << endl;
                }
                idx++;
            }
            archivoCuentas.close();
        }
    } catch (const exception& e) {
        cerr << "Advertencia al cargar cuentas: " << e.what() << endl;
    }
    try {
        vector<Tarjeta*> tarjetasTemp;
        gestor.cargar(tarjetasTemp, "tarjetas.txt");
        ifstream archivoTarjetas("tarjetas.txt");
        if (archivoTarjetas.is_open()) {
            string linea;
            getline(archivoTarjetas, linea);
            int idx = 0;
            while (getline(archivoTarjetas, linea) && idx < tarjetasTemp.size()) {
                if (linea.empty()) continue;
                stringstream ss(linea);
                string idClienteStr, nombreTarjeta, limiteStr, denominacion;
                getline(ss, idClienteStr, ',');
                getline(ss, nombreTarjeta, ',');
                getline(ss, limiteStr, ',');
                getline(ss, denominacion, ',');
                try {
                    // Matchear por IDCliente del archivo
                    double idCliente = stod(idClienteStr);
                    for (auto& cliente : clientes) {
                        if (cliente->getID() == idCliente) {
                            if (cliente->getTipoCliente() != "PLATA") {
                                cliente->setTarjeta(tarjetasTemp[idx]);
                            }
                            break;
                        }
                    }
                } catch (...) {
                    cerr << "Error al vincular tarjeta con cliente" << endl;
                }
                idx++;
            }
            archivoTarjetas.close();
        }
    } catch (const exception& e) {
        cerr << "Advertencia al cargar tarjetas: " << e.what() << endl;
    }
    try {
        gestor.cargar(transacciones, "transacciones.txt");
    } catch (const exception& e) {
        cerr << "Advertencia al cargar transacciones: " << e.what() << endl;
    }
    try {
        ofstream logFile("import_log.txt", ios::app);
        if (logFile.is_open()) {
            time_t now = time(0);
            char* dt = ctime(&now);
            logFile << "=== Banco inicializado ===" << endl;
            logFile << "Fecha: " << dt;
            logFile << "Clientes cargados: " << clientes.size() << endl;
            logFile << "Transacciones cargadas: " << transacciones.size() << endl;
            logFile << "=========================" << endl << endl;
            logFile.close();
        }
    } catch (...) {
        cerr << "No se pudo escribir en import_log.txt" << endl;
    }
}

bool Banco::altaCliente(Cliente* cliente) {
    if (cliente == nullptr) {
        cerr << "Error: Cliente nulo" << endl;
        return false;
    }
    try {
        for (const auto& c : clientes) {
            if (c->getDNI() == cliente->getDNI()) {
                throw runtime_error("Ya existe un cliente con DNI: " + cliente->getDNI());
            }
        }
        clientes.push_back(cliente);
        gestor.guardar(clientes, "clientes.txt");
        // NO guardamos cuentas/tarjetas aqui porque el cliente nuevo no tiene cuentas aun
        // Esto borraria las cuentas/tarjetas existentes de otros clientes
        ofstream logFile("import_log.txt", ios::app);
        if (logFile.is_open()) {
            time_t now = time(0);
            char* dt = ctime(&now);
            logFile << "Alta de cliente: " << cliente->getNombre() 
                   << " (DNI: " << cliente->getDNI() << ") - " << dt;
            logFile.close();
        }
        cout << "Cliente dado de alta exitosamente. ID: " << cliente->getID() << endl;
        return true;
    } catch (const exception& e) {
        cerr << "Error al dar de alta cliente: " << e.what() << endl;
        return false;
    }
}

bool Banco::bajaCliente(double id) {
    try {
        for (auto& cliente : clientes) {
            if (cliente->getID() == id) {
                if (!cliente->getEstado()) {
                    throw runtime_error("El cliente ya esta dado de baja");
                }
                cliente->setEstado(false);
                gestor.guardar(clientes, "clientes.txt");
                // NO guardamos cuentas/tarjetas en baja de cliente
                // Solo actualizamos el estado en clientes.txt
                ofstream logFile("import_log.txt", ios::app);
                if (logFile.is_open()) {
                    time_t now = time(0);
                    char* dt = ctime(&now);
                    logFile << "Baja de cliente: " << cliente->getNombre() 
                           << " (ID: " << id << ") - " << dt;
                    logFile.close();
                }
                cout << "Cliente dado de baja exitosamente" << endl;
                return true;
            }
        }
        throw runtime_error("Cliente no encontrado con ID: " + to_string((int)id));
    } catch (const exception& e) {
        cerr << "Error al dar de baja cliente: " << e.what() << endl;
        return false;
    }
}

void Banco::buscarCliente(double id) {
    try {
        for (const auto& cliente : clientes) {
            if (cliente->getID() == id) {
                cout << "\n=== DETALLE DEL CLIENTE ===" << endl;
                cout << "ID: " << cliente->getID() << endl;
                cout << "DNI: " << cliente->getDNI() << endl;
                cout << "Nombre: " << cliente->getNombre() << endl;
                cout << "Tipo: " << cliente->getTipoCliente() << endl;
                cout << "Estado: " << (cliente->getEstado() ? "ACTIVO" : "BAJA") << endl;
                cout << "Fecha Ingreso: " << cliente->getAnioIngreso() << endl;
                if (cliente->getTarjeta() != nullptr) {
                    cout << "Tarjeta: Si (Limite: " << cliente->getTarjeta()->getLimite() << ")" << endl;
                } else {
                    cout << "Tarjeta: No" << endl;
                }
                cout << "Cuentas: " << cliente->getCuentas().size() << endl;
                for (const auto& cuenta : cliente->getCuentas()) {
                    cout << "  - CBU: " << cuenta->getCBU() 
                         << " | Saldo: " << cuenta->getSaldo() << endl;
                }
                cout << "==========================\n" << endl;
                return;
            }
        }
        throw runtime_error("Cliente no encontrado con ID: " + to_string((int)id));
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void Banco::listadoClientes() {
    try {
        if (clientes.empty()) {
            cout << "No hay clientes registrados" << endl;
            return;
        }
        cout << "\n=== LISTADO DE CLIENTES ===" << endl;
        cout << "Total: " << clientes.size() << " clientes" << endl;
        cout << "---------------------------------------------------" << endl;
        for (const auto& cliente : clientes) {
            cout << "ID: " << cliente->getID() 
                 << " | DNI: " << cliente->getDNI()
                 << " | Nombre: " << cliente->getNombre()
                 << " | Tipo: " << cliente->getTipoCliente()
                 << " | Estado: " << (cliente->getEstado() ? "ACTIVO" : "BAJA") << endl;
        }
        cout << "===========================\n" << endl;
    } catch (const exception& e) {
        cerr << "Error al listar clientes: " << e.what() << endl;
    }
}

void Banco::informeTransacciones() {
    try {
        if (transacciones.empty()) {
            cout << "No hay transacciones registradas" << endl;
            return;
        }
        cout << "\n=== INFORME DE TODAS LAS TRANSACCIONES ===" << endl;
        cout << "Total: " << transacciones.size() << " transacciones" << endl;
        cout << "---------------------------------------------------" << endl;
        for (const auto& trans : transacciones) {
            cout << "ID Trans: " << trans->getIDTransaccion()
                 << " | ID Cliente: " << trans->getIDCliente()
                 << " | CBU: " << trans->getCBU()
                 << " | Fecha: " << trans->getFecha()
                 << " | Monto: " << trans->getMonto() << endl;
        }
        cout << "==========================================\n" << endl;
    } catch (const exception& e) {
        cerr << "Error al generar informe: " << e.what() << endl;
    }
}

void Banco::informeTransacciones(double id) {
    try {
        Cliente* clienteEncontrado = nullptr;
        for (const auto& cliente : clientes) {
            if (cliente->getID() == id) {
                clienteEncontrado = cliente;
                break;
            }
        }
        if (!clienteEncontrado) {
            throw runtime_error("Cliente no encontrado con ID: " + to_string((int)id));
        }
        vector<Transaccion*> transCliente;
        for (const auto& trans : transacciones) {
            if (trans->getIDCliente() == id) {
                transCliente.push_back(trans);
            }
        }
        cout << "\n=== TRANSACCIONES DEL CLIENTE ===" << endl;
        cout << "Cliente: " << clienteEncontrado->getNombre() << " (ID: " << id << ")" << endl;
        cout << "Total transacciones: " << transCliente.size() << endl;
        cout << "---------------------------------------------------" << endl;
        if (transCliente.empty()) {
            cout << "No hay transacciones para este cliente" << endl;
        } else {
            for (const auto& trans : transCliente) {
                cout << "ID Trans: " << trans->getIDTransaccion()
                     << " | CBU: " << trans->getCBU()
                     << " | Fecha: " << trans->getFecha()
                     << " | Monto: " << trans->getMonto() << endl;
            }
        }
        cout << "=================================\n" << endl;
    } catch (const exception& e) {
        cerr << "Error al generar informe: " << e.what() << endl;
    }
}

void Banco::informeTransacciones(Fecha desde, Fecha hasta) {
    try {
        vector<Transaccion*> transFiltradas;
        for (const auto& trans : transacciones) {
            Fecha fechaTrans = trans->getFecha();
            bool despuesDe = (fechaTrans.getAnio() > desde.getAnio()) ||
                            (fechaTrans.getAnio() == desde.getAnio() && fechaTrans.getMes() > desde.getMes()) ||
                            (fechaTrans.getAnio() == desde.getAnio() && fechaTrans.getMes() == desde.getMes() && 
                             fechaTrans.getDia() >= desde.getDia());
            bool antesDe = (fechaTrans.getAnio() < hasta.getAnio()) ||
                          (fechaTrans.getAnio() == hasta.getAnio() && fechaTrans.getMes() < hasta.getMes()) ||
                          (fechaTrans.getAnio() == hasta.getAnio() && fechaTrans.getMes() == hasta.getMes() && 
                           fechaTrans.getDia() <= hasta.getDia());
            if (despuesDe && antesDe) {
                transFiltradas.push_back(trans);
            }
        }
        cout << "\n=== TRANSACCIONES POR PERIODO ===" << endl;
        cout << "Desde: " << desde << " | Hasta: " << hasta << endl;
        cout << "Total: " << transFiltradas.size() << " transacciones" << endl;
        cout << "---------------------------------------------------" << endl;
        if (transFiltradas.empty()) {
            cout << "No hay transacciones en este periodo" << endl;
        } else {
            for (const auto& trans : transFiltradas) {
                cout << "ID Trans: " << trans->getIDTransaccion()
                     << " | ID Cliente: " << trans->getIDCliente()
                     << " | CBU: " << trans->getCBU()
                     << " | Fecha: " << trans->getFecha()
                     << " | Monto: " << trans->getMonto() << endl;
            }
        }
        cout << "=================================\n" << endl;
    } catch (const exception& e) {
        cerr << "Error al generar informe: " << e.what() << endl;
    }
}

double Banco::convertirMoneda(double monto, string desde, string hacia) {
    if (desde == hacia) {
        return monto;
    }
    auto it = tasasConversion.find(make_pair(desde, hacia));
    if (it != tasasConversion.end()) {
        return monto * it->second;
    }
    throw runtime_error("No existe tasa de conversion de " + desde + " a " + hacia);
}

void Banco::registrarTransaccion(Transaccion* trans) {
    transacciones.push_back(trans);
    gestor.guardar(transacciones, "transacciones.txt");
}

CajaAhorro* Banco::buscarCuentaPorCBU(string cbu) {
    for (auto& cliente : clientes) {
        for (auto& cuenta : cliente->getCuentas()) {
            if (cuenta->getCBU() == cbu) {
                return cuenta;
            }
        }
    }
    return nullptr;
}

Cliente* Banco::buscarClientePorCBU(string cbu) {
    for (auto& cliente : clientes) {
        for (auto& cuenta : cliente->getCuentas()) {
            if (cuenta->getCBU() == cbu) {
                return cliente;
            }
        }
    }
    return nullptr;
}

bool Banco::realizarDeposito(string cbu, double monto, string denominacion, double idClienteOperador) {
    if (monto <= 0) {
        throw runtime_error("El monto debe ser positivo");
    }
    
    CajaAhorro* cuenta = buscarCuentaPorCBU(cbu);
    if (!cuenta) {
        throw runtime_error("No existe cuenta con CBU: " + cbu);
    }
    
    // Validar denominacion
    if (denominacion != "ARS" && denominacion != "USD" && denominacion != "EUR") {
        throw runtime_error("Denominacion invalida. Use ARS, USD o EUR");
    }
    
    // Si la denominacion es diferente, convertir
    string denominacionCuenta = cuenta->getSaldo().getDenominacion();
    double montoFinal = monto;
    
    if (denominacion != denominacionCuenta) {
        montoFinal = convertirMoneda(monto, denominacion, denominacionCuenta);
    }
    
    // Usar el metodo depositar() de la cuenta
    if (!cuenta->depositar(Moneda(montoFinal, denominacionCuenta))) {
        throw runtime_error("Error al realizar el deposito");
    }
    
    // Registrar transaccion (en la denominacion de la cuenta, con IDCliente del operador)
    Transaccion* trans = new Transaccion(cbu, Moneda(montoFinal, denominacionCuenta), idClienteOperador);
    registrarTransaccion(trans);
    
    // Guardar cuentas actualizadas
    gestor.guardarCuentas(clientes, "cuentas.txt");
    
    return true;
}

bool Banco::realizarExtraccion(string cbu, double monto, string denominacion, double idClienteOperador) {
    if (monto <= 0) {
        throw runtime_error("El monto debe ser positivo");
    }
    
    CajaAhorro* cuenta = buscarCuentaPorCBU(cbu);
    if (!cuenta) {
        throw runtime_error("No existe cuenta con CBU: " + cbu);
    }
    
    // VALIDAR QUE LA CUENTA PERTENEZCA AL CLIENTE QUE OPERA
    Cliente* duenoCuenta = buscarClientePorCBU(cbu);
    if (!duenoCuenta || duenoCuenta->getID() != idClienteOperador) {
        throw runtime_error("PERMISO DENEGADO: Solo puede extraer de sus propias cuentas");
    }
    
    // Validar denominacion
    if (denominacion != "ARS" && denominacion != "USD" && denominacion != "EUR") {
        throw runtime_error("Denominacion invalida. Use ARS, USD o EUR");
    }
    
    // Si la denominacion es diferente, convertir
    string denominacionCuenta = cuenta->getSaldo().getDenominacion();
    double montoFinal = monto;
    
    if (denominacion != denominacionCuenta) {
        montoFinal = convertirMoneda(monto, denominacion, denominacionCuenta);
    }
    
    // Usar el metodo extraer() de la cuenta (ya valida saldo internamente)
    if (!cuenta->extraer(Moneda(montoFinal, denominacionCuenta))) {
        Moneda saldoActual = cuenta->getSaldo();
        throw runtime_error("Saldo insuficiente. Saldo actual: " + 
                          to_string(saldoActual.getMonto()) + " " + denominacionCuenta);
    }
    
    // Registrar transaccion (en la denominacion de la cuenta, monto negativo, con IDCliente)
    Transaccion* trans = new Transaccion(cbu, Moneda(-montoFinal, denominacionCuenta), idClienteOperador);
    registrarTransaccion(trans);
    
    // Guardar cuentas actualizadas
    gestor.guardarCuentas(clientes, "cuentas.txt");
    
    return true;
}

Cliente* Banco::buscarClientePorID(double id) {
    for (auto& cliente : clientes) {
        if (cliente->getID() == id) {
            return cliente;
        }
    }
    return nullptr;
}

bool Banco::crearCuentaParaCliente(double idCliente, string denominacion) {
    Cliente* cliente = buscarClientePorID(idCliente);
    if (!cliente) {
        throw runtime_error("Cliente no encontrado");
    }
    
    if (!cliente->getEstado()) {
        throw runtime_error("No se puede crear cuenta para cliente con estado BAJA");
    }
    
    // Validar denominacion
    if (denominacion != "ARS" && denominacion != "USD" && denominacion != "EUR") {
        throw runtime_error("Denominacion invalida. Use ARS, USD o EUR");
    }
    
    // Crear nueva cuenta
    CajaAhorro* nuevaCuenta = new CajaAhorro(denominacion);
    cliente->agregarCuenta(nuevaCuenta);
    
    // Guardar en archivo
    gestor.guardarCuentas(clientes, "cuentas.txt");
    
    return true;
}

bool Banco::eliminarCuentaDeCliente(double idCliente, string cbu) {
    Cliente* cliente = buscarClientePorID(idCliente);
    if (!cliente) {
        throw runtime_error("Cliente no encontrado");
    }
    
    // Buscar la cuenta en el vector del cliente
    vector<CajaAhorro*> cuentas = cliente->getCuentas();
    bool encontrada = false;
    
    for (size_t i = 0; i < cuentas.size(); i++) {
        if (cuentas[i]->getCBU() == cbu) {
            // Verificar que la cuenta tenga saldo 0
            if (cuentas[i]->getSaldo().getMonto() != 0) {
                throw runtime_error("No se puede eliminar cuenta con saldo diferente de 0");
            }
            encontrada = true;
            break;
        }
    }
    
    if (!encontrada) {
        throw runtime_error("Cuenta no encontrada para este cliente");
    }
    
    // Eliminar la cuenta del cliente
    cliente->eliminarCuenta(cbu);
    
    // Guardar cambios
    gestor.guardarCuentas(clientes, "cuentas.txt");
    
    return true;
}

void Banco::guardarTarjetas() {
    gestor.guardarTarjetas(clientes, "tarjetas.txt");
}

Banco::~Banco() {
    // Guardar todos los datos antes de destruir
    try {
        gestor.guardar(clientes, "clientes.txt");
        gestor.guardarCuentas(clientes, "cuentas.txt");
        gestor.guardarTarjetas(clientes, "tarjetas.txt");
        gestor.guardar(transacciones, "transacciones.txt");
        cout << GREEN << "\n[OK] Datos guardados correctamente" << RESET << endl;
    } catch (const exception& e) {
        cerr << RED << "Error al guardar datos: " << e.what() << RESET << endl;
    }
    
    // Liberar memoria
    for (auto cliente : clientes) {
        delete cliente;
    }
    for (auto trans : transacciones) {
        delete trans;
    }
}