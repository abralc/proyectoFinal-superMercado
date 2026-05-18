#include "Factura.h"
#include <iostream>
#include <iomanip>
using namespace std;

void Factura::imprimirVenta(int noFactura, string serie, string fecha, string nit, string cliente, vector<LineaFactura> detalle, double total) {
    cout << "\n=============================================\n";
    cout << "             SUPERMERCADO XYZ\n";
    cout << "              FACTURA DE VENTA\n";
    cout << "=============================================\n";
    cout << "Factura No: " << setw(5) << setfill('0') << noFactura << setfill(' ') << "   Serie: " << serie << endl;
    cout << "Fecha: " << fecha << endl;
    cout << "NIT: " << nit << endl;
    cout << "Cliente: " << cliente << endl;
    cout << "---------------------------------------------\n";
    cout << left << setw(6) << "COD" << setw(22) << "PRODUCTO" << right << setw(6) << "CANT" << setw(9) << "PRECIO" << setw(10) << "SUBTOTAL" << endl;
    cout << "---------------------------------------------\n";

    for (LineaFactura item : detalle) {
        cout << left << setw(6) << item.id_producto
             << setw(22) << item.producto.substr(0, 21)
             << right << setw(6) << item.cantidad
             << setw(9) << fixed << setprecision(2) << item.precio_unitario
             << setw(10) << fixed << setprecision(2) << item.subtotal << endl;
    }

    cout << "---------------------------------------------\n";
    cout << right << setw(35) << "TOTAL: Q" << fixed << setprecision(2) << total << endl;
    cout << "          Gracias por su compra\n";
    cout << "=============================================\n";
}
