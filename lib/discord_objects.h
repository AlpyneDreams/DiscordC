#pragma once

#include "client.h"

// Fills 'user' with data from JSON
void discord_parse_user(cJSON* data, user_info_t* user);