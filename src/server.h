#include <sys/socket.h> 
#include <netinet/in.h> 


typedef struct {
	int port, socket_fd;
	struct sockaddr_in address;
} http_server_t;

int http_server_init(http_server_t* server, int port);

int http_server_configure_socket(http_server_t*);

int http_server_bind(http_server_t*);

int http_server_listen(http_server_t*, int);
