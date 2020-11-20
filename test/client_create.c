#include <stdint.h>
#include <stdio.h>

#include "discord.h"
#include "client.h"

int main() {
	discord_init();

	printf("Creating client...\n");
	discord_client_t* client = discord_create_client(NULL, "");
	printf("Successfully created client!\n");

	printf("Freeing client...\n");
	discord_free_client(client);
	printf("Successfully freed client!\n");

	discord_cleanup();
	return 0;
}
