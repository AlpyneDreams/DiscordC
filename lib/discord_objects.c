#include "discord_objects.h"
#include "snowflake.h"

#include <string.h>

void discord_parse_user(cJSON* data, user_info_t* user) {
    user->id = read_snowflake(cJSON_GetObjectItem(data, "id")->valuestring);
    memcpy(user->discriminator, cJSON_GetObjectItem(data, "discriminator")->valuestring, DISCORD_DISCRIMINATOR_LENGTH);

    char* name = cJSON_GetObjectItem(data, "username")->valuestring;
    user->name_len = strlen(name);
    user->name = name;
}

struct cache {
    size_t count;
    size_t allocated_count;

    uint64_t lowest_id;
    uint64_t highest_id;
    uint64_t* ids;

    union {
        user_info_t** users;
    };
};

struct discord_cache {
    struct cache users;
};

void cache_init(struct discord_cache** cachePtr) {
    *cachePtr = malloc(sizeof(struct discord_cache));

    struct discord_cache* cache = *cachePtr;

    *cache = (struct discord_cache){.users = {0, 512, UINT64_MAX, 0, NULL, NULL}};

    cache->users.ids = malloc(512 * sizeof(uint64_t));
    cache->users.users = malloc(512 * sizeof(user_info_t*));
}

/**************************************************
	Users
**************************************************/

void cache_add_user(discord_client_t* client, user_info_t* user) {
    struct cache* cache_users = &client->_cache->users;
    
    // TODO: cache_add_user grow cache
    if (cache_users->count >= cache_users->allocated_count) {
        printf("ERR: Trying to add new user, but cache needs to be exapanded (%zu/%zu). (Not implemented)\n",
            cache_users->count,
            cache_users->allocated_count
        );
        return;
    }

    size_t i = cache_users->count;
    cache_users->ids[i] = user->id;
    cache_users->users[i] = user;

    if (cache_users->highest_id < user->id) {
        cache_users->highest_id = user->id;
    }

    if (cache_users->lowest_id > user->id) {
        cache_users->lowest_id = user->id;
    }

    cache_users->count++;
}

user_info_t* get_user(discord_client_t* client, uint64_t id) {
    struct cache* cache_users = &client->_cache->users;

    // check for cached user first
    if (cache_users->lowest_id <= id && cache_users->highest_id >= id) {

        for (size_t i = 0; i < cache_users->count; i++) {
            if (cache_users->ids[i] == id) {
                return cache_users->users[i];
            }
        }
    }

    // TODO: API get_user
    printf("ERR: Trying to get user %lu that isn't cached! (Not implemented)\n", id);

    return NULL;
}