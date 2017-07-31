#pragma once

#include <pthread.h>
#include <time.h>
#include <curl/curl.h>

typedef struct discord_client discord_client_t;

#include "websocket.h"
#include "message.h"
#include "discord.h"

#include <cjson/cJSON.h>

#define CALLBACK_TYPE(NAME, RET, ...) typedef RET (*client_ ## NAME ## _callback)(discord_client_t* client, __VA_ARGS__)
#define CALLBACK_TYPE_VOID(NAME, RET) typedef RET (*client_ ## NAME ## _callback)(discord_client_t* client)

CALLBACK_TYPE(unhandled_dispatch, void, char* dispatch_str, cJSON* root);
CALLBACK_TYPE_VOID(connected, void);
CALLBACK_TYPE(connection_error, int, uint16_t code, const char* reason);
CALLBACK_TYPE(latency_updated, int, double oldLatency, double newLatency);
CALLBACK_TYPE(message_receive, int, message_t* message);

typedef struct {
	client_unhandled_dispatch_callback	on_unhandled_dispatch;

	client_connected_callback			on_connected;
	client_connection_error_callback	on_connection_error;
	client_latency_updated_callback		on_latency_update;
	client_message_receive_callback		on_message_receive;
} discord_client_callbacks_t;

struct discord_client {
	char* _token;

	double _latency;
	int _heartbeat_interval;
	int _seq;
	struct timespec* _heartbeat_start;
	uint8_t _send_heartbeats;

	CURLM* _curl_handle;
	pthread_t* _heartbeat_thread;
	pthread_t* _gateway_thread;

	client_websocket_t* _client_socket;
	discord_client_callbacks_t* _callbacks;
};

LIB_EXPORT discord_client_t* discord_create_client(discord_client_callbacks_t* callbacks, const char* token);
LIB_EXPORT void discord_free_client(discord_client_t* client);

LIB_EXPORT void client_connect(discord_client_t* client);
LIB_EXPORT void client_disconnect(discord_client_t* client);

LIB_EXPORT void client_send_message(discord_client_t* client, uint64_t channel_id, const char* contents);
