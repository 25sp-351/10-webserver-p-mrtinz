#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "static_handler.h"
#include "utils.h"
#include "http_server.h"

#define BUFFER_SIZE 4096

void serve_static_file(int client_socket, const char *path) {
    char file_path[2048];
    snprintf(file_path, sizeof(file_path), ".%s", path);

    int file_descriptor = open(file_path, O_RDONLY);
    if (file_descriptor < 0) {
        send_http_response(client_socket, "404 Not Found", "text/plain", "File not found.", 15);
        return;
    }

    struct stat file_stat;
    fstat(file_descriptor, &file_stat);
    int file_size = file_stat.st_size;

    const char *extension = strrchr(file_path, '.');
    const char *content_type = (extension) ? get_mime_type(extension) : "application/octet-stream";

    char header[BUFFER_SIZE];
    int header_length = snprintf(header, sizeof(header),
                                  "HTTP/1.1 200 OK\r\n"
                                  "Content-Type: %s\r\n"
                                  "Content-Length: %d\r\n"
                                  "Connection: close\r\n"
                                  "\r\n",
                                  content_type, file_size);

    send(client_socket, header, header_length, 0);

    char file_buffer[BUFFER_SIZE];
    int bytes_read;
    while ((bytes_read = read(file_descriptor, file_buffer, BUFFER_SIZE)) > 0) {
        send(client_socket, file_buffer, bytes_read, 0);
    }

    close(file_descriptor);
}
