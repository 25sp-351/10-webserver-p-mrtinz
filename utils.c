#include <string.h>

const char* get_mime_type(const char* extension) {
    if (strcmp(extension, "html") == 0) return "text/html";
    if (strcmp(extension, "css") == 0) return "text/css";
    if (strcmp(extension, "js") == 0) return "application/javascript";
    if (strcmp(extension, "png") == 0) return "image/png";
    if (strcmp(extension, "jpg") == 0 || strcmp(extension, "jpeg") == 0) return "image/jpeg";
    if (strcmp(extension, "gif") == 0) return "image/gif";
    if (strcmp(extension, "txt") == 0) return "text/plain";
    return "application/octet-stream";
}
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "utils.h"

void send_http_response(int client_socket, const char *status, const char *content_type, const char *body, int body_length) {
    char header[1024];
    int header_length = snprintf(header, sizeof(header),
                                 "HTTP/1.1 %s\r\n"
                                 "Content-Type: %s\r\n"
                                 "Content-Length: %d\r\n"
                                 "Connection: close\r\n"
                                 "\r\n",
                                 status, content_type, body_length);

    send(client_socket, header, header_length, 0);
    send(client_socket, body, body_length, 0);
}
