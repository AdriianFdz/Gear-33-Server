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


sqlite3* abrirDB();
int inicioSesion(char dni[], char contrasena[]);

#endif /* SQLMANAGER_H_ */
