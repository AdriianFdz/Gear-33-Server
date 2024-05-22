/*
 * Fichero.cpp
 *
 *  Created on: 22 may 2024
 *      Author: oierc
 */

#include "Fichero.h"
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
using namespace std;


Fichero::Fichero() {
	strcpy(this->database, "");
	strcpy(this->log, "");

}

Fichero::~Fichero() {
}

void Fichero::leerConfig() {
	char linea[10000];
	char* clave;
	char* valor;

	FILE* f = fopen("../conf/propperties.cfg", "r");

	if (f == NULL) {
		cout << "ERROR AL ABRIR ARCHIVO CONFIGURACION\n" << endl;
		return;
	}

	while(fgets(linea, sizeof(linea), f)){
		// Eliminar saltos de linea
		linea[strcspn(linea, "\n")] = '\0';

		clave = strtok(linea, "=");
		valor = strtok(NULL, "=");

		if(strcmp(clave, "DATABASE") == 0) {
			strcpy(this->database, valor);
		}else if(strcmp(clave, "LOG") == 0) {
			strcpy(this->log, valor);
		}
	}
}

char* Fichero::getDatabase() {
	return this->database;
}

void Fichero::setDatabase(char *database) {
	strcpy(this->database, database);
}

char* Fichero::getLog() {
	return this->log;
}

void Fichero::setLog(char *log) {
	strcpy(this->log, log);
}
