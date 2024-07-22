#include "unity.h"
#include "../src/server.h"
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>  

http_server_t server;

void setUp(void) {
    memset(&server, 0, sizeof(server));
}

void tearDown(void) {
    if (server.socket_fd != -1) {
        close(server.socket_fd);
    }
}

void test_http_server_init(void) {
    int result = http_server_init(&server, 8080);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(8080, server.port);
    TEST_ASSERT_EQUAL_INT(-1, server.socket_fd);
}

void test_http_server_configure_socket(void) {
    http_server_init(&server, 8080);
    int result = http_server_configure_socket(&server);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_NOT_EQUAL_INT(-1, server.socket_fd);
    TEST_ASSERT_EQUAL_INT(AF_INET, server.address.sin_family);
    TEST_ASSERT_EQUAL_INT(htons(8080), server.address.sin_port);
    TEST_ASSERT_EQUAL_INT(htonl(INADDR_ANY), server.address.sin_addr.s_addr);
}

void test_http_server_bind(void) {
    http_server_init(&server, 8080);
    http_server_configure_socket(&server);
    int result = http_server_bind(&server);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_http_server_listen(void) {
    http_server_init(&server, 8080);
    http_server_configure_socket(&server);
    http_server_bind(&server);
    int result = http_server_listen(&server, 10);
    TEST_ASSERT_EQUAL_INT(0, result);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_http_server_init);
    RUN_TEST(test_http_server_configure_socket);
    RUN_TEST(test_http_server_bind);
    RUN_TEST(test_http_server_listen);
    return UNITY_END();
}
