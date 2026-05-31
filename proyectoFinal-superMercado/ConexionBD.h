#pragma once
#include <mysql.h>
#include <string>

using namespace std;

class ConexionBD {
private:
    MYSQL* conector;

public:
    ConexionBD();
    bool abrirConexion();
    MYSQL* getConector();
    void cerrarConexion();

    string escapar(string texto);
    string textoSQL(string texto);
    string numeroSQL(int numero);
    string decimalSQL(double numero);

    bool ejecutar(string consulta);
    bool existe(string consulta);
    int obtenerEntero(string consulta);
    double obtenerDecimal(string consulta);
};
