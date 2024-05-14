/*
 * Usuario.cpp
 *
 *  Created on: 11 may 2024
 *      Author: seven
 */

#include "Usuario.h"
#include "string.h"

Usuario::Usuario(char* dni, char* nombre, char* apellido, char* fechaNacimiento, char* telefono, char* direccion, char* contrasena, int idCiudad){
	strcpy(this->dni, dni);
	strcpy(this->nombre, nombre);
	strcpy(this->apellido, apellido);
	strcpy(this->fechaNacimiento, fechaNacimiento);
	strcpy(this->telefono, telefono);
	strcpy(this->direccion, direccion);
	strcpy(this->contrasena, contrasena);
	this->idCiudad = idCiudad;
}

Usuario::Usuario(){
	strcpy(this->dni, "");
	strcpy(this->nombre, "");
	strcpy(this->apellido, "");
	strcpy(this->fechaNacimiento, "");
	strcpy(this->telefono, "");
	strcpy(this->direccion, "");
	strcpy(this->contrasena, "");
	this->idCiudad = -1;
}

Usuario::~Usuario() {
}

Usuario::Usuario(const Usuario &other) {
	strcpy(this->dni, other.dni);
	strcpy(this->nombre, other.nombre);
	strcpy(this->apellido, other.apellido);
	strcpy(this->fechaNacimiento, other.fechaNacimiento);
	strcpy(this->telefono, other.telefono);
	strcpy(this->direccion, other.direccion);
	strcpy(this->contrasena, other.contrasena);
	this->idCiudad = idCiudad;
}

char* Usuario::getDni() const {
	return this->dni;
}

void Usuario::setDni(char *dni) {
	strcpy(this->dni, dni);
}

char* Usuario::getNombre() const {
	return this->nombre;
}

void Usuario::setNombre(char *nombre) {
	strcpy(this->nombre, nombre);
}

char* Usuario::getApellido() const {
	return this->apellido;
}

void Usuario::setApellido(char *apellido) {
	strcpy(this->apellido, apellido);
}

char* Usuario::getFechaNac() const {
	return this->fechaNacimiento;
}

void Usuario::setFechaNac(char *fecha) {
	strcpy(this->fechaNacimiento, fecha);
}

char* Usuario::getTelefono() const {
	return this->telefono;
}

void Usuario::setTelefono(char *telefono) {
	strcpy(this->telefono, telefono);
}

char* Usuario::getDireccion() const {
	return this->direccion;
}

void Usuario::setDireccion(char *direccion) {
	strcpy(this->direccion, direccion);
}

char* Usuario::getContrasena() const {
	return this->contrasena;
}

void Usuario::setContrasena(char *contrasena) {
	strcpy(this->contrasena, contrasena);
}

int Usuario::getIdCiudad() const {
	return this->idCiudad;
}

void Usuario::setIdCiudad(int id) {
	this->idCiudad = id;
}
