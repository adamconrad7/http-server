#include "server.h"

int init_server(http_server_t* server, int port){
	server->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	server->address.sin_family = AF_INET;
	server->address.sin_port = htons(port);
	server->addrlen = sizeof(server -> address);
	return 0;
}

int start_server(http_server_t* server){
	return bind(server->socket_fd, (struct sockaddr*)&server->address, sizeof(server->address));
}

int stop_server(http_server_t* server){
	close(server->socket_fd);
	server->socket_fd = -1;
	return 0;
}

void run_server(http_server_t* server){
	listen(server -> socket_fd, 0);
	printf("Server listening on port %d\n", ntohs(server->address.sin_port));
	int cfd = accept(server -> socket_fd, (struct sockaddr*)&server -> address, &server -> addrlen);

	void* buf[1000];
	ssize_t bytes =  recv(cfd, buf, 1000, 0);
	printf("Server: recieved %zd bytes from client\n", bytes);
	char* ack = "HTTP/1.1 200 OK Content-Type: text/html";
	send(cfd, ack, strlen(ack), 0);
}



