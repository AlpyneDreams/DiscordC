#pragma once

#include "client.h"

// Fills 'user' with data from JSON
void discord_parse_user(cJSON* data, user_info_t* user);


void cache_init(struct discord_cache** cache);

// Add user to the cache
void cache_add_user(discord_client_t* client, user_info_t* user);