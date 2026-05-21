#pragma once
#include <string>
using namespace std;
class Puesto {
private:
    int id_puesto;
    string puesto;
public:
    Puesto();
    Puesto(int id, string nom);
    void setIdPuesto(int id);
    void setPuesto(string nom);
    int getIdPuesto();
    string getPuesto();
    void crear();
    void leer();
    void actualizar();
    void borrar();
    bool existe(int id);
    bool existeNombre(string nom, int id_excluir = 0);
};
