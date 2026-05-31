#pragma once
#pragma once
#include <string>
#include <vector>
using namespace std;

struct LineaFactura {
    int id_producto;
    string producto;
    int cantidad;
    double precio_unitario;
    double subtotal;
};

class Factura {
public:
    static void imprimirVenta(int noFactura, string serie, string fecha, string nit, string cliente, vector<LineaFactura> detalle, double total);
    static void generarFacturaHTML(int noFactura, string serie, string fecha, string nit, string cliente, vector<LineaFactura> detalle, double total); //
};
