#ifndef LIB_H
#define LIB_H

#include <vector>
#include <map>
#include <string>
#include <ctime>
#include <algorithm>

using namespace std;

class Cliente;

struct GestorArchivos{
    template<typename T>
    void cargar(vector<T*>& datos, const string& NombreArchivo);
    
    template<typename T>
    void guardar(const vector<T*>& datos, const string& NombreArchivo);
    
    // Metodos que requieren contexto de clientes
    void guardarTarjetas(const vector<Cliente*>& clientes, const string& NombreArchivo);
    void guardarCuentas(const vector<Cliente*>& clientes, const string& NombreArchivo);
};

class Moneda{
    private:
        double monto;
        string denominacion;
    public:
        Moneda(double m, string d);
        Moneda(string d);
        Moneda();
        double getMonto();
        string getDenominacion();
        friend Moneda operator+(const Moneda& m1, const Moneda& m2);
        friend Moneda operator-(const Moneda& m1, const Moneda& m2);
        friend ostream& operator<<(ostream& os, const Moneda& m);
        friend bool operator>(const Moneda& m1, const Moneda& m2);
        friend bool operator<(const Moneda& m1, const Moneda& m2);
        ~Moneda() = default;
};

class Fecha{
    private:
        int dia;
        int mes;
        int anio;
    public:
        Fecha();
        Fecha(int dia, int mes, int anio);
        Fecha(string fechaStr);
        int getDia();
        int getMes();
        int getAnio();
        void setDia(int d);
        void setMes(int m);
        void setAnio(int a);
        friend ostream& operator<<(ostream& os, const Fecha& f);
        friend bool operator==(const Fecha& f1, const Fecha& f2);
        ~Fecha() = default;
};

class CajaAhorro{
    private:
        string CBU;
        Moneda saldo;
        void generarCBU();
        friend class GestorArchivos;
    public:
        CajaAhorro(string denominacion);
        bool depositar(Moneda monto);
        bool extraer(Moneda monto);
        Moneda getSaldo();
        string getCBU();
        ~CajaAhorro() = default;
};

class Tarjeta{
    protected:
        Moneda saldo;
        string nombreTarjeta;
        Moneda limite;
    public:
        Tarjeta(string nombre, Moneda lim);
        Moneda getLimite();
        string getNombreTarjeta();
        ~Tarjeta() = default;
};

class Transaccion{
    private:
        int IDTransaccion;
        int IDCliente;
        string CBU;
        Fecha fecha;
        Moneda monto;
        void generarIDTransaccion();
        friend class GestorArchivos;
    public:
        Transaccion(string cbu, Moneda m, int idCliente);
        int getIDTransaccion();
        int getIDCliente();
        string getCBU();
        Fecha getFecha();
        Moneda getMonto();
        ~Transaccion() = default;
};

class Persona{
    protected:
        string DNI;
        string nombre;
        Fecha ingreso;
    public:
        Persona(string dni, string Nombre);
        string getDNI();
        string getNombre();
        Fecha getAnioIngreso();
        virtual ~Persona() = default;
};

class Cliente : public Persona {
    private:
        double ID;
        string tipoCliente;
        bool estado;
        vector<CajaAhorro*> cuentas;
        Tarjeta* tarjeta;
        void generarID();
        friend class GestorArchivos;
    public:
        Cliente(string dni, string Nombre, string Tipo);
        double getID();
        string getTipoCliente();
        bool getEstado();
        vector<CajaAhorro*> getCuentas();
        Tarjeta* getTarjeta();
        void setEstado(bool estado);
        void setTipoCliente(string tipo);
        void agregarCuenta(CajaAhorro* cuenta);
        void eliminarCuenta(string cbu);
        void setTarjeta(Tarjeta* t);
        ~Cliente() = default;
};

class Banco {
    private:
        vector<Cliente*> clientes;
        vector<Transaccion*> transacciones;
        GestorArchivos gestor;
        friend class GestorArchivos;
    public:
        map<pair<string, string>, double> tasasConversion;
        Banco();
        bool altaCliente(Cliente* cliente);
        bool bajaCliente(double id);
        void buscarCliente(double id);
        void listadoClientes();
        void informeTransacciones();
        void informeTransacciones(double id);
        void informeTransacciones(Fecha desde, Fecha hasta);
        double convertirMoneda(double monto, string desde, string hacia);
        void registrarTransaccion(Transaccion* trans);
        CajaAhorro* buscarCuentaPorCBU(string cbu);
        Cliente* buscarClientePorCBU(string cbu);
        bool realizarDeposito(string cbu, double monto, string denominacion, double idClienteOperador);
        bool realizarExtraccion(string cbu, double monto, string denominacion, double idClienteOperador);
        Cliente* buscarClientePorID(double id);
        bool crearCuentaParaCliente(double idCliente, string denominacion);
        bool eliminarCuentaDeCliente(double idCliente, string cbu);
        void guardarTarjetas();
        ~Banco();
};

#endif