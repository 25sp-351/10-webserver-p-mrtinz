#include <stdio.h>
#include <string.h>
#include "calc_handler.h"
#include "http_server.h"

void handle_calculation_request(int client_socket, const char *path) {
    char operation[8];
    int num1, num2;
    if (sscanf(path, "/calc/%[^/]/%d/%d", operation, &num1, &num2) == 3) {
        char result_body[1024];
        int result = 0;

        if (strcmp(operation, "add") == 0) {
            result = num1 + num2;
        } else if (strcmp(operation, "mul") == 0) {
            result = num1 * num2;
        } else if (strcmp(operation, "div") == 0) {
            if (num2 == 0) {
                send_http_response(client_socket, "400 Bad Request", "text/plain", "Division by zero.", 17);
                return;
            }
            result = num1 / num2;
        } else {
            send_http_response(client_socket, "400 Bad Request", "text/plain", "Unknown operation.", 18);
            return;
        }

        snprintf(result_body, sizeof(result_body), "<html><body><h1>Result: %d</h1></body></html>", result);
        send_http_response(client_socket, "200 OK", "text/html", result_body, strlen(result_body));
    } else {
        send_http_response(client_socket, "400 Bad Request", "text/plain", "Invalid /calc request.", 22);
    }
}
