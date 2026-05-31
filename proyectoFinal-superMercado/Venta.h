#pragma once
#include <string>
using namespace std;

class Venta {
private:
    int id_venta;
    int no_factura;
    string serie;
    string fecha_factura;
    int id_cliente;
    int id_empleado;
public:
    Venta();
    void registrarVenta();
    void listarVentas();
    int siguienteFactura();
};