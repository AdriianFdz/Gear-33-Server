/*
 * sqlManager.h
 *
 *  Created on: 10 may 2024
 *      Author: seven
 */

#ifndef SQLMANAGER_H_
#define SQLMANAGER_H_

extern "C" {
#include "sqlite3.h"
}

#include "Usuario.h"

sqlite3* abrirDB();
int inicioSesion(char dni[], char contrasena[], Usuario& u);
int anadirUsuario(Usuario u);

int modificarDniUsuario(char dni[10], char dniNuevo[10]);
int modificarNombreUsuario(char dni[10], char nombreNuevo[51]);
int modificarApellidoUsuario(char dni[10], char apellidoNuevo[51]);
int modificarFechaUsuario(char dni[10], char fechaNueva[11]);
int modificarTelefonoUsuario(char dni[10], char telefonoNuevo[10]);
int modificarDireccionUsuario(char dni[10], char direccionNueva[51]);
int modificarContrasenaUsuario(char dni[10], char contrasenaNueva[51]);
int modificarCiudadUsuario(char dni[10], int id_ciudad);

#endif /* SQLMANAGER_H_ */
