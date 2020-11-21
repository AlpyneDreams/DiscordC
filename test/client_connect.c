#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

#include "discord.h"
#include "client.h"

int on_message_received(discord_client_t* client, message_t* message) {
	//printf("Message received: %s\n", message->contents); /* TODO: provide better methods for this */
	if (strstr(message->contents, "§ping") == message->contents) {
		printf("Received ping command!\n");

		char data[256];
		sprintf(data, "Hello, world, from C!\nUsing %s at <%s>!\nCurrent Gateway Latency: %1.0fms", DISCCORD_VERSION_STRING, DISCCORD_PROJECT_URL, client->_latency);

		//printf("%s\n", data);

		discord_send_message(client, message->channel_id, data);
	}
	return 0;
}

void on_unhandled_dispatch(discord_client_t* client, char* dispatch_str, cJSON* root) {
	printf("Unhandled Dispatch: %s\n", dispatch_str);
}

void on_connected(discord_client_t* client) {
	printf("Connected as user @%s#%s\n", client->user.name, client->user.discriminator);
}

char* get_token(const char* file) {
	FILE* fp = fopen(file, "r");

	if (!fp) {
		perror("fopen");
		return NULL;
	}

	static char token[128];
	fgets(token, 128, fp);
	fclose(fp);

	// remove trailing newlines
	strtok(token, "\n");

	return &token[0];
}

int main() {

	const char* token = get_token("token.data.txt");

	discord_init();

	discord_client_callbacks_t callbacks;
	callbacks.on_connected = on_connected;
	callbacks.on_message_receive = on_message_received;
	callbacks.on_unhandled_dispatch = on_unhandled_dispatch;

	printf("Creating client...\n");
	discord_client_t* client = discord_create_client(&callbacks, token);
	if (client) {
		printf("Successfully created client!\n");
	} else {
		fprintf(stderr, "Failed to create discord client!\n");
		return 1;
	}

	printf("Connecting...\n");
	discord_client_connect(client);

	sleep(120);

	printf("Disconnecting...\n");
	discord_client_disconnect(client);

	sleep(3); /* ensure there's enough time to disconnect cleanly */

	printf("Freeing client...\n");
	discord_free_client(client);
	printf("Successfully freed client!\n");

	discord_cleanup();
	return 0;
}
