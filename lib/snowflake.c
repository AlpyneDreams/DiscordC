#include "snowflake.h"

#include <stdio.h>

uint64_t read_snowflake(const char* string)
{
    uint64_t snowflake = 0UL;
    sscanf(string, "%lu", &snowflake);
    return snowflake;
}