#include "discord_objects.h"
#include "snowflake.h"

#include <string.h>

void discord_parse_user(cJSON* data, user_info_t* user)
{
    user->id = read_snowflake(cJSON_GetObjectItem(data, "id")->valuestring);
    memcpy(user->discriminator, cJSON_GetObjectItem(data, "discriminator")->valuestring, DISCORD_DISCRIMINATOR_LENGTH);

    char* name = cJSON_GetObjectItem(data, "username")->valuestring;
    user->name_len = strlen(name);
    user->name = name;
}