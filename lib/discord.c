#include <openssl/ssl.h>
#include <openssl/engine.h>
#include <openssl/conf.h>

#include <curl/curl.h>
#include <libwebsockets.h>

#include "discord.h"

void discord_init() {
	curl_global_init(CURL_GLOBAL_ALL);
#ifdef LIBWEBSOCKET_DEBUG
	/* enable debugging of libwebsockets in case of errors */
	lws_set_log_level(1023 ^ LLL_PARSER ^ LLL_EXT, NULL);
#else
	lws_set_log_level(0, NULL);
#endif
}


void discord_cleanup() {
	ERR_remove_thread_state(NULL);

	EVP_cleanup();
	ERR_free_strings();
	CRYPTO_cleanup_all_ex_data();
	ENGINE_cleanup();
	CONF_modules_finish();
	CONF_modules_unload(1);
	COMP_zlib_cleanup();
	/* TODO: figure out what ssl define this is behind */
	/* SSL_COMP_free_compression_methods(); */
	sk_SSL_COMP_free(SSL_COMP_get_compression_methods());
	RAND_cleanup();

	curl_global_cleanup();
}
