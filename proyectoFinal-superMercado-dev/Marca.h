#pragma once
#include <string>
using namespace std;

class Marca {
private:
    int id_marca;
    string marca;
public:
    Marca();
    Marca(int id, string nom);
    void setIdMarca(int id);
    void setMarca(string nom);
    int getIdMarca();
    string getMarca();
    void crear();
    void leer();
    void actualizar();
    void borrar();
    bool existe(int id);
    bool existeNombre(string nom, int id_excluir = 0);
};
