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

