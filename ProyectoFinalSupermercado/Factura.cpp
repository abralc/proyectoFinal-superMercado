#include "Factura.h"
#include <iostream>
#include <iomanip>
#include <fstream>   // para factura
#include <cstdlib>   // para factura
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


//METODO PARA GENERAR FACTURA EN HTML
void Factura::generarFacturaHTML(int noFactura, string serie, string fecha, string nit, string cliente, vector<LineaFactura> detalle, double total) {
    string numFacturaStr = to_string(noFactura);
    string nombreArchivo = "factura_" + numFacturaStr + ".html";

    ofstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cout << "Error al generar la factura HTML." << endl;
        return;
    }

    archivo << R"(
    <!DOCTYPE html>
    <html lang="es">
    <head>
        <meta charset="UTF-8">
        <title>Factura )" << numFacturaStr << R"(</title>
        <style>
            body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; padding: 40px; color: #333; max-width: 800px; margin: auto; }
            .encabezado { text-align: center; margin-bottom: 30px; border-bottom: 2px solid #333; padding-bottom: 10px; }
            .datos-cliente p { margin: 5px 0; }
            .datos-cliente { margin-bottom: 30px; }
            table { width: 100%; border-collapse: collapse; margin-bottom: 20px; }
            th, td { border: 1px solid #ddd; padding: 12px; text-align: left; }
            th { background-color: #f4f4f4; color: #333; }
            .total { text-align: right; font-size: 1.5em; font-weight: bold; margin-top: 20px; }
            .pie { text-align: center; margin-top: 50px; font-style: italic; color: #666; }
        </style>
    </head>
    <body>
        <div class="encabezado">
            <h1>SUPERMERCADO XYZ</h1>
            <h2>Factura de venta</h2>
        </div>
        
        <div class="datos-cliente">
            <p><strong>Factura No:</strong> )" << numFacturaStr << " &nbsp;&nbsp;&nbsp; <strong>Serie:</strong> " << serie << R"(</p>
            <p><strong>Fecha:</strong> )" << fecha << R"(</p>
            <p><strong>NIT:</strong> )" << nit << R"(</p>
            <p><strong>Cliente:</strong> )" << cliente << R"(</p>
        </div>

        <table>
            <thead>
                <tr>
                    <th>COD</th>
                    <th>PRODUCTO</th>
                    <th>CANT</th>
                    <th>PRECIO</th>
                    <th>SUBTOTAL</th>
                </tr>
            </thead>
            <tbody>
    )";

    for (LineaFactura item : detalle) {
        archivo << "<tr>"
            << "<td>" << item.id_producto << "</td>"
            << "<td>" << item.producto << "</td>"
            << "<td>" << item.cantidad << "</td>"
            << "<td>Q " << fixed << setprecision(2) << item.precio_unitario << "</td>"
            << "<td>Q " << fixed << setprecision(2) << item.subtotal << "</td>"
            << "</tr>";
    }

    archivo << R"(
            </tbody>
        </table>
        
        <div class="total">
            TOTAL: Q )" << fixed << setprecision(2) << total << R"(
        </div>
        
        <div class="pie">
            Gracias por su compra
        </div>
    </body>
    </html>
    )";

    archivo.close();

    cout << "Factura HTML generada con exito." << endl;

    // Abrir el archivo en el navegador
    string comando = "start " + nombreArchivo;
    system(comando.c_str());
}
