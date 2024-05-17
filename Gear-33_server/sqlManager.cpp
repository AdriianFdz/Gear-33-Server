/*
 * sqlManager.cpp
 *
 *  Created on: 10 may 2024
 *      Author: seven
 */

extern "C" {
#include "sqlite3.h"
}



#include "string.h"
#include "Usuario.h"
#include <iostream>

using namespace std;

sqlite3* abrirDB() {
	sqlite3 *db;

	int result = sqlite3_open("Gear-33.db", &db);
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

			cout<<"ENTRADO"<<endl;

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

int anadirUsuario(Usuario u) {
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
		result = sqlite3_bind_int(stmt, 8, 333); // ESTO ES ALGO AÑADIDO EN LA BD

		if (result != SQLITE_OK) {
			printf("Error binding parameters\n");
			printf("%s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return result;
		}

		result = sqlite3_step(stmt);
		if (result != SQLITE_DONE) {
			printf("Error inserting new data into Usuario\n");
			return result;
		}
		printf("Usuario anadido correctamente\n");
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
