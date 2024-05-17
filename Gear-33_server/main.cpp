/*
 * main.cpp
 *
 *  Created on: 7 may 2024
 *      Author: seven
 */

#include <winsock2.h>
#include <iostream>

extern "C" {
#include "sqlite3.h"
}



#include "sqlManager.h"
#include "Usuario.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

using namespace std;

int main(void) {
	WSADATA wsaData;
	SOCKET conn_socket;
	SOCKET comm_socket;
	struct sockaddr_in server;
	struct sockaddr_in client;
	char sendBuff[512], recvBuff[512];

	cout << "Iniciando libreria socket\n" << endl;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << " No se ha podido inicializar la libreria socket. Codigo de error: " << WSAGetLastError() << endl;
		return -1;
	}
	cout << "Libreria socket inicializada" << endl;

	//SOCKET creation
	if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		cout << "No se ha podido crear el socket. Codigo de error: " << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}
	cout << "Socket creado correctamente" << endl;

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	if (bind(conn_socket, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
		cout << "Error al vincular el socket. Codigo de error: " << WSAGetLastError() <<endl;
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}
	cout << "Bindeo realizado correctamente" << endl;

	//LISTEN to incoming connections (socket server moves to listening mode)
	if (listen(conn_socket, 1) == SOCKET_ERROR) {
		cout << "Error al activar modo escucha. Codigo de error: " << WSAGetLastError() <<endl;
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	//ACCEPT incoming connections (server keeps waiting for them)
	cout << "Esperando a conexiones del cliente..." << endl;
	int stsize = sizeof(struct sockaddr);
	comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);

	if (comm_socket == INVALID_SOCKET) {
		cout << "Error al aceptar conexion del cliente. Codigo de error: " << WSAGetLastError() << endl;
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	cout << "Conexion entrante por: " << inet_ntoa(client.sin_addr) << " (" << ntohs(client.sin_port) << ")" << endl;

	// Closing the listening sockets (is not going to be used anymore)
	closesocket(conn_socket);

	cout << "Esperando comandos del cliente..." << endl;

	do {
		recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
		cout << "Comando recibido: " << recvBuff << endl;

		if (strcmp(recvBuff, "COMP_INICIO_SESION") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char contrasena[strlen(recvBuff)] = "";
			strcpy(contrasena, recvBuff);

			cout<<dni<<endl;
			cout<<contrasena<<endl;
			Usuario u;
			int resultado = inicioSesion(dni, contrasena, u);

			/*
			 * Aqui tendrias que hacer varios
			 * sprintf de cada propiedad del usuario u
			 * enviandola al cliente una a una
			 */

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


			sprintf(sendBuff, "%d", resultado);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);




			printf("Respuesta enviada: %s \n", sendBuff);

		}

		if(strcmp(recvBuff, "COMP_REGISTRO") == 0) {
			Usuario u;
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);
			u.setDni(dni);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char nombre[strlen(recvBuff)] = "";
			strcpy(nombre, recvBuff);
			u.setNombre(nombre);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char apellido[strlen(recvBuff)] = "";
			strcpy(apellido, recvBuff);
			u.setApellido(apellido);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char fecha[strlen(recvBuff)] = "";
			strcpy(fecha, recvBuff);
			u.setFechaNac(fecha);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char direccion[strlen(recvBuff)] = "";
			strcpy(direccion, recvBuff);
			u.setDireccion(direccion);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char telefono[strlen(recvBuff)] = "";
			strcpy(telefono, recvBuff);
			u.setTelefono(telefono);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char contrasena[strlen(recvBuff)] = "";
			strcpy(contrasena, recvBuff);
			u.setContrasena(contrasena);

			printf("Datos de registro recibidos\n");

			anadirUsuario(u);


		}

		if(strcmp(recvBuff, "COMP_MODIF_NOMBRE") == 0){
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char nombreNuevo[strlen(recvBuff)] = "";
			strcpy(nombreNuevo, recvBuff);

			modificarNombreUsuario(dni, nombreNuevo);

		}

		if(strcmp(recvBuff, "COMP_MODIF_DNI") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dniNuevo[strlen(recvBuff)] = "";
			strcpy(dniNuevo, recvBuff);

			modificarDniUsuario(dni, dniNuevo);
		}

		if(strcmp(recvBuff, "COMP_MODIF_APELLIDO") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char apellidoNuevo[strlen(recvBuff)] = "";
			strcpy(apellidoNuevo, recvBuff);

			modificarApellidoUsuario(dni, apellidoNuevo);
		}

		if(strcmp(recvBuff, "COMP_MODIF_FECHA") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char fechaNueva[strlen(recvBuff)] = "";
			strcpy(fechaNueva, recvBuff);

			modificarFechaUsuario(dni, fechaNueva);
		}

		if(strcmp(recvBuff, "COMP_MODIF_DIRECCION") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char direccionNueva[strlen(recvBuff)] = "";
			strcpy(direccionNueva, recvBuff);

			modificarDireccionUsuario(dni, direccionNueva);
		}

		if(strcmp(recvBuff, "COMP_MODIF_TELEFONO") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char telefonoNuevo[strlen(recvBuff)] = "";
			strcpy(telefonoNuevo, recvBuff);

			modificarTelefonoUsuario(dni, telefonoNuevo);
		}

		if(strcmp(recvBuff, "COMP_MODIF_CONTRASENA") == 0) {
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char dni[strlen(recvBuff)] = "";
			strcpy(dni, recvBuff);

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			char contrasenaNueva[strlen(recvBuff)] = "";
			strcpy(contrasenaNueva, recvBuff);

			modificarContrasenaUsuario(dni, contrasenaNueva);
		}

	} while (1);

}

