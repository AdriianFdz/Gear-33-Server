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

}

Fichero::Fichero(char *database) {
	strcpy(this->database, database);
}

Fichero::~Fichero() {
	// TODO Auto-generated destructor stub
}

void Fichero::leerConfig() {
	char linea[10000];
	char* clave;
	char* valor;

	FILE* f = fopen("../conf/propperties.cfg", "r");

	if (f == NULL) {
		printf("ERROR AL ABRIR ARCHIVO CONFIGURACION\n");
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
	return this->getLog();
}

void Fichero::setLog(char *log) {
	strcpy(this->log, log);
}
