#pragma once

#include "client.h"

int validateToken(const char* token);
void realloc_copy(char** dest, const char* src);

int client_ws_receive_callback(client_websocket_t* socket, char* data, size_t length);
int client_ws_connection_error_callback(client_websocket_t* socket, char* data, size_t length);
