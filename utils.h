#ifndef UTILS_H
#define UTILS_H

void send_http_response(int client_socket, const char *status, const char *content_type, const char *body, int body_length);
const char* get_mime_type(const char* extension);

#endif
