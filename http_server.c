#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "http_server.h"
#include "static_handler.h"
#include "calc_handler.h"

#define BUFFER_SIZE 4096
#define DEFAULT_PORT 80

void *handle_client_request(void *client_socket_ptr);

void start_http_server(int argc, char *argv[]) {
    int server_socket, client_socket, port_number = DEFAULT_PORT;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length = sizeof(client_address);

    if (argc == 3 && strcmp(argv[1], "-p") == 0) {
        port_number = atoi(argv[2]);
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port_number);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 10) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", port_number);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_length);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        pthread_t client_thread;
        int *new_client_socket = malloc(sizeof(int));
        *new_client_socket = client_socket;
        pthread_create(&client_thread, NULL, handle_client_request, new_client_socket);
        pthread_detach(client_thread);
    }

    close(server_socket);
}

void *handle_client_request(void *client_socket_ptr) {
    int client_socket = *((int*)client_socket_ptr);
    free(client_socket_ptr);

    char buffer[BUFFER_SIZE];
    int received_bytes = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    if (received_bytes <= 0) {
        close(client_socket);
        pthread_exit(NULL);
    }
    buffer[received_bytes] = '\0';

    char method[8], path[1024];
    sscanf(buffer, "%s %s", method, path);

    if (strcmp(method, "GET") != 0) {
        send_http_response(client_socket, "405 Method Not Allowed", "text/plain", "Only GET allowed.", 17);
        close(client_socket);
        pthread_exit(NULL);
    }

    if (strncmp(path, "/static/", 8) == 0) {
        serve_static_file(client_socket, path);
    } else if (strncmp(path, "/calc/", 6) == 0) {
        handle_calculation_request(client_socket, path);
    } else {
        send_http_response(client_socket, "404 Not Found", "text/plain", "Path not found.", 15);
    }

    close(client_socket);
    pthread_exit(NULL);
}
