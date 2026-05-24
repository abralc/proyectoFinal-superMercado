#pragma once
#include "Persona.h"
#include <string>
using namespace std;

class Empleado : public Persona {
private:
    int id_empleado;
    string cui;
    string fecha_nacimiento;
    int id_puesto;
    string fecha_inicio_labores;
public:
    Empleado();
    Empleado(int id, string nom, string ape, string dir, string tel, string cuiEmp, int gen, string fechaNac, int puesto, string fechaInicio);
    void setIdEmpleado(int id); void setCui(string val); void setFechaNacimiento(string val); void setIdPuesto(int val); void setFechaInicioLabores(string val);
    int getIdEmpleado(); string getCui(); string getFechaNacimiento(); int getIdPuesto(); string getFechaInicioLabores();
    void crear(); void leer(); void actualizar(); void borrar(); bool existe(int id); bool existeCui(string cuiEmp, int id_excluir = 0);
};
