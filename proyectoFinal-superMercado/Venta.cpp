#include "Venta.h"
#include "ConexionBD.h"
#include "Validaciones.h"
#include "Cliente.h"
#include "Producto.h"
#include "Factura.h"
#include <iostream>
#include <vector>
#include <mysql.h>
#include <ctime>
#include <sstream>
#include <iomanip>
using namespace std;

static string obtenerFechaActual() {
    time_t ahora = time(0);
    tm fechaLocal;
    localtime_s(&fechaLocal, &ahora);

    stringstream ss;
    ss << (fechaLocal.tm_year + 1900) << "-"
        << setw(2) << setfill('0') << (fechaLocal.tm_mon + 1) << "-"
        << setw(2) << setfill('0') << fechaLocal.tm_mday;

    return ss.str();
}

static bool esConsumidorFinal(string nit) {
    return nit == "C/F" || nit == "c/f" || nit == "CF" || nit == "cf";
}

Venta::Venta() {
    id_venta = 0;
    no_factura = 0;
    serie = "A";
    fecha_factura = "";
    id_cliente = 0;
    id_empleado = 0;
}

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

    string nit;
    int idEmpleado;
    double total = 0.00;

    cout << "\n===== REGISTRO DE VENTA =====\n";

    // Cliente por NIT. Permite NIT normal, registro nuevo y Consumidor Final C/F.
    nit = Validaciones::pedirNit("Ingrese NIT del cliente (o C/F): ");

    if (esConsumidorFinal(nit)) {
        nit = "C/F";
    }

    id_cliente = cliente.obtenerIdPorNit(nit);

    if (esConsumidorFinal(nit) && id_cliente == 0) {
        id_cliente = cliente.crearAutomaticoPorNit("C/F");
    }

    if (id_cliente == 0) {
        cout << "ALERTA: El cliente no existe en el sistema.\n";

        string opcion;
        cout << "Desea registrar sus datos manualmente? (S/N): ";
        getline(cin, opcion);

        if (opcion == "S" || opcion == "s") {
            cout << "\n===== REGISTRO DE NUEVO CLIENTE =====\n";
            string nombres = Validaciones::pedirLetras("Nombres: ", 60);
            string apellidos = Validaciones::pedirLetras("Apellidos: ", 60);
            int genero = Validaciones::pedirGenero();
            string telefono = Validaciones::pedirTelefono("Telefono: ");
            string correo = Validaciones::pedirCorreoOpcional("Correo: ");

            Cliente nuevoCliente(0, nombres, apellidos, nit, genero, telefono, correo);
            nuevoCliente.crear();

            id_cliente = nuevoCliente.obtenerIdPorNit(nit);

            if (id_cliente == 0) {
                cout << "Error al crear cliente. Venta cancelada.\n";
                return;
            }
        }
        else {
            cout << "Se creara el cliente de forma automatica con datos basicos.\n";
            id_cliente = cliente.crearAutomaticoPorNit(nit);
        }
    }

    // Validacion de empleado vendedor.
    string nombreEmpleado = "";
    do {
        idEmpleado = Validaciones::pedirEntero("Ingrese ID del empleado vendedor: ");

        ConexionBD consultaEmp;
        if (consultaEmp.abrirConexion()) {
            MYSQL_RES* resEmp;
            MYSQL_ROW rowEmp;
            string sqlEmp = "SELECT CONCAT(nombres, ' ', apellidos) FROM empleados WHERE id_empleado=" + consultaEmp.numeroSQL(idEmpleado);

            if (mysql_query(consultaEmp.getConector(), sqlEmp.c_str()) == 0) {
                resEmp = mysql_store_result(consultaEmp.getConector());
                if ((rowEmp = mysql_fetch_row(resEmp))) {
                    if (rowEmp[0] != nullptr) nombreEmpleado = rowEmp[0];
                }
                mysql_free_result(resEmp);
            }
            consultaEmp.cerrarConexion();
        }

        if (nombreEmpleado.empty()) {
            cout << "ALERTA: Empleado no encontrado. Verifique el ID e intente de nuevo.\n";
        }
        else {
            cout << ">>> Bienvenido(a), " << nombreEmpleado << " <<<\n" << endl;
        }
    } while (nombreEmpleado.empty());

    // Fecha, factura y serie automaticas.
    fecha_factura = obtenerFechaActual();
    no_factura = siguienteFactura();
    serie = "A";
    id_empleado = idEmpleado;

    cout << "Fecha generada automaticamente: " << fecha_factura << endl;
    cout << "Factura generada automaticamente: " << serie << "-" << setw(5) << setfill('0') << no_factura << setfill(' ') << endl;

    // Detalle de productos.
    do {
        int idProducto = Validaciones::pedirEntero("Ingrese ID producto: ");

        if (!producto.existe(idProducto)) {
            cout << "ALERTA: producto inexistente.\n";
            continue;
        }

        string nombreProducto;
        string marcaProducto;
        string imagenProducto;
        int existencia = 0;
        double precio = 0.00;

        if (!producto.obtenerDatosVenta(idProducto, nombreProducto, marcaProducto, existencia, precio, imagenProducto)) {
            cout << "Error al obtener datos del producto.\n";
            continue;
        }

        cout << "Producto: " << nombreProducto << endl;
        cout << "Marca: " << marcaProducto << endl;
        cout << "Precio venta: Q" << fixed << setprecision(2) << precio << endl;
        cout << "Existencia actual: " << existencia << endl;

        int cantidad = Validaciones::pedirEntero("Ingrese cantidad: ");

        if (cantidad > existencia) {
            cout << "ALERTA: no hay existencia suficiente.\n";
            continue;
        }

        double subtotal = precio * cantidad;
        total += subtotal;

        cout << "Subtotal linea: Q" << fixed << setprecision(2) << subtotal << endl;
        cout << "Total acumulado: Q" << fixed << setprecision(2) << total << endl;

        LineaFactura linea;
        linea.id_producto = idProducto;
        linea.producto = nombreProducto;
        linea.marca = marcaProducto;
        linea.imagen = imagenProducto;
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

    mysql_query(cn.getConector(), "START TRANSACTION");

    string sqlVenta = "INSERT INTO ventas(no_factura, serie, fecha_factura, id_cliente, id_empleado, fecha_ingreso) VALUES(" +
        cn.numeroSQL(no_factura) + "," + cn.textoSQL(serie) + "," + cn.textoSQL(fecha_factura) + "," +
        cn.numeroSQL(id_cliente) + "," + cn.numeroSQL(id_empleado) + ",NOW())";

    bool ok = cn.ejecutar(sqlVenta);
    int idVentaNueva = (int)mysql_insert_id(cn.getConector());

    for (LineaFactura item : detalle) {
        string sqlDetalle = "INSERT INTO ventas_detalle(id_venta,id_producto,cantidad,precio_unitario) VALUES(" +
            cn.numeroSQL(idVentaNueva) + "," + cn.numeroSQL(item.id_producto) + "," +
            cn.numeroSQL(item.cantidad) + "," + cn.decimalSQL(item.precio_unitario) + ")";

        if (!cn.ejecutar(sqlDetalle)) ok = false;

        string sqlInv = "UPDATE productos SET existencia=existencia-" + cn.numeroSQL(item.cantidad) +
            " WHERE id_producto=" + cn.numeroSQL(item.id_producto) +
            " AND existencia>=" + cn.numeroSQL(item.cantidad);

        if (!cn.ejecutar(sqlInv)) ok = false;
        if (mysql_affected_rows(cn.getConector()) == 0) {
            ok = false;
            cout << "ALERTA: no se pudo descontar inventario del producto " << item.id_producto << ".\n";
        }
    }

    if (ok) {
        mysql_query(cn.getConector(), "COMMIT");
        cout << "Venta registrada correctamente.\n";

        string nombreCliente = "Cliente General";
        MYSQL_RES* resCliente;
        MYSQL_ROW rowCliente;
        string sqlCliente = "SELECT CONCAT(nombres, ' ', apellidos) FROM clientes WHERE id_cliente=" + cn.numeroSQL(id_cliente);

        if (mysql_query(cn.getConector(), sqlCliente.c_str()) == 0) {
            resCliente = mysql_store_result(cn.getConector());
            rowCliente = mysql_fetch_row(resCliente);
            if (rowCliente != nullptr && rowCliente[0] != nullptr) {
                nombreCliente = rowCliente[0];
            }
            mysql_free_result(resCliente);
        }

        Factura::imprimirVenta(no_factura, serie, fecha_factura, nit, nombreCliente, detalle, total);
        Factura::generarFacturaHTML(no_factura, serie, fecha_factura, nit, nombreCliente, detalle, total);
    }
    else {
        mysql_query(cn.getConector(), "ROLLBACK");
        cout << "ALERTA: venta revertida por error.\n";
    }

    cn.cerrarConexion();
}

void Venta::listarVentas() {
    ConexionBD cn;
    MYSQL_RES* res;
    MYSQL_ROW row;

    if (!cn.abrirConexion()) return;

    string sql = "SELECT v.id_venta,v.no_factura,v.serie,v.fecha_factura,c.nit,CONCAT(c.nombres,' ',c.apellidos),CONCAT(e.nombres,' ',e.apellidos) "
        "FROM ventas v "
        "INNER JOIN clientes c ON v.id_cliente=c.id_cliente "
        "INNER JOIN empleados e ON v.id_empleado=e.id_empleado "
        "ORDER BY v.id_venta DESC";

    if (mysql_query(cn.getConector(), sql.c_str()) == 0) {
        res = mysql_store_result(cn.getConector());

        cout << "\n===== VENTAS =====\n";

        while ((row = mysql_fetch_row(res))) {
            cout << row[0] << " | Factura " << row[2] << "-" << row[1]
                << " | Fecha: " << row[3]
                << " | NIT: " << row[4]
                << " | Cliente: " << row[5]
                << " | Empleado: " << row[6] << endl;
        }

        mysql_free_result(res);
    }
    else {
        cout << "ALERTA SQL: " << mysql_error(cn.getConector()) << endl;
    }

    cn.cerrarConexion();
}
