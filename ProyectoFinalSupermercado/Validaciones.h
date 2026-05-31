#pragma once
#include <string>

using namespace std;

class Validaciones {
public:
    static bool soloNumeros(string texto);
    static bool soloLetrasEspacios(string texto);
    static bool textoObligatorio(string texto, int maximo);
    static bool telefonoValido(string telefono);
    static bool nitValido(string nit);
    static bool correoValido(string correo);
    static bool fechaValida(string fecha);
    static bool fechaNoFutura(string fecha);
    static bool decimalPositivo(string texto);
    static bool enteroPositivo(string texto);
    static bool contienePeligroso(string texto);

    static string pedirTexto(string mensaje, int maximo);
    static string pedirLetras(string mensaje, int maximo);
    static string pedirTelefono(string mensaje);
    static string pedirNit(string mensaje);
    static string pedirCorreoOpcional(string mensaje);
    static string pedirFecha(string mensaje);
    static int pedirEntero(string mensaje);
    static int pedirOpcion(string mensaje);
    static double pedirDecimal(string mensaje);
    static int pedirGenero();
};
