/*
 * Usuario.h
 *
 *  Created on: 11 may 2024
 *      Author: seven
 */

#ifndef USUARIO_H_
#define USUARIO_H_

class Usuario {
private:
	char dni[10];
	char nombre[51];
	char apellido[51];
	char fechaNacimiento[11];
	char telefono[10];
	char direccion[51];
	char contrasena[51];
	int idCiudad;

public:
	Usuario(char* dni, char* nombre, char* apellido, char* fechaNacimiento, char* telefono, char* direccion, char* contrasena, int idCiudad);
	Usuario();
	virtual ~Usuario();
	Usuario(const Usuario &other);
	char* getDni() const;
	void setDni(char* dni);
	char* getNombre() const;
	void setNombre(char* nombre);
	char* getApellido() const;
	void setApellido(char* apellido);
	char* getFechaNac() const;
	void setFechaNac(char* fecha);
	char* getTelefono() const;
	void setTelefono(char* telefono);
	char* getDireccion() const;
	void setDireccion(char* direccion);
	char* getContrasena() const;
	void setContrasena(char * contrasena);
	int getIdCiudad() const;
	void setIdCiudad(int id);

};

#endif /* USUARIO_H_ */
