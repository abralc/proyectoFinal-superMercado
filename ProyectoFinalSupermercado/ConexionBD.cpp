#include "ConexionBD.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

ConexionBD::ConexionBD() {
    conector = nullptr;
}

bool ConexionBD::abrirConexion() {
    const char* servidor = "localhost";
    const char* usuario = "root";
    const char* password = "D@vid0rellana";
    const char* base_datos = "db_supermercado";
    unsigned int puerto = 3306;

  

    conector = mysql_init(nullptr);

    if (conector == nullptr) {
        cout << "ALERTA: No se pudo iniciar MySQL." << endl;
        return false;
    }

    MYSQL* resultado = mysql_real_connect(
        conector,
        servidor,
        usuario,
        password,
        base_datos,
        puerto,
        nullptr,
        0
    );

    if (resultado == nullptr) {
        cout << "ALERTA: Error de conexion: " << mysql_error(conector) << endl;
        mysql_close(conector);
        conector = nullptr;
        return false;
    }

    mysql_set_character_set(conector, "utf8mb4");
    return true;
}

MYSQL* ConexionBD::getConector() {
    return conector;
}

void ConexionBD::cerrarConexion() {
    if (conector != nullptr) {
        mysql_close(conector);
        conector = nullptr;
    }
}

string ConexionBD::escapar(string texto) {
    if (conector == nullptr) {
        return "";
    }

    char* buffer = new char[texto.length() * 2 + 1];
    unsigned long longitud = mysql_real_escape_string(conector, buffer, texto.c_str(), (unsigned long)texto.length());
    string resultado(buffer, longitud);
    delete[] buffer;
    return resultado;
}

string ConexionBD::textoSQL(string texto) {
    return "'" + escapar(texto) + "'";
}

string ConexionBD::numeroSQL(int numero) {
    return to_string(numero);
}

string ConexionBD::decimalSQL(double numero) {
    stringstream ss;
    ss << fixed << setprecision(2) << numero;
    return ss.str();
}

bool ConexionBD::ejecutar(string consulta) {
    if (mysql_query(conector, consulta.c_str()) == 0) {
        return true;
    }

    cout << "ALERTA SQL: " << mysql_error(conector) << endl;
    return false;
}

bool ConexionBD::existe(string consulta) {
    MYSQL_RES* resultado;
    bool encontrado = false;

    if (mysql_query(conector, consulta.c_str()) == 0) {
        resultado = mysql_store_result(conector);

        if (resultado != nullptr && mysql_num_rows(resultado) > 0) {
            encontrado = true;
        }

        if (resultado != nullptr) {
            mysql_free_result(resultado);
        }
    }
    else {
        cout << "ALERTA SQL: " << mysql_error(conector) << endl;
    }

    return encontrado;
}

int ConexionBD::obtenerEntero(string consulta) {
    MYSQL_RES* resultado;
    MYSQL_ROW fila;
    int valor = 0;

    if (mysql_query(conector, consulta.c_str()) == 0) {
        resultado = mysql_store_result(conector);
        fila = mysql_fetch_row(resultado);

        if (fila != nullptr && fila[0] != nullptr) {
            valor = atoi(fila[0]);
        }

        mysql_free_result(resultado);
    }
    else {
        cout << "ALERTA SQL: " << mysql_error(conector) << endl;
    }

    return valor;
}

double ConexionBD::obtenerDecimal(string consulta) {
    MYSQL_RES* resultado;
    MYSQL_ROW fila;
    double valor = 0.00;

    if (mysql_query(conector, consulta.c_str()) == 0) {
        resultado = mysql_store_result(conector);
        fila = mysql_fetch_row(resultado);

        if (fila != nullptr && fila[0] != nullptr) {
            valor = atof(fila[0]);
        }

        mysql_free_result(resultado);
    }
    else {
        cout << "ALERTA SQL: " << mysql_error(conector) << endl;
    }

    return valor;
}
