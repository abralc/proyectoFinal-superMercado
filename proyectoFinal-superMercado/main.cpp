#include <iostream>
#include <string>
#include "Validaciones.h"
#include "Marca.h"
#include "Puesto.h"
#include "Cliente.h"
#include "Proveedor.h"
#include "Empleado.h"
#include "Producto.h"
#include "Venta.h"
#include "Compra.h"

using namespace std;

void pausa() {
    cout << "\nPresione Enter para continuar...";
    cin.get();
}

void menuMarcas() {
    Marca obj;
    int op;
    do {
        cout << "\n===== CRUD MARCAS =====\n1. Crear\n2. Listar\n3. Actualizar\n4. Eliminar\n0. Regresar\nOpcion: ";
        op = Validaciones::pedirOpcion("");
        if (op == 1) { obj.setMarca(Validaciones::pedirTexto("Marca: ", 50)); obj.crear(); }
        else if (op == 2) obj.leer();
        else if (op == 3) { obj.setIdMarca(Validaciones::pedirEntero("ID marca: ")); obj.setMarca(Validaciones::pedirTexto("Nueva marca: ", 50)); obj.actualizar(); }
        else if (op == 4) { obj.setIdMarca(Validaciones::pedirEntero("ID marca: ")); obj.borrar(); }
        if (op != 0) pausa();
    } while (op != 0);
}

void menuPuestos() {
    Puesto obj;
    int op;
    do {
        cout << "\n===== CRUD PUESTOS =====\n1. Crear\n2. Listar\n3. Actualizar\n4. Eliminar\n0. Regresar\nOpcion: ";
        op = Validaciones::pedirOpcion("");
        if (op == 1) { obj.setPuesto(Validaciones::pedirTexto("Puesto: ", 50)); obj.crear(); }
        else if (op == 2) obj.leer();
        else if (op == 3) { obj.setIdPuesto(Validaciones::pedirEntero("ID puesto: ")); obj.setPuesto(Validaciones::pedirTexto("Nuevo puesto: ", 50)); obj.actualizar(); }
        else if (op == 4) { obj.setIdPuesto(Validaciones::pedirEntero("ID puesto: ")); obj.borrar(); }
        if (op != 0) pausa();
    } while (op != 0);
}

Cliente pedirCliente(int id = 0) {
    string nombres = Validaciones::pedirLetras("Nombres: ", 60);
    string apellidos = Validaciones::pedirLetras("Apellidos: ", 60);
    string nit = Validaciones::pedirNit("NIT: ");
    int genero = Validaciones::pedirGenero();
    string telefono = Validaciones::pedirTelefono("Telefono: ");
    string correo = Validaciones::pedirCorreoOpcional("Correo electronico opcional: ");
    return Cliente(id, nombres, apellidos, nit, genero, telefono, correo);
}

void menuClientes() {
    Cliente obj;
    int op;
    do {
        cout << "\n===== CRUD CLIENTES =====\n1. Crear\n2. Listar\n3. Actualizar\n4. Eliminar\n0. Regresar\nOpcion: ";
        op = Validaciones::pedirOpcion("");
        if (op == 1) { obj = pedirCliente(); obj.crear(); }
        else if (op == 2) obj.leer();
        else if (op == 3) { int id = Validaciones::pedirEntero("ID cliente: "); obj = pedirCliente(id); obj.actualizar(); }
        else if (op == 4) { obj.setIdCliente(Validaciones::pedirEntero("ID cliente: ")); obj.borrar(); }
        if (op != 0) pausa();
    } while (op != 0);
}

Proveedor pedirProveedor(int id = 0) {
    string proveedor = Validaciones::pedirTexto("Proveedor: ", 60);
    string nit = Validaciones::pedirNit("NIT: ");
    string direccion = Validaciones::pedirTexto("Direccion: ", 80);
    string telefono = Validaciones::pedirTelefono("Telefono: ");
    return Proveedor(id, proveedor, nit, direccion, telefono);
}

void menuProveedores() {
    Proveedor obj;
    int op;
    do {
        cout << "\n===== CRUD PROVEEDORES =====\n1. Crear\n2. Listar\n3. Actualizar\n4. Eliminar\n0. Regresar\nOpcion: ";
        op = Validaciones::pedirOpcion("");
        if (op == 1) { obj = pedirProveedor(); obj.crear(); }
        else if (op == 2) obj.leer();
        else if (op == 3) { int id = Validaciones::pedirEntero("ID proveedor: "); obj = pedirProveedor(id); obj.actualizar(); }
        else if (op == 4) { obj.setIdProveedor(Validaciones::pedirEntero("ID proveedor: ")); obj.borrar(); }
        if (op != 0) pausa();
    } while (op != 0);
}

Empleado pedirEmpleado(int id = 0) {
    string nombres = Validaciones::pedirLetras("Nombres: ", 60);
    string apellidos = Validaciones::pedirLetras("Apellidos: ", 60);
    string direccion = Validaciones::pedirTexto("Direccion: ", 80);
    string telefono = Validaciones::pedirTelefono("Telefono: ");
    string cui = Validaciones::pedirTexto("CUI: ", 15);
    int genero = Validaciones::pedirGenero();
    string fechaNac = Validaciones::pedirFecha("Fecha nacimiento YYYY-MM-DD: ");
    int idPuesto = Validaciones::pedirEntero("ID puesto: ");
    string fechaInicio = Validaciones::pedirFecha("Fecha inicio labores YYYY-MM-DD: ");
    return Empleado(id, nombres, apellidos, direccion, telefono, cui, genero, fechaNac, idPuesto, fechaInicio);
}

void menuEmpleados() {
    Empleado obj;
    int op;
    do {
        cout << "\n===== CRUD EMPLEADOS =====\n1. Crear\n2. Listar\n3. Actualizar\n4. Eliminar\n0. Regresar\nOpcion: ";
        op = Validaciones::pedirOpcion("");
        if (op == 1) { obj = pedirEmpleado(); obj.crear(); }
        else if (op == 2) obj.leer();
        else if (op == 3) { int id = Validaciones::pedirEntero("ID empleado: "); obj = pedirEmpleado(id); obj.actualizar(); }
        else if (op == 4) { obj.setIdEmpleado(Validaciones::pedirEntero("ID empleado: ")); obj.borrar(); }
        if (op != 0) pausa();
    } while (op != 0);
}

Producto pedirProducto(int id = 0) {
    string producto = Validaciones::pedirTexto("Producto: ", 50);
    int idMarca = Validaciones::pedirEntero("ID marca: ");
    string descripcion = Validaciones::pedirTexto("Descripcion: ", 100);
    string imagen = Validaciones::pedirTexto("Imagen: ", 30);
    double costo = Validaciones::pedirDecimal("Precio costo: ");
    double venta = Validaciones::pedirDecimal("Precio venta: ");
    int existencia = Validaciones::pedirEntero("Existencia: ");
    return Producto(id, producto, idMarca, descripcion, imagen, costo, venta, existencia);
}

void menuProductos() {
    Producto obj;
    int op;
    do {
        cout << "\n===== CRUD PRODUCTOS =====\n1. Crear\n2. Listar\n3. Actualizar\n4. Eliminar\n0. Regresar\nOpcion: ";
        op = Validaciones::pedirOpcion("");
        if (op == 1) { obj = pedirProducto(); obj.crear(); }
        else if (op == 2) obj.leer();
        else if (op == 3) { int id = Validaciones::pedirEntero("ID producto: "); obj = pedirProducto(id); obj.actualizar(); }
        else if (op == 4) { obj.setIdProducto(Validaciones::pedirEntero("ID producto: ")); obj.borrar(); }
        if (op != 0) pausa();
    } while (op != 0);
}

int main() {
    int opcion;
    do {
        cout << "\n========== SISTEMA SUPERMERCADO ==========\n";
        cout << "1. Marcas\n";
        cout << "2. Productos\n";
        cout << "3. Proveedores\n";
        cout << "4. Clientes\n";
        cout << "5. Puestos\n";
        cout << "6. Empleados\n";
        cout << "7. Registrar compra\n";
        cout << "8. Listar compras\n";
        cout << "9. Registrar venta\n";
        cout << "10. Listar ventas\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opcion: ";
        opcion = Validaciones::pedirOpcion("");

        switch (opcion) {
        case 1: menuMarcas(); break;
        case 2: menuProductos(); break;
        case 3: menuProveedores(); break;
        case 4: menuClientes(); break;
        case 5: menuPuestos(); break;
        case 6: menuEmpleados(); break;
        case 7: { Compra c; c.registrarCompra(); pausa(); break; }
        case 8: { Compra c; c.listarCompras(); pausa(); break; }
        case 9: { Venta v; v.registrarVenta(); pausa(); break; }
        case 10: { Venta v; v.listarVentas(); pausa(); break; }
        case 0: cout << "Programa finalizado.\n"; break;
        default: cout << "Opcion no valida.\n"; pausa(); break;
        }
    } while (opcion != 0);

    return 0;
}
