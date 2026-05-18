#include "Marca.h"
#include "ConexionBD.h"
#include <iostream>
#include <mysql.h>
using namespace std;

Marca::Marca() { id_marca = 0; marca = ""; }
Marca::Marca(int id, string nom) { id_marca = id; marca = nom; }
void Marca::setIdMarca(int id) { id_marca = id; }
void Marca::setMarca(string nom) { marca = nom; }
int Marca::getIdMarca() { return id_marca; }
string Marca::getMarca() { return marca; }

void Marca::crear() {
    ConexionBD cn; if (!cn.abrirConexion()) return;
    if (existeNombre(marca)) { cout << "ALERTA: la marca ya existe.\n"; cn.cerrarConexion(); return; }
    string sql = "INSERT INTO marcas(marca) VALUES(" + cn.textoSQL(marca) + ")";
    if (cn.ejecutar(sql)) cout << "Marca registrada correctamente.\n";
    cn.cerrarConexion();
}


void Marca::leer() {
    ConexionBD cn; MYSQL_RES* res; MYSQL_ROW row;
    if (!cn.abrirConexion()) return;
    string sql = "SELECT id_marca, marca FROM marcas ORDER BY id_marca";
    if (mysql_query(cn.getConector(), sql.c_str()) == 0) {
        res = mysql_store_result(cn.getConector());
        cout << "\n===== MARCAS =====\n";
        while ((row = mysql_fetch_row(res))) cout << row[0] << " | " << row[1] << endl;
        mysql_free_result(res);
    } else cout << "ALERTA SQL: " << mysql_error(cn.getConector()) << endl;
    cn.cerrarConexion();
}

void Marca::actualizar() {
    ConexionBD cn; if (!cn.abrirConexion()) return;
    if (!existe(id_marca)) { cout << "ALERTA: la marca no existe.\n"; cn.cerrarConexion(); return; }
    if (existeNombre(marca, id_marca)) { cout << "ALERTA: otra marca ya tiene ese nombre.\n"; cn.cerrarConexion(); return; }
    string sql = "UPDATE marcas SET marca=" + cn.textoSQL(marca) + " WHERE id_marca=" + cn.numeroSQL(id_marca);
    if (cn.ejecutar(sql)) cout << "Marca actualizada correctamente.\n";
    cn.cerrarConexion();
}

void Marca::borrar() {
    ConexionBD cn; if (!cn.abrirConexion()) return;
    string sql = "DELETE FROM marcas WHERE id_marca=" + cn.numeroSQL(id_marca);
    if (cn.ejecutar(sql)) cout << "Marca eliminada correctamente.\n";
    cn.cerrarConexion();
}

bool Marca::existe(int id) {
    ConexionBD cn; if (!cn.abrirConexion()) return false;
    bool r = cn.existe("SELECT id_marca FROM marcas WHERE id_marca=" + cn.numeroSQL(id) + " LIMIT 1");
    cn.cerrarConexion(); return r;
}

bool Marca::existeNombre(string nom, int id_excluir) {
    ConexionBD cn; if (!cn.abrirConexion()) return false;
    string sql = "SELECT id_marca FROM marcas WHERE marca=" + cn.textoSQL(nom);
    if (id_excluir > 0) sql += " AND id_marca<>" + cn.numeroSQL(id_excluir);
    sql += " LIMIT 1";
    bool r = cn.existe(sql);
    cn.cerrarConexion(); return r;
}
