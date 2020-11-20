#include <stdbool.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#include "client.h"
#include "client_internal.h"

enum http_content {
	CONTENT_NONE,
	CONTENT_JSON,
	CONTENT_MULTIPART
};

struct curl_req {
	CURL* handle;
	struct curl_slist* headers;
};

static size_t curl_header_callback(char* buffer, size_t size, size_t nitems, void* userdata) {
	(void)buffer;
	(void)userdata;

	return size * nitems;
}
struct curl_req* setup_request(discord_client_t* client, char* endpoint, size_t len) {
	CURL* handle = curl_easy_init();

	if (!handle)
		return NULL;

	struct curl_req* req = malloc(sizeof(struct curl_req));

	char url[sizeof(DISCORD_API_URL) + len];
	sprintf(url, "%s%s", DISCORD_API_URL, endpoint);

	curl_easy_setopt(handle, CURLOPT_URL, url);
	curl_easy_setopt(handle, CURLOPT_USERAGENT, DISCCORD_USER_AGENT);

	curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, curl_header_callback);
	curl_easy_setopt(handle, CURLOPT_HEADERDATA, req);

	char auth[128];
	sprintf(auth, "Authorization: %s %s", "Bot", client->_token);

	struct curl_slist* ext_headers = curl_slist_append(NULL, auth);

	req->handle = handle;
	req->headers = ext_headers;

	return req;
}

void finish_request(CURL* handle, struct curl_req* req) {
	curl_easy_setopt(handle, CURLOPT_HTTPHEADER, req->headers);

	/* curl_multi_add_handle(client->_curl_handle, handle); */
	CURLcode response_code = curl_easy_perform(handle);
	if(response_code != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(response_code));
		return;
	}

	curl_easy_cleanup(handle);

	// there may be some potential memory fuckery down here

	//curl_slist_free_all(req->headers);
	//free(req);
}

void post(discord_client_t* client, struct curl_req* req, enum http_content type, char* body) {
	CURL* handle = req->handle;

	curl_easy_setopt(handle, CURLOPT_POSTFIELDS, body);
	switch (type) {
		case CONTENT_JSON:
			req->headers = curl_slist_append(req->headers, "Content-Type: application/json");
			break;
		default:
			break;
	}

	finish_request(handle, req);

	(void)client;
}

void get(discord_client_t* client, struct curl_req* req) {
	CURL* handle = req->handle;

	curl_easy_setopt(handle, CURLOPT_HTTPGET, true);

	finish_request(handle, req);

	(void)client;
}

user_info_t* discord_get_user(discord_client_t* client, uint64_t id) {
	char endpoint[38];
	sprintf(endpoint, "users/%lu", id);

	struct curl_req* req = setup_request(client, endpoint, 38);

	if (!req) return NULL;



}

void discord_send_message(discord_client_t* client, uint64_t channel_id, const char* contents) {
	char endpoint[38]; /* TODO: should we allocate more than this? */
	sprintf(endpoint, "channels/%lu/messages", channel_id);

	struct curl_req* req = setup_request(client, endpoint, 38);

	if (!req)
		return;

	cJSON* root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "content", contents);

	char* data = cJSON_PrintUnformatted(root);
	post(client, req, CONTENT_JSON, data);

	cJSON_Delete(root);
}
