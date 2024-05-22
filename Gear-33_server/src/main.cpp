/*
 * main.cpp
 *
 *  Created on: 7 may 2024
 *      Author: seven
 */

#include <winsock2.h>
#include <iostream>

extern "C" {
#include "../include/sqlite3.h"
}



#include "../include/sqlManager.h"
#include "../include/Usuario.h"
#include "../include/Coche.h"
#include "../include/Log.h"
#include "../include/Fichero.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

using namespace std;

int main(void) {
	/*
	 * CONFIGURAR LOGGER
	 */
	Fichero f;
	f.leerConfig();
	Log logger(f.getLog());
	WSADATA wsaData;
	SOCKET conn_socket;
	SOCKET comm_socket;
	struct sockaddr_in server;
	struct sockaddr_in client;
	char sendBuff[512], recvBuff[512];

	cout << "Iniciando libreria socket\n" << endl;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		char mensaje[256];
		snprintf(mensaje, sizeof(mensaje), "No se ha podido inicializar la libreria socket. Codigo de error: %d", WSAGetLastError());
		logger.anadirLog(mensaje);
		cout << "No se ha podido inicializar la libreria socket. Codigo de error: " << WSAGetLastError() << endl;
		return -1;
	}
	logger.anadirLog("Libreria socket inicializada");
	cout << "Libreria socket inicializada" << endl;

	//SOCKET creation
	if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		char mensaje[256];
		snprintf(mensaje, sizeof(mensaje), "No se ha podido crear el socket. Codigo de error: %d", WSAGetLastError());
		logger.anadirLog(mensaje);
		cout << "No se ha podido crear el socket. Codigo de error: " << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}
	logger.anadirLog("Socket creado correctamente");
	cout << "Socket creado correctamente" << endl;

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	if (bind(conn_socket, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
		char mensaje[256];
		snprintf(mensaje, sizeof(mensaje), "Error al vincular el socket. Codigo de error: %d", WSAGetLastError());
		logger.anadirLog(mensaje);
		cout << "Error al vincular el socket. Codigo de error: " << WSAGetLastError() <<endl;
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}
	logger.anadirLog("Bindeo realizado correctamente");
	cout << "Bindeo realizado correctamente" << endl;

	//LISTEN to incoming connections (socket server moves to listening mode)
	if (listen(conn_socket, 1) == SOCKET_ERROR) {
		char mensaje[256];
		snprintf(mensaje, sizeof(mensaje), "Error al activar modo escucha. Codigo de error: %d", WSAGetLastError());
		logger.anadirLog(mensaje);
		cout << "Error al activar modo escucha. Codigo de error: " << WSAGetLastError() <<endl;
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	//ACCEPT incoming connections (server keeps waiting for them)
	logger.anadirLog("Esperando a conexiones del cliente...");
	cout << "Esperando a conexiones del cliente..." << endl;
	int stsize = sizeof(struct sockaddr);
	comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);

	if (comm_socket == INVALID_SOCKET) {
		char mensaje[256];
		snprintf(mensaje, sizeof(mensaje), "Error al aceptar conexion del cliente. Codigo de error: %d", WSAGetLastError());
		logger.anadirLog(mensaje);
		cout << "Error al aceptar conexion del cliente. Codigo de error: " << WSAGetLastError() << endl;
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}
	char mensaje[256];
	snprintf(mensaje, sizeof(mensaje), "Conexion entrante por: %s (%i)", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
	logger.anadirLog(mensaje);
	cout << "Conexion entrante por: " << inet_ntoa(client.sin_addr) << " (" << ntohs(client.sin_port) << ")" << endl;

	// Closing the listening sockets (is not going to be used anymore)
	closesocket(conn_socket);

	logger.anadirLog("Esperando comandos del cliente...");
	cout << "Esperando comandos del cliente..." << endl;

	do {
		recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
		logger.anadirLog("Esperando comandos del cliente...");

		char mensaje[256];
		snprintf(mensaje, sizeof(mensaje), "Comando recibido: %s", recvBuff);
		logger.anadirLog(mensaje);
		cout << "Comando recibido: " << recvBuff << endl;

		if (strcmp(recvBuff, "COMP_INICIO_SESION") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char contrasena[strlen(recvBuff)] = "";
			strcpy(contrasena, recvBuff);

			Usuario u;
			int resultado = inicioSesion(dni, contrasena, u);

			sprintf(sendBuff, "%s", u.getDni());
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			sprintf(sendBuff, "%s", u.getNombre());
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			sprintf(sendBuff, "%s", u.getApellido());
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			sprintf(sendBuff, "%s", u.getFechaNac());
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			sprintf(sendBuff, "%s", u.getTelefono());
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			sprintf(sendBuff, "%s", u.getDireccion());
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			sprintf(sendBuff, "%s", u.getContrasena());
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			sprintf(sendBuff, "%d", u.getIdCiudad());
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

			logger.anadirLog("Datos de inicio de sesion recibidos");

			sprintf(sendBuff, "%d", resultado);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

		}

		if(strcmp(recvBuff, "COMP_REGISTRO") == 0) {
			Usuario u;
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			u.setDni(recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			u.setNombre(recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			u.setApellido(recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			u.setFechaNac(recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			u.setDireccion(recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			u.setTelefono(recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			u.setContrasena(recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			u.setIdCiudad(atoi(recvBuff));
			int idCiudad;
			idCiudad = atoi(recvBuff);

			logger.anadirLog("Datos de registro recibidos");

			anadirUsuario(u, idCiudad);

		}

		if(strcmp(recvBuff, "COMP_MODIF_NOMBRE") == 0){
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char nombreNuevo[strlen(recvBuff)] = "";
			strcpy(nombreNuevo, recvBuff);

			logger.anadirLog("Nombre recibido");
			modificarNombreUsuario(dni, nombreNuevo);

		}

		if(strcmp(recvBuff, "COMP_MODIF_DNI") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dniNuevo[strlen(recvBuff)] = "";
			strcpy(dniNuevo, recvBuff);

			logger.anadirLog("DNIs recibidos");

			modificarDniUsuario(dni, dniNuevo);
		}

		if(strcmp(recvBuff, "COMP_MODIF_APELLIDO") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char apellidoNuevo[strlen(recvBuff)] = "";
			strcpy(apellidoNuevo, recvBuff);

			logger.anadirLog("Apellido recibido");

			modificarApellidoUsuario(dni, apellidoNuevo);
		}

		if(strcmp(recvBuff, "COMP_MODIF_FECHA") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char fechaNueva[strlen(recvBuff)] = "";
			strcpy(fechaNueva, recvBuff);

			logger.anadirLog("Fecha de nacimiento recibido");
			modificarFechaUsuario(dni, fechaNueva);
		}

		if(strcmp(recvBuff, "COMP_MODIF_DIRECCION") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char direccionNueva[strlen(recvBuff)] = "";
			strcpy(direccionNueva, recvBuff);

			logger.anadirLog("Direccion recibida");
			modificarDireccionUsuario(dni, direccionNueva);
		}

		if(strcmp(recvBuff, "COMP_MODIF_TELEFONO") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char telefonoNuevo[strlen(recvBuff)] = "";
			strcpy(telefonoNuevo, recvBuff);

			logger.anadirLog("Telefono recibido");
			modificarTelefonoUsuario(dni, telefonoNuevo);
		}

		if(strcmp(recvBuff, "COMP_MODIF_CONTRASENA") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char contrasenaNueva[strlen(recvBuff)] = "";
			strcpy(contrasenaNueva, recvBuff);

			logger.anadirLog("Contrasena recibida");
			modificarContrasenaUsuario(dni, contrasenaNueva);
		}

		if(strcmp(recvBuff, "COMP_MODIF_CIUDAD") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			int idCiudad;
			idCiudad = atoi(recvBuff);

			logger.anadirLog("Ciudad recibida");
			modificarCiudadUsuario(dni, idCiudad);
		}

		if(strcmp(recvBuff, "OBTENER_NUMERO_COCHES_POR_PRECIO") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			int precioMin = atoi(recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			int precioMax = atoi(recvBuff);

			int numeroCoches = 0;
			obtenerNumeroCoches(precioMin, precioMax, numeroCoches);
			sprintf(sendBuff, "%i", numeroCoches);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			logger.anadirLog("Numero de coches por precios enviados");
		}

		if(strcmp(recvBuff, "OBTENER_COCHES_POR_PRECIO") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			int precioMin = atoi(recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			int precioMax = atoi(recvBuff);

			int numeroCoches = 0;
			obtenerNumeroCoches(precioMin, precioMax, numeroCoches);
			Coche listaCoches[numeroCoches];

			sprintf(sendBuff, "%i", numeroCoches);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

			obtenerCoches(precioMin, precioMax, listaCoches);

			for (int i = 0; i < numeroCoches; i++) {
				sprintf(sendBuff, "%s", listaCoches[i].getMatricula());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaCoches[i].getColor());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%i", listaCoches[i].getPotencia());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%f", listaCoches[i].getPrecio());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%i", listaCoches[i].getAnyo());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaCoches[i].getModelo());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaCoches[i].getCambio());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaCoches[i].getCombustible());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaCoches[i].getMarca());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			}
			logger.anadirLog("Coches por precio enviados");
		}

		if(strcmp(recvBuff, "OBTENER_NUMERO_COCHES_TOTAL") == 0) {
			int numeroCoches = 0;
			obtenerNumeroCochesTotal(numeroCoches);
			sprintf(sendBuff, "%i", numeroCoches);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			logger.anadirLog("Numero de coches enviados");
		}



		if(strcmp(recvBuff, "OBTENER_COCHES_TOTAL") == 0) {

			int numeroCoches = 0;
			obtenerNumeroCochesTotal(numeroCoches);
			Coche listaCoches[numeroCoches];

			sprintf(sendBuff, "%i", numeroCoches);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

			obtenerCochesTotal(listaCoches);

			for (int i = 0; i < numeroCoches; i++) {
				sprintf(sendBuff, "%s", listaCoches[i].getMatricula());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaCoches[i].getColor());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%i", listaCoches[i].getPotencia());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%f", listaCoches[i].getPrecio());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%i", listaCoches[i].getAnyo());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaCoches[i].getModelo());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaCoches[i].getCambio());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaCoches[i].getCombustible());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaCoches[i].getMarca());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			}
			logger.anadirLog("Numero de coches enviados");
		}

		if(strcmp(recvBuff, "ADQUIRIR_COCHE") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char fecha_ini[11];
			strcpy(fecha_ini, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char fecha_fin[11];
			strcpy(fecha_fin, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char tipo_adquisicion[9];
			strcpy(tipo_adquisicion, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			float precio_adquisicion = atof(recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[10];
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char matricula[8];
			strcpy(matricula, recvBuff);

			adquirirCoche(fecha_ini, fecha_fin, precio_adquisicion, dni, matricula, tipo_adquisicion);
			logger.anadirLog("Adquisicion coche realizada correctamente");
		}

		if(strcmp(recvBuff, "OBTENER_NUMERO_ADQUISICIONES_POR_DNI") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[11];
			strcpy(dni, recvBuff);
			int numeroAdquisiciones;
			obtenerNumeroAdquisiciones(numeroAdquisiciones, dni);
			sprintf(sendBuff, "%i", numeroAdquisiciones);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

		}

		if(strcmp(recvBuff, "OBTENER_ADQUISICIONES_POR_DNI") == 0) {

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[11];
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			int numeroAdquisiciones;
			numeroAdquisiciones = atoi(recvBuff);


			Adquisicion listaAdquisicion[numeroAdquisiciones];

			obtenerAdquisicionesPorDni(dni, listaAdquisicion);

			for (int i = 0; i < numeroAdquisiciones ; i++) {
				sprintf(sendBuff, "%s", listaAdquisicion[i].getTipoAdquisicion());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaAdquisicion[i].getFechaInicio());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaAdquisicion[i].getFechaFin());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%f", listaAdquisicion[i].getPrecioAdquisicion());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaAdquisicion[i].getCoche().getMatricula());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaAdquisicion[i].getCoche().getColor());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%i", listaAdquisicion[i].getCoche().getPotencia());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%f", listaAdquisicion[i].getCoche().getPrecio());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%i", listaAdquisicion[i].getCoche().getAnyo());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaAdquisicion[i].getCoche().getModelo());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaAdquisicion[i].getCoche().getCambio());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaAdquisicion[i].getCoche().getCombustible());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaAdquisicion[i].getCoche().getMarca());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

			}
			logger.anadirLog("Numero de adquisiciones por DNI realizadas correctamente");
		}

		if(strcmp(recvBuff, "OBTENER_NUMERO_COCHES_POR_PRECIO_ALQUILER") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			int precioMin = atoi(recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			int precioMax = atoi(recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char fechaInicio[11];
			strcpy(fechaInicio, recvBuff);


			int numeroCoches = 0;
			obtenerNumeroCochesAlquiler(precioMin, precioMax, numeroCoches, fechaInicio);
			sprintf(sendBuff, "%i", numeroCoches);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			logger.anadirLog("Numero de coches por precios enviados correctamente");
		}

		if(strcmp(recvBuff, "OBTENER_NUMERO_COCHES_TOTAL_ALQUILER") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char fechaInicio[11];
			strcpy(fechaInicio, recvBuff);
			int numeroCoches = 0;
			obtenerNumeroCochesTotalAlquiler(numeroCoches, fechaInicio);
			sprintf(sendBuff, "%i", numeroCoches);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			logger.anadirLog("Numero de coches enviados correctamente");
		}

		if(strcmp(recvBuff, "OBTENER_COCHES_POR_PRECIO_ALQUILER") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			int precioMin = atoi(recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			int precioMax = atoi(recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char fechaInicio[11];
			strcpy(fechaInicio, recvBuff);

			int numeroCoches = 0;
			obtenerNumeroCochesAlquiler(precioMin, precioMax, numeroCoches, fechaInicio);
			Coche listaCoches[numeroCoches];

			sprintf(sendBuff, "%i", numeroCoches);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

			obtenerCoches(precioMin, precioMax, listaCoches);

			for (int i = 0; i < numeroCoches; i++) {
				sprintf(sendBuff, "%s", listaCoches[i].getMatricula());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaCoches[i].getColor());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%i", listaCoches[i].getPotencia());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%f", listaCoches[i].getPrecio());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%i", listaCoches[i].getAnyo());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaCoches[i].getModelo());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaCoches[i].getCambio());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaCoches[i].getCombustible());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaCoches[i].getMarca());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			}
			logger.anadirLog("Coches por precio enviados correctamente");
		}

		if(strcmp(recvBuff, "OBTENER_COCHES_TOTAL_ALQUILER") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char fechaInicio[11];
			strcpy(fechaInicio, recvBuff);

			int numeroCoches = 0;
			obtenerNumeroCochesTotalAlquiler(numeroCoches, fechaInicio);
			Coche listaCoches[numeroCoches];
			sprintf(sendBuff, "%i", numeroCoches);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

			obtenerCochesTotalAlquiler(listaCoches, fechaInicio);

			for (int i = 0; i < numeroCoches; i++) {
				sprintf(sendBuff, "%s", listaCoches[i].getMatricula());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaCoches[i].getColor());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%i", listaCoches[i].getPotencia());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%f", listaCoches[i].getPrecio());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%i", listaCoches[i].getAnyo());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaCoches[i].getModelo());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaCoches[i].getCambio());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaCoches[i].getCombustible());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", listaCoches[i].getMarca());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			}
			logger.anadirLog("Coches enviados correctamente");
		}

		if(strcmp(recvBuff, "OBTENER_NUMERO_PROVINCIAS") == 0) {
			int numeroProvs;
			obtenerNumeroProvincias(&numeroProvs);

			sprintf(sendBuff, "%i", numeroProvs);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			logger.anadirLog("Numero de provincias obtenidas correctamente");

		}

		if(strcmp(recvBuff, "OBTENER_PROVINCIAS") == 0) {
			int numeroProvs;
			obtenerNumeroProvincias(&numeroProvs);
			Provincia p[numeroProvs];
			guardarProvincias(p);

			sprintf(sendBuff, "%i", numeroProvs);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

			for (int i = 0; i < numeroProvs ; i++) {
				sprintf(sendBuff, "%i", p[i].getId());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				sprintf(sendBuff, "%s", p[i].getNombre());
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			}
			logger.anadirLog("Provincias obtenidas correctamente");
		}

		if(strcmp(recvBuff, "ANADIR_CIUDAD") == 0) {
			int idCiudad;

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char nombreCiudad[51];
			strcpy(nombreCiudad, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			int idProvincia;
			idProvincia = atoi(recvBuff);


			anadirCiudad(nombreCiudad, idProvincia, &idCiudad);

			sprintf(sendBuff, "%i", idCiudad);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			logger.anadirLog("Ciudad anadida correctamente");
		}

		if(strcmp(recvBuff, "OBTENER_CIUDAD") == 0) {
			int idCiudad;
			char nombreCiudad[51];

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			idCiudad = atoi(recvBuff);

			//Funcion bd
			obtenerCiudad(idCiudad, nombreCiudad);

			sprintf(sendBuff, "%s", nombreCiudad);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			logger.anadirLog("Ciudad obtenida correctamente");

		}
		if(strcmp(recvBuff, "SALIR") == 0) {
			exit(0);
		}

	} while (1);
}

