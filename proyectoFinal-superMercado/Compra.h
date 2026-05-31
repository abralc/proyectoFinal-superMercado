#pragma once
class Compra {
private:
    int id_compra;
    int no_orden_compra;
    int id_proveedor;
public:
    Compra();
    void registrarCompra();
    void listarCompras();
    int siguienteOrden();
};
