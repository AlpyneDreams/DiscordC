#pragma once

#include <pthread.h>
#include <arpa/inet.h>

#include <openssl/ssl.h>


int discord_protocol(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len);
