#include "Validaciones.h"
#include <iostream>
#include <regex>
#include <ctime>

using namespace std;

bool Validaciones::soloNumeros(string texto) {
    if (texto.empty()) return false;
    for (char c : texto) {
        if (!isdigit((unsigned char)c)) return false;
    }
    return true;
}

bool Validaciones::soloLetrasEspacios(string texto) {
    if (texto.empty()) return false;
    string permitidos = "abcdefghijklmnñopqrstuvwxyzABCDEFGHIJKLMNÑOPQRSTUVWXYZ áéíóúÁÉÍÓÚ";
    for (char c : texto) {
        if (permitidos.find(c) == string::npos) return false;
    }
    return true;
}

bool Validaciones::textoObligatorio(string texto, int maximo) {
    if (texto.empty()) return false;
    if ((int)texto.length() > maximo) return false;
    if (contienePeligroso(texto)) return false;
    return true;
}

bool Validaciones::telefonoValido(string telefono) {
    return soloNumeros(telefono) && telefono.length() >= 8 && telefono.length() <= 25;
}

bool Validaciones::nitValido(string nit) {
    if (nit == "C/F" || nit == "c/f" || nit == "CF" || nit == "cf") return true;
    regex patron("^[0-9]{4,10}-?[0-9Kk]$");
    return regex_match(nit, patron);
}

bool Validaciones::correoValido(string correo) {
    if (correo.empty()) return true;
    regex patron("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
    return regex_match(correo, patron);
}

bool Validaciones::fechaValida(string fecha) {
    regex patron("^\\d{4}-\\d{2}-\\d{2}$");
    if (!regex_match(fecha, patron)) return false;

    int anio = stoi(fecha.substr(0, 4));
    int mes = stoi(fecha.substr(5, 2));
    int dia = stoi(fecha.substr(8, 2));

    if (anio < 1900 || mes < 1 || mes > 12) return false;

    int diasMes[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    bool bisiesto = (anio % 400 == 0) || (anio % 4 == 0 && anio % 100 != 0);
    if (bisiesto) diasMes[1] = 29;

    return dia >= 1 && dia <= diasMes[mes - 1];
}

bool Validaciones::fechaNoFutura(string fecha) {
    if (!fechaValida(fecha)) return false;

    time_t ahora = time(0);
    tm local;
    localtime_s(&local, &ahora);

    int anioActual = local.tm_year + 1900;
    int mesActual = local.tm_mon + 1;
    int diaActual = local.tm_mday;

    int anio = stoi(fecha.substr(0, 4));
    int mes = stoi(fecha.substr(5, 2));
    int dia = stoi(fecha.substr(8, 2));

    if (anio > anioActual) return false;
    if (anio == anioActual && mes > mesActual) return false;
    if (anio == anioActual && mes == mesActual && dia > diaActual) return false;
    return true;
}

bool Validaciones::decimalPositivo(string texto) {
    regex patron("^[0-9]+(\\.[0-9]{1,2})?$");
    return regex_match(texto, patron) && stod(texto) >= 0;
}

bool Validaciones::enteroPositivo(string texto) {
    return soloNumeros(texto) && stoi(texto) > 0;
}

bool Validaciones::contienePeligroso(string texto) {
    string peligrosos[] = { "'", "\"", ";", "--", "/*", "*/", "\\", "DROP", "drop", "DELETE", "delete", "UPDATE", "update", "INSERT", "insert", "SELECT", "select" };
    for (string p : peligrosos) {
        if (texto.find(p) != string::npos) return true;
    }
    return false;
}

string Validaciones::pedirTexto(string mensaje, int maximo) {
    string texto;
    do {
        cout << mensaje;
        getline(cin, texto);
        if (textoObligatorio(texto, maximo)) return texto;
        cout << "ALERTA: dato obligatorio, maximo " << maximo << " caracteres y sin caracteres peligrosos.\n";
    } while (true);
}

string Validaciones::pedirLetras(string mensaje, int maximo) {
    string texto;
    do {
        cout << mensaje;
        getline(cin, texto);
        if (!texto.empty() && (int)texto.length() <= maximo && soloLetrasEspacios(texto)) return texto;
        cout << "ALERTA: solo letras y espacios, maximo " << maximo << " caracteres.\n";
    } while (true);
}

string Validaciones::pedirTelefono(string mensaje) {
    string telefono;
    do {
        cout << mensaje;
        getline(cin, telefono);
        if (telefonoValido(telefono)) return telefono;
        cout << "ALERTA: telefono obligatorio, solo numeros, minimo 8 digitos.\n";
    } while (true);
}

string Validaciones::pedirNit(string mensaje) {
    string nit;
    do {
        cout << mensaje;
        getline(cin, nit);
        if (nitValido(nit)) return nit;
        cout << "ALERTA: NIT invalido. Use formato 1234567-8 o C/F.\n";
    } while (true);
}

string Validaciones::pedirCorreoOpcional(string mensaje) {
    string correo;
    do {
        cout << mensaje;
        getline(cin, correo);
        if (correoValido(correo)) return correo;
        cout << "ALERTA: correo invalido.\n";
    } while (true);
}

string Validaciones::pedirFecha(string mensaje) {
    string fecha;
    do {
        cout << mensaje;
        getline(cin, fecha);
        if (fechaNoFutura(fecha)) return fecha;
        cout << "ALERTA: fecha invalida. Use YYYY-MM-DD y no futura.\n";
    } while (true);
}

int Validaciones::pedirEntero(string mensaje) {
    string texto;
    do {
        cout << mensaje;
        getline(cin, texto);
        if (enteroPositivo(texto)) return stoi(texto);
        cout << "ALERTA: ingrese un numero entero mayor que cero.\n";
    } while (true);
}

int Validaciones::pedirOpcion(string mensaje) {
    string texto;
    do {
        cout << mensaje;
        getline(cin, texto);
        if (soloNumeros(texto)) return stoi(texto);
        cout << "ALERTA: ingrese una opcion numerica.\n";
    } while (true);
}

double Validaciones::pedirDecimal(string mensaje) {
    string texto;
    do {
        cout << mensaje;
        getline(cin, texto);
        if (decimalPositivo(texto)) return stod(texto);
        cout << "ALERTA: ingrese un valor decimal valido mayor o igual a cero.\n";
    } while (true);
}

int Validaciones::pedirGenero() {
    string texto;
    do {
        cout << "Genero (1 masculino, 0 femenino): ";
        getline(cin, texto);
        if (texto == "1") return 1;
        if (texto == "0") return 0;
        cout << "ALERTA: ingrese 1 o 0.\n";
    } while (true);
}
