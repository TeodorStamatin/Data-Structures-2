/* Copyright 2023 <> */
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include "utils.h"

#define HMAX 1600

server_memory *init_server_memory() {
	server_memory *server = malloc(sizeof(server_memory));
	DIE(server == NULL, "please malloc server");

	server->ht = ht_create(HMAX, hash_function_int,
	compare_function_ints, key_val_free_function);
	DIE(server->ht == NULL, "please malloc ht");

	return server;
}

void server_store(server_memory *server, char *key, char *value) {
    ht_put(server->ht, key, strlen(key) + 1,
	value, strlen(value) + 1);
}


char *server_retrieve(server_memory *server, char *key) {
	return ht_get(server->ht, key);
}

void server_remove(server_memory *server, char *key) {
	ht_remove_entry(server->ht, key);
}

void free_server_memory(server_memory *server) {
	ht_free(server->ht);
	free(server);
}
