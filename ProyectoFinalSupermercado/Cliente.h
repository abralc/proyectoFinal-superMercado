#pragma once
#include "Persona.h"
#include <string>
using namespace std;

class Cliente : public Persona {
private:
    int id_cliente;
    string nit;
    string correo_electronico;
public:
    Cliente();
    Cliente(int id, string nom, string ape, string nitCliente, int gen, string tel, string correo);
    void setIdCliente(int id);
    void setNit(string valor);
    void setCorreoElectronico(string valor);
    int getIdCliente();
    string getNit();
    string getCorreoElectronico();
    void crear();
    void leer();
    void actualizar();
    void borrar();
    bool existe(int id);
    bool existeNit(string nitCliente, int id_excluir = 0);
    int obtenerIdPorNit(string nitCliente);
    int crearAutomaticoPorNit(string nitCliente);
};
