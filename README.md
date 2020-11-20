# DiscordC #
A Discord library written in C, forked from disccord.

## Dependencies ##


- [libcurl](https://curl.haxx.se/libcurl/)
- [openssl](https://www.openssl.org/)
- [libsodium](https://download.libsodium.org/doc/)
- pthreads
- [libwebsockets master](https://libwebsockets.org/)    (tested with v2.3.0)
- cJSON                                                 (tested with v1.5.7)

You can retrieve the first three with apt-get:
```shell
sudo apt-get install libcurl4-openssl-dev libssl-dev libsodium-dev
```

You may have to build libwebsockets (and maybe cJSON) yourself if you do not have a package for it.

## Building ##
Building should be as simple as using the included makefile:

```shell
$make all
```

## Contributing ##

Help needed. Lots of it.

### Testing ###

Valgrind is currently broken with OpenSSL programs.

Please test your code before committing. Use the `make test` command to test
your code to ensure that it works correctly.

`client_connect_test` requires your token in `token.data.txt`

### Known Allocation Error Locations ###
These aren't my fault, they're leftover from the fork.


- `client_internal.c: client_ws_receive_callback -> cJSON_Parse`
- `websocket.c: websocket_free`
- `websocket_internal.c: on LWS_CALLBACK_CLIENT_RECEIVE calls realloc`
- `client.c: discord_send_message -> exec_request`

also segfault after freeing client sometimes
