#include "Compra.h"
#include "ConexionBD.h"
#include "Validaciones.h"
#include "Producto.h"
#include <iostream>
#include <vector>
#include <mysql.h>
using namespace std;

Compra::Compra() { id_compra = 0; no_orden_compra = 0; id_proveedor = 0; id_empleado = 0; }

int Compra::siguienteOrden() {
    ConexionBD cn;
    if (!cn.abrirConexion()) return 1001;
    int orden = cn.obtenerEntero("SELECT IFNULL(MAX(no_orden_compra),1000)+1 FROM compras");
    cn.cerrarConexion();
    return orden;
}

void Compra::registrarCompra() {
    struct ItemCompra {
        int id_producto;
        int cantidad;
        double precio;
    };

    vector<ItemCompra> detalle;
    Producto producto;
    string fecha;

    cout << "\n===== REGISTRO DE COMPRA =====\n";
    id_proveedor = Validaciones::pedirEntero("Ingrese ID proveedor: ");
    id_empleado = Validaciones::pedirEntero("Ingrese ID empleado responsable: ");
    fecha = Validaciones::pedirFecha("Ingrese fecha de orden YYYY-MM-DD: ");

    do {
        int idProducto = Validaciones::pedirEntero("Ingrese ID producto: ");
        if (!producto.existe(idProducto)) {
            cout << "ALERTA: producto inexistente.\n";
            continue;
        }

        int cantidad = Validaciones::pedirEntero("Ingrese cantidad comprada: ");
        double costo = Validaciones::pedirDecimal("Ingrese precio costo unitario: ");

        ItemCompra item;
        item.id_producto = idProducto;
        item.cantidad = cantidad;
        item.precio = costo;
        detalle.push_back(item);

        string continuar;
        cout << "Agregar otro producto? S/N: ";
        getline(cin, continuar);
        if (continuar != "S" && continuar != "s") break;

    } while (true);

    if (detalle.empty()) {
        cout << "Compra cancelada. No hay productos.\n";
        return;
    }

    ConexionBD cn;
    if (!cn.abrirConexion()) return;

    no_orden_compra = siguienteOrden();
    mysql_query(cn.getConector(), "START TRANSACTION");

    string sqlCompra = "INSERT INTO compras(no_orden_compra,id_proveedor,fecha_orden,fecha_ingreso) VALUES(" +
        cn.numeroSQL(no_orden_compra) + "," + cn.numeroSQL(id_proveedor) + "," + cn.textoSQL(fecha) + ",NOW())";

    bool ok = cn.ejecutar(sqlCompra);
    int idCompraNueva = (int)mysql_insert_id(cn.getConector());

    for (ItemCompra item : detalle) {
        string sqlDetalle = "INSERT INTO compras_detalle(id_compra,id_producto,cantidad,precio_costo_unitario) VALUES(" +
            cn.numeroSQL(idCompraNueva) + "," + cn.numeroSQL(item.id_producto) + "," + cn.numeroSQL(item.cantidad) + "," + cn.decimalSQL(item.precio) + ")";

        if (!cn.ejecutar(sqlDetalle)) ok = false;

        string sqlInv = "UPDATE productos SET existencia=existencia+" + cn.numeroSQL(item.cantidad) +
            ", precio_costo=" + cn.decimalSQL(item.precio) + " WHERE id_producto=" + cn.numeroSQL(item.id_producto);

        if (!cn.ejecutar(sqlInv)) ok = false;
    }

    if (ok) {
        mysql_query(cn.getConector(), "COMMIT");
        cout << "Compra registrada correctamente. Orden No. " << no_orden_compra << endl;
    }
    else {
        mysql_query(cn.getConector(), "ROLLBACK");
        cout << "ALERTA: compra revertida por error.\n";
    }

    cn.cerrarConexion();
}

void Compra::listarCompras() {
    ConexionBD cn; MYSQL_RES* res; MYSQL_ROW row;
    if (!cn.abrirConexion()) return;
    string sql = "SELECT c.id_compra,c.no_orden_compra,c.fecha_orden,p.proveedor FROM compras c INNER JOIN proveedores p ON c.id_proveedor=p.id_proveedor ORDER BY c.id_compra DESC";
    if (mysql_query(cn.getConector(), sql.c_str()) == 0) {
        res = mysql_store_result(cn.getConector());
        cout << "\n===== COMPRAS =====\n";
        while ((row = mysql_fetch_row(res))) cout << row[0] << " | Orden: " << row[1] << " | Fecha: " << row[2] << " | Proveedor: " << row[3] << endl;
        mysql_free_result(res);
    } else cout << "ALERTA SQL: " << mysql_error(cn.getConector()) << endl;
    cn.cerrarConexion();
}
