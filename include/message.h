#pragma once

#include <stdint.h>

#include "user.h"

typedef struct {
	uint64_t _channel_id;
	uint64_t _message_id;
	const char* _contents;
	user_info_t* _sender;
} message_t;
