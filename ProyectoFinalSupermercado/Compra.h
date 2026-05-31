#pragma once
class Compra {
private:
    int id_compra;
    int no_orden_compra;
    int id_proveedor;
    int id_empleado;
public:
    Compra();
    void registrarCompra();
    void listarCompras();
    int siguienteOrden();
};
