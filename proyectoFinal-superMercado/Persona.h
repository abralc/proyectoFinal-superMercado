#pragma once
#include <string>

using namespace std;

class Persona {
protected:
    string nombres;
    string apellidos;
    string direccion;
    string telefono;
    int genero;

public:
    Persona();
    Persona(string nom, string ape, string dir, string tel, int gen);

    void setNombres(string valor);
    void setApellidos(string valor);
    void setDireccion(string valor);
    void setTelefono(string valor);
    void setGenero(int valor);

    string getNombres();
    string getApellidos();
    string getDireccion();
    string getTelefono();
    int getGenero();
};
