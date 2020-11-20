#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "client.h"
#include "client_internal.h"

discord_client_t* discord_create_client(discord_client_callbacks_t* callbacks, const char* token) {
	if (!token || !validateToken(token)) {
		/* invalid token */
		return NULL;
	}

	discord_client_t* client = malloc(sizeof(discord_client_t));
	client->_token = NULL;
	client->_gateway_thread = NULL;
	client->_callbacks = callbacks;

	/* if cURL fails to init, then we can't do anything! */
	if (!(client->_curl_handle = curl_multi_init()))
		return NULL;

	realloc_copy(&client->_token, token);

	/* TODO: this */
	return client;
}

void discord_free_client(discord_client_t* client) {
	/* disconnect the websocket if we are connected */
	if (client->_gateway_thread) {
		discord_client_disconnect(client);
	}

	curl_multi_cleanup(client->_curl_handle);

	free(client->_token);
	free(client);
}


// start routine for the gateway_thread
void *client_listen(void* arg) {
	discord_client_t* client = arg;

	client_websocket_callbacks_t callbacks;
	memset(&callbacks, 0, sizeof(callbacks));

	callbacks.on_receive = client_ws_receive_callback;
	callbacks.on_connection_error = client_ws_connection_error_callback;

	client->_client_socket = websocket_create(&callbacks);
	websocket_set_userdata(client->_client_socket, client);

#define CONNECT_TO "wss://gateway.discord.gg/?v=6&encoding=json"
	websocket_connect(client->_client_socket, CONNECT_TO);

	/* TODO: don't use internal fields */
	while (client->_client_socket->_remain_connected) {
		if (!client->_client_socket->_connected) {
			websocket_connect(client->_client_socket, CONNECT_TO);
		}
		websocket_think(client->_client_socket);
	}
#undef CONNECT_TO

	return NULL;
}

void discord_client_connect(discord_client_t* client) {
	pthread_t* gateway_thread = malloc(sizeof(pthread_t));
	pthread_create(gateway_thread, NULL, client_listen, client);
	client->_gateway_thread = gateway_thread;
}

void discord_client_disconnect(discord_client_t* client) {
	websocket_disconnect(client->_client_socket);

	/* cancel the gateway thread if it exists */
	if (client->_gateway_thread) {
		if (client->_heartbeat_thread) {
			client->_send_heartbeats = 0;
			pthread_cancel(*client->_heartbeat_thread);
			pthread_join(*client->_heartbeat_thread, NULL);
			client->_heartbeat_thread = NULL;
		}

		pthread_cancel(*client->_gateway_thread);
		pthread_join(*client->_gateway_thread, NULL);

		free(client->_gateway_thread);
		client->_gateway_thread = NULL;
	}

	websocket_free(client->_client_socket);
}
