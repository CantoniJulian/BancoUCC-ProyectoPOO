#include "headers.h"

Tarjeta :: Tarjeta(string nombre, Moneda Lim) : nombreTarjeta(nombre), limite(Lim)
{

}

Moneda Tarjeta :: getLimite()
{
    return limite;
}

Tarjeta :: ~Tarjeta()
{

}

Transaccion :: Transaccion(string cbu, Moneda m) : CBU(cbu), monto(m)
{
    time_t t = time(NULL);
    struct tm* tiempo = localtime(&t);
    fecha = Fecha(tiempo->tm_mday, tiempo->tm_mon, tiempo->tm_year);
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
    IDTransaccion = fecha.getAnio() * 10000 + fecha.getMes() * 1000 + fecha.getDia() *100 + rand() % 100 + 1 ;
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

Persona :: Persona(string dni, string Nombre) : DNI(dni), nombre(Nombre)
{
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

Cliente :: Cliente(string dni, string Nombre, string tipo) : Persona(dni, nombre), tipoCliente(tipo)
{
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

void Cliente :: generarID()
{
    ID = rand() % 10000 +1;
}