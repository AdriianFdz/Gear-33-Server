/*
 * sqlManager.cpp
 *
 *  Created on: 10 may 2024
 *      Author: seven
 */

extern "C" {
#include "sqlite3.h"
}

#include <windows.h>

#include "string.h"
#include "Usuario.h"
#include "Coche.h"
#include "Adquisicion.h"
#include <iostream>
#include "Provincia.h"
#include "Fichero.h"

using namespace std;

sqlite3* abrirDB() {
	sqlite3 *db;

	Fichero f;
	f.leerConfig();

	int result = sqlite3_open(f.getDatabase(), &db);
	if (result != SQLITE_OK) {
		cout << "Error opening database" << endl;
	}

	return db;
}

int inicioSesion(char dni[], char contrasena[], Usuario& u) {
	sqlite3 *db = abrirDB();
	int result = 0;

	sqlite3_stmt *stmt;

	char sql[] = "SELECT * FROM Usuario WHERE dni = ?";

	result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		return 0;
	}

	result = sqlite3_bind_text(stmt, 1, dni, strlen(dni), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		return result;
	}

	result = sqlite3_step(stmt);
	if (result == SQLITE_ROW) {
		if (strcmp(dni, (char*) sqlite3_column_text(stmt, 0)) == 0
				&& strcmp(contrasena, (char*) sqlite3_column_text(stmt, 6)) == 0) {


			u.setDni((char*) sqlite3_column_text(stmt, 0));
			u.setNombre((char*) sqlite3_column_text(stmt, 1));
			u.setApellido((char*) sqlite3_column_text(stmt, 2));
			u.setFechaNac((char*) sqlite3_column_text(stmt, 3));
			u.setTelefono((char*) sqlite3_column_text(stmt, 4));
			u.setDireccion((char*) sqlite3_column_text(stmt, 5));
			u.setContrasena((char*) sqlite3_column_text(stmt, 6));
			u.setIdCiudad(sqlite3_column_int(stmt, 7));


			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return 1;
		}

	} else if (result != SQLITE_DONE) {
		printf("Error fetching data\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return 0;
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;
}

int anadirUsuario(Usuario u, int idCiudad) {
	sqlite3 *db = abrirDB();
	sqlite3_stmt *stmt;


	char sql[] = "SELECT * FROM Usuario WHERE dni = ?";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 1, u.getDni(), strlen(u.getDni()), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_step(stmt);

	if (result == SQLITE_ROW) {
		printf("El usuario ya existe");
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return 0;
	} else {
		char sql2[] = "INSERT INTO Usuario VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
		int result = sqlite3_prepare_v2(db, sql2, strlen(sql2) + 1, &stmt,
				NULL);
		if (result != SQLITE_OK) {
			printf("Error preparing statement\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_close(db);
			return result;
		}

		result = sqlite3_bind_text(stmt, 1, u.getDni(), strlen(u.getDni()),
				SQLITE_STATIC);
		if (result != SQLITE_OK) {
			printf("Error binding parameters\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return result;
		}
		result = sqlite3_bind_text(stmt, 2, u.getNombre(), strlen(u.getNombre()),
				SQLITE_STATIC);
		if (result != SQLITE_OK) {
			printf("Error binding parameters\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return result;
		}
		result = sqlite3_bind_text(stmt, 3, u.getApellido(), strlen(u.getApellido()),
				SQLITE_STATIC);
		if (result != SQLITE_OK) {
			printf("Error binding parameters\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return result;
		}
		result = sqlite3_bind_text(stmt, 4, u.getFechaNac(), strlen(u.getFechaNac()

		),
				SQLITE_STATIC);
		if (result != SQLITE_OK) {
			printf("Error binding parameters\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return result;
		}
		result = sqlite3_bind_text(stmt, 5, u.getTelefono(), strlen(u.getTelefono()),
				SQLITE_STATIC);
		if (result != SQLITE_OK) {
			printf("Error binding parameters\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return result;
		}
		result = sqlite3_bind_text(stmt, 6, u.getDireccion(), strlen(u.getDireccion()),
				SQLITE_STATIC);
		if (result != SQLITE_OK) {
			printf("Error binding parameters\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return result;
		}
		result = sqlite3_bind_text(stmt, 7, u.getContrasena(), strlen(u.getContrasena()),
				SQLITE_STATIC);
		if (result != SQLITE_OK) {
			printf("Error binding parameters\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return result;
		}
		result = sqlite3_bind_int(stmt, 8, idCiudad); // ESTO ES ALGO AÑADIDO EN LA BD



		if (result != SQLITE_OK) {
			printf("Error binding parameters\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return result;
		}

		result = sqlite3_step(stmt);
		if (result != SQLITE_DONE) {
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			printf("Error inserting new data into Usuario\n");
			return result;
		}
		cout << "Usuario con DNI " << u.getDni() << " anadido correctamente" << endl;
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return 0;
	}
}

int modificarDniUsuario(char dni[10], char dniNuevo[10]) {
	sqlite3 *db = abrirDB();

	sqlite3_stmt *stmt;

	char sql[] = "UPDATE Usuario SET dni = ? WHERE dni = ?";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 1, dniNuevo, strlen(dniNuevo),
			SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 2, dni, strlen(dni), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_step(stmt);
	if (result != SQLITE_DONE) {
		printf("Error updating DNI usuario\n");
		return result;
	}
	result = sqlite3_finalize(stmt);
	if (result != SQLITE_OK) {
		printf("Error finalizing statement (UPDATE)\n");
		printf("%s\n", sqlite3_errmsg(db));
		return result;
	}
	sqlite3_stmt *stmt2;

    char sql2[] = "UPDATE Adquisicion SET dni_usuario = ? WHERE dni_usuario = ?";

    result = sqlite3_prepare_v2(db, sql2, strlen(sql2) + 1, &stmt2, NULL);
    	if (result != SQLITE_OK) {
    		printf("Error preparing statement\n");
    		printf("%s\n", sqlite3_errmsg(db));
    		sqlite3_close(db);
    		return result;
    	}

	result = sqlite3_bind_text(stmt2, 1, dniNuevo, strlen(dniNuevo), SQLITE_STATIC);
	result += sqlite3_bind_text(stmt2, 2, dni, strlen(dni), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt2);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_step(stmt2);
	if (result != SQLITE_DONE) {
		printf("Error updating DNI\n");
		return result;
	}

	result = sqlite3_finalize(stmt2);
	if (result != SQLITE_OK) {
		printf("Error finalizing statement (UPDATE)\n");
		printf("%s\n", sqlite3_errmsg(db));
		return result;
	}
	sqlite3_close(db);
	return 0;
}
int modificarNombreUsuario(char dni[10], char nombreNuevo[51]) {
	sqlite3 *db = abrirDB();

	sqlite3_stmt *stmt;

	char sql[] = "UPDATE Usuario SET nombre = ? WHERE dni = ?";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 1, nombreNuevo, strlen(nombreNuevo),
			SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 2, dni, strlen(dni), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_step(stmt);
	if (result != SQLITE_DONE) {
		printf("Error updating nombre usuario\n");
		return result;
	}
	result = sqlite3_finalize(stmt);
	if (result != SQLITE_OK) {
		printf("Error finalizing statement (UPDATE)\n");
		printf("%s\n", sqlite3_errmsg(db));
		return result;
	}
	sqlite3_close(db);
	return 0;
}
int modificarApellidoUsuario(char dni[10], char apellidoNuevo[51]) {
	sqlite3 *db = abrirDB();

	sqlite3_stmt *stmt;

	char sql[] = "UPDATE Usuario SET apellido = ? WHERE dni = ?";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 1, apellidoNuevo, strlen(apellidoNuevo),
			SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 2, dni, strlen(dni), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_step(stmt);
	if (result != SQLITE_DONE) {
		printf("Error updating apellido usuario\n");
		return result;
	}
	result = sqlite3_finalize(stmt);
	if (result != SQLITE_OK) {
		printf("Error finalizing statement (UPDATE)\n");
		printf("%s\n", sqlite3_errmsg(db));
		return result;
	}
	sqlite3_close(db);
	return 0;
}
int modificarFechaUsuario(char dni[10], char fechaNueva[11]) {
	sqlite3 *db = abrirDB();

	sqlite3_stmt *stmt;

	char sql[] = "UPDATE Usuario SET fecha_nacimiento = ? WHERE dni = ?";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 1, fechaNueva, strlen(fechaNueva),
			SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 2, dni, strlen(dni), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_step(stmt);
	if (result != SQLITE_DONE) {
		printf("Error updating fecha nacimiento usuario\n");
		return result;
	}
	result = sqlite3_finalize(stmt);
	if (result != SQLITE_OK) {
		printf("Error finalizing statement (UPDATE)\n");
		printf("%s\n", sqlite3_errmsg(db));
		return result;
	}
	sqlite3_close(db);
	return 0;
}
int modificarTelefonoUsuario(char dni[10], char telefonoNuevo[10]) {
	sqlite3 *db = abrirDB();

	sqlite3_stmt *stmt;

	char sql[] = "UPDATE Usuario SET telefono = ? WHERE dni = ?";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 1, telefonoNuevo, strlen(telefonoNuevo),
			SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 2, dni, strlen(dni), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_step(stmt);
	if (result != SQLITE_DONE) {
		printf("Error updating telefono usuario\n");
		return result;
	}
	result = sqlite3_finalize(stmt);
	if (result != SQLITE_OK) {
		printf("Error finalizing statement (UPDATE)\n");
		printf("%s\n", sqlite3_errmsg(db));
		return result;
	}
	sqlite3_close(db);
	return 0;
}
int modificarDireccionUsuario(char dni[10], char direccionNueva[51]) {
	sqlite3 *db = abrirDB();

	sqlite3_stmt *stmt;

	char sql[] = "UPDATE Usuario SET direccion = ? WHERE dni = ?";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 1, direccionNueva, strlen(direccionNueva),
			SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 2, dni, strlen(dni), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_step(stmt);
	if (result != SQLITE_DONE) {
		printf("Error updating direccion usuario\n");
		return result;
	}
	result = sqlite3_finalize(stmt);
	if (result != SQLITE_OK) {
		printf("Error finalizing statement (UPDATE)\n");
		printf("%s\n", sqlite3_errmsg(db));
		return result;
	}
	sqlite3_close(db);
	return 0;
}
int modificarContrasenaUsuario(char dni[10], char contrasenaNueva[51]) {
	sqlite3 *db = abrirDB();

	sqlite3_stmt *stmt;

	char sql[] = "UPDATE Usuario SET contrasena = ? WHERE dni = ?";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 1, contrasenaNueva,
			strlen(contrasenaNueva), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 2, dni, strlen(dni), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_step(stmt);
	if (result != SQLITE_DONE) {
		printf("Error updating contrasena usuario\n");
		return result;
	}
	result = sqlite3_finalize(stmt);
	if (result != SQLITE_OK) {
		printf("Error finalizing statement (UPDATE)\n");
		printf("%s\n", sqlite3_errmsg(db));
		return result;
	}
	sqlite3_close(db);
	return 0;
}
int modificarCiudadUsuario(char dni[10], int id_ciudad) {
	sqlite3 *db = abrirDB();

	sqlite3_stmt *stmt;

	char sql[] = "UPDATE Usuario SET id_ciudad = ? WHERE dni = ?";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_int(stmt, 1, id_ciudad);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 2, dni, strlen(dni), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_step(stmt);
	if (result != SQLITE_DONE) {
		printf("Error updating id_ciudad usuario\n");
		return result;
	}
	result = sqlite3_finalize(stmt);
	if (result != SQLITE_OK) {
		printf("Error finalizing statement (UPDATE)\n");
		printf("%s\n", sqlite3_errmsg(db));
		return result;
	}
	sqlite3_close(db);
	return 0;
}

int obtenerCoches(int precioMin, int precioMax, Coche* listaCoches) {
	sqlite3 *db = abrirDB();
	sqlite3_stmt *stmt;

	char sql[] = "SELECT c.matricula, c.color, c.potencia, c.precio_base, c.anyo, mo.nombre, mo.cambio, mo.combustible, ma.nombre FROM Coche c, Modelo mo, Marca ma WHERE c.id_modelo = mo.id AND mo.id_marca = ma.id AND c.estaComprado = 'false' AND c.precio_base BETWEEN ? AND ?";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		return 0;
	}

	result = sqlite3_bind_int(stmt, 1, precioMin);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_int(stmt, 2, precioMax);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	int contador = 0;
	do {
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW) {
			listaCoches[contador].setMatricula((char*) sqlite3_column_text(stmt, 0));
			listaCoches[contador].setColor((char*) sqlite3_column_text(stmt, 1));
			listaCoches[contador].setPotencia(sqlite3_column_int(stmt, 2));
			listaCoches[contador].setPrecio(sqlite3_column_double(stmt, 3));
			listaCoches[contador].setAnyo(sqlite3_column_int(stmt, 4));
			listaCoches[contador].setModelo((char*) sqlite3_column_text(stmt, 5));
			listaCoches[contador].setCambio((char*) sqlite3_column_text(stmt, 6));
			listaCoches[contador].setCombustible((char*) sqlite3_column_text(stmt, 7));
			listaCoches[contador].setMarca((char*) sqlite3_column_text(stmt, 8));
			contador++;
		}
	} while (result == SQLITE_ROW);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;
}
int obtenerNumeroCoches(int precioMin, int precioMax, int& numero) {
	sqlite3 *db = abrirDB();
	sqlite3_stmt *stmt;

	char sql[] = "SELECT count(*) FROM Coche c, Modelo mo, Marca ma WHERE c.id_modelo = mo.id AND mo.id_marca = ma.id AND c.estaComprado = 'false' AND c.precio_base BETWEEN ? AND ?";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		return 0;
	}

	result = sqlite3_bind_int(stmt, 1, precioMin);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_int(stmt, 2, precioMax);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_step(stmt);

	numero = sqlite3_column_int(stmt, 0);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;
}

int obtenerNumeroCochesTotal(int& numero){
	sqlite3 *db = abrirDB();
	sqlite3_stmt *stmt;

	char sql[] = "SELECT count(*) FROM Coche c, Modelo mo, Marca ma WHERE c.id_modelo = mo.id AND mo.id_marca = ma.id AND c.estaComprado = 'false'";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		return 0;
	}

	result = sqlite3_step(stmt);

	numero = sqlite3_column_int(stmt, 0);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;
}
int obtenerCochesTotal(Coche* listaCoches) {
	sqlite3 *db = abrirDB();
	sqlite3_stmt *stmt;

	char sql[] = "SELECT c.matricula, c.color, c.potencia, c.precio_base, c.anyo, mo.nombre, mo.cambio, mo.combustible, ma.nombre FROM Coche c, Modelo mo, Marca ma WHERE c.id_modelo = mo.id AND mo.id_marca = ma.id AND c.estaComprado = 'false'";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		return 0;
	}

	int contador = 0;
	do {
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW) {
			listaCoches[contador].setMatricula((char*) sqlite3_column_text(stmt, 0));
			listaCoches[contador].setColor((char*) sqlite3_column_text(stmt, 1));
			listaCoches[contador].setPotencia(sqlite3_column_int(stmt, 2));
			listaCoches[contador].setPrecio(sqlite3_column_double(stmt, 3));
			listaCoches[contador].setAnyo(sqlite3_column_int(stmt, 4));
			listaCoches[contador].setModelo((char*) sqlite3_column_text(stmt, 5));
			listaCoches[contador].setCambio((char*) sqlite3_column_text(stmt, 6));
			listaCoches[contador].setCombustible((char*) sqlite3_column_text(stmt, 7));
			listaCoches[contador].setMarca((char*) sqlite3_column_text(stmt, 8));
			contador++;
		}
	} while (result == SQLITE_ROW);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;
}
int adquirirCoche(char* fecha_ini, char* fecha_fin, float precio_adquisicion, char* dni_usuario, char* matricula, char* tipo_adquisicion) {
	sqlite3 *db = abrirDB();
	sqlite3_stmt *stmt;
	char sql[] = "INSERT INTO Adquisicion VALUES (?, ?, ?, ?, ?, ?)";
	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 1, fecha_ini, strlen(fecha_ini), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}
	result = sqlite3_bind_text(stmt, 2, fecha_fin, strlen(fecha_fin),
			SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}
	result = sqlite3_bind_double(stmt, 3, precio_adquisicion);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}
	result = sqlite3_bind_text(stmt, 4, dni_usuario, strlen(dni_usuario), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}
	result = sqlite3_bind_text(stmt, 5, matricula, strlen(matricula), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}
	result = sqlite3_bind_text(stmt, 6, tipo_adquisicion, strlen(tipo_adquisicion), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_step(stmt);
	if (result != SQLITE_DONE) {
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		printf("Error inserting new data into Adquisicion\n");
		return result;
	}

	if (strcmp(tipo_adquisicion, "compra") == 0) {
		sqlite3_stmt *stmt2;
		char sql[] = "UPDATE Coche SET estaComprado = 'true' WHERE matricula = ?";
		int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt2, NULL);
		if (result != SQLITE_OK) {
			printf("Error preparing statement\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_close(db);
			return result;
		}

		result = sqlite3_bind_text(stmt2, 1, matricula, strlen(matricula), SQLITE_STATIC);
		if (result != SQLITE_OK) {
			printf("Error binding parameters\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return result;
		}

		result = sqlite3_step(stmt2);
		if (result != SQLITE_DONE) {
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			printf("Error modifying estaComprado intro Coche\n");
			return result;
		}
	}

	cout << "Adquisicion con matricula " << matricula << " anadida correctamente" << endl;

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;
}

int obtenerNumeroAdquisiciones(int &numero, char *dni) {
	sqlite3 *db = abrirDB();
	sqlite3_stmt *stmt;

	char sql[] = "SELECT count(*) FROM Adquisicion WHERE dni_usuario = ?";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		return 0;
	}

	result = sqlite3_bind_text(stmt, 1, dni, strlen(dni),
			SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_step(stmt);

	numero = sqlite3_column_int(stmt, 0);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;

}
int obtenerAdquisicionesPorDni(char *dni, Adquisicion *listaAdquisicion) {
	sqlite3 *db = abrirDB();
	sqlite3_stmt *stmt;


	char sql[] = "SELECT a.tipo_adquisicion, a.fecha_inicio, a.fecha_fin, a.precio_adquisicion, c.matricula, c.color, c.potencia, c.precio_base, c.anyo, mo.nombre, mo.cambio, mo.combustible, ma.nombre FROM Adquisicion a, Coche c, Modelo mo, Marca ma WHERE a.matricula = c.matricula AND c.id_modelo = mo.id AND mo.id_marca = ma.id AND a.dni_usuario = ?";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement1\n");
		printf("%s\n", sqlite3_errmsg(db));
		return 0;
	}

	result = sqlite3_bind_text(stmt, 1, dni, strlen(dni), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	int contador = 0;
	do {
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW) {
			listaAdquisicion[contador].setTipoAdquisicion((char*) sqlite3_column_text(stmt, 0));
			listaAdquisicion[contador].setFechaInicio((char*) sqlite3_column_text(stmt, 1));
			listaAdquisicion[contador].setFechaFin((char*) sqlite3_column_text(stmt, 2));
			listaAdquisicion[contador].setPrecioAdquisicion(sqlite3_column_double(stmt, 3));
			listaAdquisicion[contador].getCoche().setMatricula((char*) sqlite3_column_text(stmt, 4));
			listaAdquisicion[contador].getCoche().setColor((char*) sqlite3_column_text(stmt, 5));
			listaAdquisicion[contador].getCoche().setPotencia(sqlite3_column_int(stmt, 6));
			listaAdquisicion[contador].getCoche().setPrecio(sqlite3_column_double(stmt, 7));
			listaAdquisicion[contador].getCoche().setAnyo(sqlite3_column_int(stmt, 8));
			listaAdquisicion[contador].getCoche().setModelo((char*) sqlite3_column_text(stmt, 9));
			listaAdquisicion[contador].getCoche().setCambio((char*) sqlite3_column_text(stmt, 10));
			listaAdquisicion[contador].getCoche().setCombustible((char*) sqlite3_column_text(stmt, 11));
			listaAdquisicion[contador].getCoche().setMarca((char*) sqlite3_column_text(stmt, 12));

			contador++;
		}
	} while (result == SQLITE_ROW);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;
}

int obtenerNumeroCochesAlquiler(int precioMin, int precioMax, int& numero, char* fechaInicio) {
	sqlite3 *db = abrirDB();
	sqlite3_stmt *stmt;
	char sql[] = "SELECT count(*) FROM Coche c, Modelo mo, Marca ma LEFT JOIN Adquisicion ad ON ad.matricula = c.matricula WHERE c.id_modelo = mo.id AND mo.id_marca = ma.id AND c.estaComprado = 'false' AND c.precio_base BETWEEN ? AND ? AND ((ad.fecha_fin < ? OR ad.fecha_inicio > ?) OR ad.matricula IS NULL)";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		return 0;
	}

	result = sqlite3_bind_int(stmt, 1, precioMin);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_int(stmt, 2, precioMax);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 3, fechaInicio, strlen(fechaInicio), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 4, fechaInicio, strlen(fechaInicio), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_step(stmt);

	numero = sqlite3_column_int(stmt, 0);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;
}

int obtenerNumeroCochesTotalAlquiler(int& numero, char* fechaInicio){
	sqlite3 *db = abrirDB();
	sqlite3_stmt *stmt;

	char sql[] = "SELECT count(*) FROM Coche c, Modelo mo, Marca ma LEFT JOIN Adquisicion ad ON ad.matricula = c.matricula WHERE c.id_modelo = mo.id AND mo.id_marca = ma.id AND c.estaComprado = 'false' AND ((ad.fecha_fin < ? OR ad.fecha_inicio > ?) OR ad.matricula IS NULL)";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		return 0;
	}

	result = sqlite3_bind_text(stmt, 1, fechaInicio, strlen(fechaInicio), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}
	result = sqlite3_bind_text(stmt, 2, fechaInicio, strlen(fechaInicio), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_step(stmt);

	numero = sqlite3_column_int(stmt, 0);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;
}

int obtenerCochesAlquiler(int precioMin, int precioMax, Coche* listaCoches, char* fechaInicio) {
	sqlite3 *db = abrirDB();
	sqlite3_stmt *stmt;

	char sql[] = "SELECT c.matricula, c.color, c.potencia, c.precio_base, c.anyo, mo.nombre, mo.cambio, mo.combustible, ma.nombre FROM Coche c, Modelo mo, Marca ma LEFT JOIN Adquisicion ad ON ad.matricula = c.matricula WHERE c.id_modelo = mo.id AND mo.id_marca = ma.id AND c.estaComprado = 'false' AND ((ad.fecha_fin < ? OR ad.fecha_inicio > ?) OR ad.matricula IS NULL)";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		return 0;
	}

	result = sqlite3_bind_int(stmt, 1, precioMin);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_int(stmt, 2, precioMax);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	result = sqlite3_bind_text(stmt, 3, fechaInicio, strlen(fechaInicio), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}
	result = sqlite3_bind_text(stmt, 4, fechaInicio, strlen(fechaInicio), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	int contador = 0;
	do {
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW) {
			listaCoches[contador].setMatricula((char*) sqlite3_column_text(stmt, 0));
			listaCoches[contador].setColor((char*) sqlite3_column_text(stmt, 1));
			listaCoches[contador].setPotencia(sqlite3_column_int(stmt, 2));
			listaCoches[contador].setPrecio(sqlite3_column_double(stmt, 3));
			listaCoches[contador].setAnyo(sqlite3_column_int(stmt, 4));
			listaCoches[contador].setModelo((char*) sqlite3_column_text(stmt, 5));
			listaCoches[contador].setCambio((char*) sqlite3_column_text(stmt, 6));
			listaCoches[contador].setCombustible((char*) sqlite3_column_text(stmt, 7));
			listaCoches[contador].setMarca((char*) sqlite3_column_text(stmt, 8));
			contador++;
		}
	} while (result == SQLITE_ROW);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;
}

int obtenerCochesTotalAlquiler(Coche* listaCoches, char* fechaInicio) {
	sqlite3 *db = abrirDB();
	sqlite3_stmt *stmt;

	char sql[] = "SELECT c.matricula, c.color, c.potencia, c.precio_base, c.anyo, mo.nombre, mo.cambio, mo.combustible, ma.nombre FROM Coche c, Modelo mo, Marca ma LEFT JOIN Adquisicion ad ON ad.matricula = c.matricula WHERE c.id_modelo = mo.id AND mo.id_marca = ma.id AND c.estaComprado = 'false' AND ((ad.fecha_fin < ? OR ad.fecha_inicio > ?) OR ad.matricula IS NULL)";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		return 0;
	}

	result = sqlite3_bind_text(stmt, 1, fechaInicio, strlen(fechaInicio), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}
	result = sqlite3_bind_text(stmt, 2, fechaInicio, strlen(fechaInicio), SQLITE_STATIC);
	if (result != SQLITE_OK) {
		printf("Error binding parameters\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return result;
	}

	int contador = 0;
	do {
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW) {
			listaCoches[contador].setMatricula((char*) sqlite3_column_text(stmt, 0));
			listaCoches[contador].setColor((char*) sqlite3_column_text(stmt, 1));
			listaCoches[contador].setPotencia(sqlite3_column_int(stmt, 2));
			listaCoches[contador].setPrecio(sqlite3_column_double(stmt, 3));
			listaCoches[contador].setAnyo(sqlite3_column_int(stmt, 4));
			listaCoches[contador].setModelo((char*) sqlite3_column_text(stmt, 5));
			listaCoches[contador].setCambio((char*) sqlite3_column_text(stmt, 6));
			listaCoches[contador].setCombustible((char*) sqlite3_column_text(stmt, 7));
			listaCoches[contador].setMarca((char*) sqlite3_column_text(stmt, 8));
			contador++;
		}
	} while (result == SQLITE_ROW);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;
}

int obtenerNumeroProvincias(int *numero) {
	sqlite3 *db = abrirDB();
	sqlite3_stmt *stmt;

	char sql[] = "SELECT count(*) FROM Provincia";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		return 0;
	}

	result = sqlite3_step(stmt);
	if (result == SQLITE_ROW) {
		*numero = sqlite3_column_int(stmt, 0);
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;
}
int guardarProvincias(Provincia provincias[]) {
	sqlite3 *db = abrirDB();
	sqlite3_stmt *stmt;

	char sql[] = "SELECT * FROM Provincia";

	int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
	if (result != SQLITE_OK) {
		printf("Error preparing statement\n");
		printf("%s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return result;
	}



	int contador = 0;
	do {
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW) {
			provincias[contador].setId(sqlite3_column_int(stmt, 0));
			provincias[contador].setNombre((char*) sqlite3_column_text(stmt, 1));
					;
			contador++;
		}
	} while (result == SQLITE_ROW);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;

}

int existeCiudad(char ciudad[51], int id_prov, int* id_ciudad){
    sqlite3 *db = abrirDB();

    sqlite3_stmt *stmt;

    char sql[] = "SELECT * FROM Ciudad WHERE nombre = ? AND id_provincia = ?";

    int result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error preparing statement\n");
        printf("%s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return result;
    }

    result = sqlite3_bind_text(stmt, 1, ciudad, strlen(ciudad), SQLITE_STATIC);
    result = sqlite3_bind_int(stmt, 2, id_prov);
    if (result != SQLITE_OK) {
    	printf("Error binding parameters\n");
    	printf("%s\n", sqlite3_errmsg(db));
    	sqlite3_finalize(stmt);
    	sqlite3_close(db);
    	return result;
    }


    result = sqlite3_step(stmt);

    if (result == SQLITE_ROW) {
			*id_ciudad = sqlite3_column_int(stmt, 0);

		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return 1;
    }

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;
}


int anadirCiudad(char* nombreCiudad, int id_provincia, int* id_ciudad) {
	sqlite3 *db = abrirDB();
	sqlite3_stmt *stmt;

	int resultado = existeCiudad(nombreCiudad, id_provincia, id_ciudad);

	if (resultado != 0) {
		printf("La ciudad ya existe\n");
		return 0;
	} else {
		char sql2[] = "INSERT INTO Ciudad VALUES (NULL, ?, ?)";
		int result = sqlite3_prepare_v2(db, sql2, strlen(sql2) + 1, &stmt,
				NULL);
		if (result != SQLITE_OK) {
			printf("Error preparing statement\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_close(db);
			return result;
		}

		result = sqlite3_bind_text(stmt, 1, nombreCiudad, strlen(nombreCiudad),
				SQLITE_STATIC);

		result += sqlite3_bind_int(stmt, 2, id_provincia);

		if (result != SQLITE_OK) {
			printf("Error binding parameters\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return result;
		}

		result = sqlite3_step(stmt);
		if (result != SQLITE_DONE) {
			printf("Error inserting new data into Ciudad\n");
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return result;
		}

		*id_ciudad = sqlite3_last_insert_rowid(db);

		printf("Ciudad anadida correctamente\n");
		return 0;
	}
}
