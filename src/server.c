#include "server.h"
#include "stdio.h"
#include <sys/socket.h> 
//Create a socket and bind it to an IP address and the chosen port.
//Put the socket in listening mode to accept incoming connections.

int http_server_init(http_server_t* server, int port){
	server->port = port;
	server->socket_fd=socket(AF_INET, SOCK_STREAM, 0);
	return 0;
}


int http_server_configure_socket(http_server_t*){
	return 0;
}

int http_server_bind(http_server_t*){
	return 0;
}

int http_server_listen(http_server_t*, int x){
	return 0;
}


