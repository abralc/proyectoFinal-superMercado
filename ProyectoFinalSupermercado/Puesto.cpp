#include "Puesto.h"
#include "ConexionBD.h"
#include <iostream>
#include <mysql.h>
using namespace std;

Puesto::Puesto() { id_puesto = 0; puesto = ""; }
Puesto::Puesto(int id, string nom) { id_puesto = id; puesto = nom; }
void Puesto::setIdPuesto(int id) { id_puesto = id; }
void Puesto::setPuesto(string nom) { puesto = nom; }
int Puesto::getIdPuesto() { return id_puesto; }
string Puesto::getPuesto() { return puesto; }

void Puesto::crear() { ConexionBD cn; if (!cn.abrirConexion()) return; if (existeNombre(puesto)) { cout << "ALERTA: el puesto ya existe.\n"; cn.cerrarConexion(); return; } if (cn.ejecutar("INSERT INTO puestos(puesto) VALUES(" + cn.textoSQL(puesto) + ")")) cout << "Puesto registrado correctamente.\n"; cn.cerrarConexion(); }
void Puesto::leer() { ConexionBD cn; MYSQL_RES* res; MYSQL_ROW row; if (!cn.abrirConexion()) return; string sql = "SELECT id_puesto, puesto FROM puestos ORDER BY id_puesto"; if (mysql_query(cn.getConector(), sql.c_str()) == 0) { res = mysql_store_result(cn.getConector()); cout << "\n===== PUESTOS =====\n"; while ((row = mysql_fetch_row(res))) cout << row[0] << " | " << row[1] << endl; mysql_free_result(res); } else cout << "ALERTA SQL: " << mysql_error(cn.getConector()) << endl; cn.cerrarConexion(); }
void Puesto::actualizar() { ConexionBD cn; if (!cn.abrirConexion()) return; if (!existe(id_puesto)) { cout << "ALERTA: el puesto no existe.\n"; cn.cerrarConexion(); return; } if (existeNombre(puesto, id_puesto)) { cout << "ALERTA: otro puesto ya tiene ese nombre.\n"; cn.cerrarConexion(); return; } string sql = "UPDATE puestos SET puesto=" + cn.textoSQL(puesto) + " WHERE id_puesto=" + cn.numeroSQL(id_puesto); if (cn.ejecutar(sql)) cout << "Puesto actualizado correctamente.\n"; cn.cerrarConexion(); }
void Puesto::borrar() { ConexionBD cn; if (!cn.abrirConexion()) return; if (cn.ejecutar("DELETE FROM puestos WHERE id_puesto=" + cn.numeroSQL(id_puesto))) cout << "Puesto eliminado correctamente.\n"; cn.cerrarConexion(); }
bool Puesto::existe(int id) { ConexionBD cn; if (!cn.abrirConexion()) return false; bool r = cn.existe("SELECT id_puesto FROM puestos WHERE id_puesto=" + cn.numeroSQL(id) + " LIMIT 1"); cn.cerrarConexion(); return r; }
bool Puesto::existeNombre(string nom, int id_excluir) { ConexionBD cn; if (!cn.abrirConexion()) return false; string sql = "SELECT id_puesto FROM puestos WHERE puesto=" + cn.textoSQL(nom); if (id_excluir > 0) sql += " AND id_puesto<>" + cn.numeroSQL(id_excluir); sql += " LIMIT 1"; bool r = cn.existe(sql); cn.cerrarConexion(); return r; }
