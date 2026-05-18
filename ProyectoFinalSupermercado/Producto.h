#pragma once
#include <string>
using namespace std;
class Producto {
private:
    int id_producto;
    string producto;
    int id_marca;
    string descripcion;
    string imagen;
    double precio_costo;
    double precio_venta;
    int existencia;
public:
    Producto();
    Producto(int id, string prod, int marca, string desc, string img, double costo, double venta, int stock);
    void setIdProducto(int id); void setProducto(string val); void setIdMarca(int val); void setDescripcion(string val); void setImagen(string val); void setPrecioCosto(double val); void setPrecioVenta(double val); void setExistencia(int val);
    int getIdProducto(); string getProducto(); int getIdMarca(); string getDescripcion(); string getImagen(); double getPrecioCosto(); double getPrecioVenta(); int getExistencia();
    void crear(); void leer(); void actualizar(); void borrar(); bool existe(int id); double obtenerPrecioVenta(int id); int obtenerExistencia(int id); bool descontarInventario(int id, int cantidad); bool sumarInventario(int id, int cantidad);
};
