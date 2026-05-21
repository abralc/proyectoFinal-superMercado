#pragma once
#include <string>
using namespace std;
class Proveedor {
private:
    int id_proveedor;
    string proveedor, nit, direccion, telefono;
public:
    Proveedor();
    Proveedor(int id, string prov, string nitProv, string dir, string tel);
    void setIdProveedor(int id); void setProveedor(string val); void setNit(string val); void setDireccion(string val); void setTelefono(string val);
    int getIdProveedor(); string getProveedor(); string getNit(); string getDireccion(); string getTelefono();
    void crear(); void leer(); void actualizar(); void borrar(); bool existe(int id); bool existeNit(string nitProv, int id_excluir = 0);
};
