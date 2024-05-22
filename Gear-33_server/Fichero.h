/*
 * Fichero.h
 *
 *  Created on: 22 may 2024
 *      Author: oierc
 */

#ifndef FICHERO_H_
#define FICHERO_H_

class Fichero {
private:
	char database[100];
public:
	Fichero();
	Fichero(char* database);
	virtual ~Fichero();
	void leerConfig();

	char* getDatabase();
	void setDatabase(char* database);

};

#endif /* FICHERO_H_ */
