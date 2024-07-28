//#include "unity.h"
//#include "../src/server.h"
#include "unity.h"
#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define TEST_PORT 8080
#define LOCALHOST "127.0.0.1"

http_server_t server;

void setUp(void) {
    init_server(&server, TEST_PORT);
}

void tearDown(void) {
    // Ensure server is stopped and socket is closed
    stop_server(&server);
}

void test_server_initialization(void) {
    TEST_ASSERT_EQUAL(TEST_PORT, ntohs(server.address.sin_port));
    TEST_ASSERT_EQUAL(AF_INET, server.address.sin_family);
    TEST_ASSERT_EQUAL(INADDR_ANY, server.address.sin_addr.s_addr);
}

void test_server_start_and_stop(void) {
    TEST_ASSERT_EQUAL(0, start_server(&server));
    TEST_ASSERT_NOT_EQUAL(-1, server.socket_fd);
    
    // Verify server is listening on the correct port
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    TEST_ASSERT_EQUAL(0, getsockname(server.socket_fd, (struct sockaddr*)&addr, &addr_len));
    TEST_ASSERT_EQUAL(htons(TEST_PORT), addr.sin_port);
    
    TEST_ASSERT_EQUAL(0, stop_server(&server));
    
    // Verify socket is closed
    TEST_ASSERT_EQUAL(-1, server.socket_fd);
}

// Helper function to send HTTP request and receive response
char* send_http_request(const char* request) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    TEST_ASSERT_NOT_EQUAL(-1, sock);
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(TEST_PORT);
    inet_pton(AF_INET, LOCALHOST, &server_addr.sin_addr);
    
	printf("testing connection\n");
    TEST_ASSERT_EQUAL(0, connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)));
    

	printf("testing send\n");
    send(sock, request, strlen(request), 0);
    
    static char response[4096];
    memset(response, 0, sizeof(response));

//	printf("testing recv\n");
    recv(sock, response, sizeof(response) - 1, 0);
    
//	printf("testing close\n");
    close(sock);
    return response;
}

void test_http_get_request(void) {
    start_server(&server);
    
    // Run server in a separate thread
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, (void *(*)(void *))run_server, &server);
    
    // Allow some time for the server to start
    sleep(1);
    
    const char* request = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";

    printf("Client: sending resuest: %s\n", request);
    char* response = send_http_request(request);
    printf("Client: recieved response: %s\n", response);
    
    TEST_ASSERT_NOT_NULL(strstr(response, "HTTP/1.1 200 OK"));
    TEST_ASSERT_NOT_NULL(strstr(response, "Content-Type: text/html"));
    
    // Stop the server
    stop_server(&server);
    pthread_join(server_thread, NULL);
}

void test_http_404_not_found(void) {
    start_server(&server);
    
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, (void *(*)(void *))run_server, &server);
    
    sleep(1);
    
    const char* request = "GET /nonexistent HTTP/1.1\r\nHost: localhost\r\n\r\n";
    char* response = send_http_request(request);
    
    TEST_ASSERT_NOT_NULL(strstr(response, "HTTP/1.1 404 Not Found"));
    
    stop_server(&server);
    pthread_join(server_thread, NULL);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_server_initialization);
    RUN_TEST(test_server_start_and_stop);
    RUN_TEST(test_http_get_request);
    RUN_TEST(test_http_404_not_found);
    return UNITY_END();
}
