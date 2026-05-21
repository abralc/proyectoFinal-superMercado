#include "Cliente.h"
#include "ConexionBD.h"
#include <iostream>
#include <mysql.h>
using namespace std;

Cliente::Cliente() : Persona() { id_cliente = 0; nit = ""; correo_electronico = ""; }
Cliente::Cliente(int id, string nom, string ape, string nitCliente, int gen, string tel, string correo) : Persona(nom, ape, "", tel, gen) { id_cliente = id; nit = nitCliente; correo_electronico = correo; }
void Cliente::setIdCliente(int id) { id_cliente = id; }
void Cliente::setNit(string valor) { nit = valor; }
void Cliente::setCorreoElectronico(string valor) { correo_electronico = valor; }
int Cliente::getIdCliente() { return id_cliente; }
string Cliente::getNit() { return nit; }
string Cliente::getCorreoElectronico() { return correo_electronico; }

void Cliente::crear() { ConexionBD cn; if (!cn.abrirConexion()) return; if (existeNit(nit)) { cout << "ALERTA: el NIT ya existe.\n"; cn.cerrarConexion(); return; } string correoSQL = correo_electronico.empty() ? "NULL" : cn.textoSQL(correo_electronico); string sql = "INSERT INTO clientes(nombres,apellidos,nit,genero,telefono,correo_electronico,fecha_ingreso) VALUES(" + cn.textoSQL(nombres) + "," + cn.textoSQL(apellidos) + "," + cn.textoSQL(nit) + ",b'" + to_string(genero) + "'," + cn.textoSQL(telefono) + "," + correoSQL + ",NOW())"; if (cn.ejecutar(sql)) cout << "Cliente registrado correctamente.\n"; cn.cerrarConexion(); }
void Cliente::leer() { ConexionBD cn; MYSQL_RES* res; MYSQL_ROW row; if (!cn.abrirConexion()) return; string sql = "SELECT id_cliente,nombres,apellidos,nit,BIN(genero),telefono,IFNULL(correo_electronico,'') FROM clientes ORDER BY id_cliente"; if (mysql_query(cn.getConector(), sql.c_str()) == 0) { res = mysql_store_result(cn.getConector()); cout << "\n===== CLIENTES =====\n"; while ((row = mysql_fetch_row(res))) cout << row[0] << " | " << row[1] << " " << row[2] << " | NIT: " << row[3] << " | Genero: " << row[4] << " | Tel: " << row[5] << " | Correo: " << row[6] << endl; mysql_free_result(res); } else cout << "ALERTA SQL: " << mysql_error(cn.getConector()) << endl; cn.cerrarConexion(); }
void Cliente::actualizar() { ConexionBD cn; if (!cn.abrirConexion()) return; if (!existe(id_cliente)) { cout << "ALERTA: el cliente no existe.\n"; cn.cerrarConexion(); return; } if (existeNit(nit, id_cliente)) { cout << "ALERTA: otro cliente ya tiene ese NIT.\n"; cn.cerrarConexion(); return; } string correoSQL = correo_electronico.empty() ? "NULL" : cn.textoSQL(correo_electronico); string sql = "UPDATE clientes SET nombres=" + cn.textoSQL(nombres) + ", apellidos=" + cn.textoSQL(apellidos) + ", nit=" + cn.textoSQL(nit) + ", genero=b'" + to_string(genero) + "', telefono=" + cn.textoSQL(telefono) + ", correo_electronico=" + correoSQL + " WHERE id_cliente=" + cn.numeroSQL(id_cliente); if (cn.ejecutar(sql)) cout << "Cliente actualizado correctamente.\n"; cn.cerrarConexion(); }
void Cliente::borrar() { ConexionBD cn; if (!cn.abrirConexion()) return; if (cn.ejecutar("DELETE FROM clientes WHERE id_cliente=" + cn.numeroSQL(id_cliente))) cout << "Cliente eliminado correctamente.\n"; cn.cerrarConexion(); }
bool Cliente::existe(int id) { ConexionBD cn; if (!cn.abrirConexion()) return false; bool r = cn.existe("SELECT id_cliente FROM clientes WHERE id_cliente=" + cn.numeroSQL(id) + " LIMIT 1"); cn.cerrarConexion(); return r; }
bool Cliente::existeNit(string nitCliente, int id_excluir) { ConexionBD cn; if (!cn.abrirConexion()) return false; string sql = "SELECT id_cliente FROM clientes WHERE nit=" + cn.textoSQL(nitCliente); if (id_excluir > 0) sql += " AND id_cliente<>" + cn.numeroSQL(id_excluir); sql += " LIMIT 1"; bool r = cn.existe(sql); cn.cerrarConexion(); return r; }
int Cliente::obtenerIdPorNit(string nitCliente) { ConexionBD cn; if (!cn.abrirConexion()) return 0; int id = cn.obtenerEntero("SELECT id_cliente FROM clientes WHERE nit=" + cn.textoSQL(nitCliente) + " LIMIT 1"); cn.cerrarConexion(); return id; }
int Cliente::crearAutomaticoPorNit(string nitCliente) { if (existeNit(nitCliente)) return obtenerIdPorNit(nitCliente); nombres = "Cliente"; apellidos = "Nuevo"; nit = nitCliente; genero = 1; telefono = "00000000"; correo_electronico = ""; crear(); return obtenerIdPorNit(nitCliente); }
