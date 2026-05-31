#include "Persona.h"

Persona::Persona() {
    nombres = "";
    apellidos = "";
    direccion = "";
    telefono = "";
    genero = 1;
}

Persona::Persona(string nom, string ape, string dir, string tel, int gen) {
    nombres = nom;
    apellidos = ape;
    direccion = dir;
    telefono = tel;
    genero = gen;
}

void Persona::setNombres(string valor) { nombres = valor; }
void Persona::setApellidos(string valor) { apellidos = valor; }
void Persona::setDireccion(string valor) { direccion = valor; }
void Persona::setTelefono(string valor) { telefono = valor; }
void Persona::setGenero(int valor) { genero = valor; }

string Persona::getNombres() { return nombres; }
string Persona::getApellidos() { return apellidos; }
string Persona::getDireccion() { return direccion; }
string Persona::getTelefono() { return telefono; }
int Persona::getGenero() { return genero; }
