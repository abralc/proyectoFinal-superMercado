#include "Venta.h"
#include "ConexionBD.h"
#include "Validaciones.h"
#include "Cliente.h"
#include "Producto.h"
#include "Factura.h"
#include <iostream>
#include <vector>
#include <mysql.h>
using namespace std;

Venta::Venta() { id_venta = 0; no_factura = 0; serie = "A"; fecha_factura = ""; id_cliente = 0; id_empleado = 0; }

int Venta::siguienteFactura() {
    ConexionBD cn;
    if (!cn.abrirConexion()) return 1;
    int factura = cn.obtenerEntero("SELECT IFNULL(MAX(no_factura),0)+1 FROM ventas WHERE serie='A'");
    cn.cerrarConexion();
    return factura;
}

void Venta::registrarVenta() {
    ConexionBD cn;
    Cliente cliente;
    Producto producto;
    vector<LineaFactura> detalle;
    string nit, fecha;
    int idEmpleado;
    double total = 0.00;

    cout << "\n===== REGISTRO DE VENTA =====\n";
    nit = Validaciones::pedirNit("Ingrese NIT del cliente (o C/F): ");
    id_cliente = cliente.obtenerIdPorNit(nit);

    if (id_cliente == 0) {
        cout << "Cliente no existe. Se creara automaticamente con datos basicos.\n";
        id_cliente = cliente.crearAutomaticoPorNit(nit);
    }

    idEmpleado = Validaciones::pedirEntero("Ingrese ID del empleado vendedor: ");
    fecha = Validaciones::pedirFecha("Ingrese fecha de factura YYYY-MM-DD: ");

    do {
        int idProducto = Validaciones::pedirEntero("Ingrese ID producto: ");
        if (!producto.existe(idProducto)) {
            cout << "ALERTA: producto inexistente.\n";
            continue;
        }

        int existencia = producto.obtenerExistencia(idProducto);
        cout << "Existencia actual: " << existencia << endl;
        int cantidad = Validaciones::pedirEntero("Ingrese cantidad: ");

        if (cantidad > existencia) {
            cout << "ALERTA: no hay existencia suficiente.\n";
            continue;
        }

        double precio = producto.obtenerPrecioVenta(idProducto);
        double subtotal = precio * cantidad;
        total += subtotal;

        ConexionBD consultaProducto;
        string nombreProducto = "Producto";
        if (consultaProducto.abrirConexion()) {
            MYSQL_RES* res;
            MYSQL_ROW row;
            string sql = "SELECT producto FROM productos WHERE id_producto=" + consultaProducto.numeroSQL(idProducto);
            if (mysql_query(consultaProducto.getConector(), sql.c_str()) == 0) {
                res = mysql_store_result(consultaProducto.getConector());
                row = mysql_fetch_row(res);
                if (row != nullptr && row[0] != nullptr) nombreProducto = row[0];
                mysql_free_result(res);
            }
            consultaProducto.cerrarConexion();
        }

        LineaFactura linea;
        linea.id_producto = idProducto;
        linea.producto = nombreProducto;
        linea.cantidad = cantidad;
        linea.precio_unitario = precio;
        linea.subtotal = subtotal;
        detalle.push_back(linea);

        string continuar;
        cout << "Agregar otro producto? S/N: ";
        getline(cin, continuar);
        if (continuar != "S" && continuar != "s") break;

    } while (true);

    if (detalle.empty()) {
        cout << "Venta cancelada. No hay productos.\n";
        return;
    }

    if (!cn.abrirConexion()) return;

    no_factura = siguienteFactura();
    serie = "A";
    fecha_factura = fecha;
    id_empleado = idEmpleado;

    mysql_query(cn.getConector(), "START TRANSACTION");

    string sqlVenta = "INSERT INTO ventas(no_factura, serie, fecha_factura, id_cliente, id_empleado, fecha_ingreso) VALUES(" +
        cn.numeroSQL(no_factura) + "," + cn.textoSQL(serie) + "," + cn.textoSQL(fecha_factura) + "," + cn.numeroSQL(id_cliente) + "," + cn.numeroSQL(id_empleado) + ",NOW())";

    bool ok = cn.ejecutar(sqlVenta);
    int idVentaNueva = (int)mysql_insert_id(cn.getConector());

    for (LineaFactura item : detalle) {
        string sqlDetalle = "INSERT INTO ventas_detalle(id_venta,id_producto,cantidad,precio_unitario) VALUES(" +
            cn.numeroSQL(idVentaNueva) + "," + cn.numeroSQL(item.id_producto) + "," + cn.numeroSQL(item.cantidad) + "," + cn.decimalSQL(item.precio_unitario) + ")";

        if (!cn.ejecutar(sqlDetalle)) ok = false;

        string sqlInv = "UPDATE productos SET existencia=existencia-" + cn.numeroSQL(item.cantidad) +
            " WHERE id_producto=" + cn.numeroSQL(item.id_producto) + " AND existencia>=" + cn.numeroSQL(item.cantidad);

        if (!cn.ejecutar(sqlInv)) ok = false;
    }

    if (ok) {
        mysql_query(cn.getConector(), "COMMIT");
        cout << "Venta registrada correctamente.\n";
        Factura::imprimirVenta(no_factura, serie, fecha_factura, nit, "Cliente", detalle, total);
    }
    else {
        mysql_query(cn.getConector(), "ROLLBACK");
        cout << "ALERTA: venta revertida por error.\n";
    }

    cn.cerrarConexion();
}

void Venta::listarVentas() {
    ConexionBD cn; MYSQL_RES* res; MYSQL_ROW row;
    if (!cn.abrirConexion()) return;
    string sql = "SELECT v.id_venta,v.no_factura,v.serie,v.fecha_factura,c.nit,CONCAT(c.nombres,' ',c.apellidos),e.nombres FROM ventas v INNER JOIN clientes c ON v.id_cliente=c.id_cliente INNER JOIN empleados e ON v.id_empleado=e.id_empleado ORDER BY v.id_venta DESC";
    if (mysql_query(cn.getConector(), sql.c_str()) == 0) {
        res = mysql_store_result(cn.getConector());
        cout << "\n===== VENTAS =====\n";
        while ((row = mysql_fetch_row(res))) cout << row[0] << " | Factura " << row[2] << "-" << row[1] << " | Fecha: " << row[3] << " | NIT: " << row[4] << " | Cliente: " << row[5] << " | Empleado: " << row[6] << endl;
        mysql_free_result(res);
    } else cout << "ALERTA SQL: " << mysql_error(cn.getConector()) << endl;
    cn.cerrarConexion();
}
