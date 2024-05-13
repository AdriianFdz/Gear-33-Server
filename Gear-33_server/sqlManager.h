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

#endif /* SQLMANAGER_H_ */
