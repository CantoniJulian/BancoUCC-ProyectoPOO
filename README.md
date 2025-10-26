# Sistema de Gestión Bancaria UCC

Este diagrama de clases UML modela el tema 2 del proyecto final de Programación Orientada a Objetos. Consiste en un sistema de gestión que permite realizar el registro y control de actividades que se llevan a cabo en la UCC.

```mermaid
---
title: Sistema de Gestión Bancaria (Banco UCC)
---
classDiagram
    direction LR

    %% --- Clases Principales ---
    
    class Banco {
        -list~Cliente*~ clientes
        -list~Transaccion*~ transacciones
        -GestorArchivos gestor
        -map~pair~string,string~, double~ tasasConversion
        +void altaCliente(DNI, Nombre, Tipo, Año)
        +void bajaCliente(DNI)
        +void registrarTransaccion(IDCliente, tipo, Moneda, cuenta)
        +Moneda convertir(Moneda m, string denomDestino)
        +Cliente* buscarCliente(int DNI)
        +list~Cliente*~ listarClientes()
        +list~Transaccion*~ listarTransaccionesPorCliente(int DNI)
        +list~Transaccion*~ informeTransacciones(Fecha filtro)
    }

    class GestorArchivos {
        +template~T~ void guardar(list~T*~ datos, string filename)
        +template~T~ list~T*~ cargar(string filename)
    }

    %% --- Entidades (Persona y Cliente) ---

    class Persona {
        <<abstract>>
        #int DNI
        #string nombre
        #Fecha anioIngreso
    }

    class Cliente {
        -double ID
        -string tipoCliente
        -bool estado
        -list~Cuenta*~ cuentas
        -Tarjeta* tarjetaCredito
        +Tarjeta* getTarjeta()
        +list~Cuenta*~ getCuentas()
    }

    %% --- Productos Bancarios (Cuenta y Tarjeta) ---

    class Cuenta {
        <<abstract>>
        #string CBU
        #Moneda saldo
        +Cuenta(string denominacion)
        +virtual bool depositar(Moneda m) = 0
        +virtual bool extraer(Moneda m) = 0
        +Moneda getSaldo()
        +string getDenominacionSaldo()
    }

    class CajaAhorro {
        +CajaAhorro(string denominacion)
        +bool depositar(Moneda m)
        +bool extraer(Moneda m)
    }

    class Tarjeta {
        <<abstract>>
        #string nombreTarjeta
        #Moneda limite
        +Moneda getLimite()
    }

    class TarjetaCredix {
        -Moneda limite(250000, "ARS")
        -string nombreTarjeta("Credix")
    }

    class TarjetaPremium {
        -Moneda limite(500000, "ARS")
        -string nombreTarjeta("Premium")
    }

    %% --- Clases de Datos y Utilidad ---

    class Transaccion {
        -int IDTransaccion
        -int IDCliente
        -Fecha fecha
        -string tipo
        -Moneda monto
        +friend ostream& operator<<(ostream& os, const Transaccion& t)
    }

    class Fecha {
        -int dia
        -int mes
        -int anio
        +Fecha(string fechaStr)
        +int getDia()
        +int getMes()
        +int getAnio()
        +friend bool operator==(const Fecha& a, const Fecha& b)
    }

    class Moneda {
        -double monto
        -string denominacion
        +Moneda(double m, string d)
        +double getMonto()
        +string getDenominacion()
        +friend Moneda operator+(const Moneda& a, const Moneda& b)
        +friend Moneda operator-(const Moneda& a, const Moneda& b)
        +friend ostream& operator<<(ostream& os, const Moneda& m)
    }

    %% --- Relaciones ---

    Banco "1" *-- "1" GestorArchivos : utiliza
    Banco "1" *-- "0..*" Cliente : gestiona
    Banco "1" *-- "0..*" Transaccion : registra

    Persona <|-- Cliente

    Cliente "1" *-- "0..*" Cuenta : posee
    Cliente "1" *-- "0..1" Tarjeta : posee

    Cuenta <|-- CajaAhorro

    Tarjeta <|-- TarjetaCredix
    Tarjeta <|-- TarjetaPremium

    Transaccion "1" -- "1" Fecha : ocurreEn
    Transaccion "1" -- "1" Moneda : tieneUn
    Cuenta "1" -- "1" Moneda : tieneUnSaldo
    Tarjeta "1" -- "1" Moneda : tieneUnLimite
```

## Descripción del diagrama
La clase central del sistema es **Banco**, que actúa como el facade o **gestor principal**, administrando las **colecciones de Cliente y Transaccion**. Utiliza un **GestorArchivos** para la persistencia de datos y mantiene un registro de tasas de conversión para operaciones con divisas. Realiza las siguientes funciones:
- Alta/Baja de clientes, 
- registro de transacciones,
- búsqueda, 
- listado,
- informes (incluyendo conversión interna de Moneda)

El **GestorArchivos** implementa la lógica de persistencia de datos al guardar y cargar colecciones de objetos (template T) de forma segura, validando el tipo de dato en el tiempo de ejecución y evitando que el proceso principal se congele.

El sistema distingue entre la información básica de una persona y el rol de cliente bancario.
- Persona (<<abstract>>): clase base que define los atributos de identificación comunes: DNI, nombre y el anioIngreso (utilizando la clase Fecha).
- Cliente: hereda de Persona y añade atributos específicos del cliente (ID, tipoCliente, estado). Un cliente posee múltiples **Cuenta**s y puede poseer una única Tarjeta.

Además, se define una jerarquía para los productos financieros, basada en clases abstractas.
- ***Cuentas***
→ **Cuenta (<<abstract>>):** clase base para todos los tipos de cuentas. Define el CBU y el saldo (de tipo Moneda). Establece la interfaz para las operaciones básicas con métodos virtuales puros: depositar() y extraer().
	→ **CajaAhorro:** implementación concreta de Cuenta, proporcionando la lógica específica para las operaciones de depósito y extracción.

- ***Tarjetas***
→ **Tarjeta (<<abstract>>):** clase base que establece la interfaz para las tarjetas, definiendo un nombreTarjeta y un limite (de tipo Moneda).
	→ **TarjetaCredix y TarjetaPremium:** implementaciones concretas de Tarjeta, que se diferencian principalmente por su limite de crédito predefinido (e.g., $250.000 y $500.000 ARS, respectivamente).

Finalmente, se usan clases adicionales para el formato de los datos como Moneda, Fecha y Transacción
- **Moneda:** encapsula un monto (double) junto a su denominacion (string). Sobrecarga los operadores de suma (+) y resta (-) para facilitar operaciones monetarias y permite su output a streams (ostream).
- **Fecha:** almacena día, mes y año. Permite la construcción a partir de un string y sobrecarga el operador de igualdad (==) para comparaciones.
- **Transaccion:** registra las operaciones realizadas. Contiene IDTransaccion, IDCliente, tipo (e.g., "depósito", "extracción"), la fecha y el monto (de tipo Moneda).