#include <sys/socket.h> 
#include <string.h>
#include <netinet/in.h> 
#include "stdio.h"
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <unistd.h>

typedef struct {
	int socket_fd;
	socklen_t addrlen; 
	struct sockaddr_in address;
} http_server_t;

int init_server(http_server_t* server, int port);
int start_server(http_server_t* server);
int stop_server(http_server_t* server);
void run_server(http_server_t* server);

//
//int http_server_configure_socket(http_server_t*);
//
//int http_server_bind(http_server_t*);
//
//int http_server_listen(http_server_t*, int);
