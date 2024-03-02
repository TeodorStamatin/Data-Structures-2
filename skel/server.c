/* Copyright 2023 <> */
#include <stdlib.h>
#include <string.h>

#include "server.h"

struct server_memory {

	int server_id;
	char *key;
	int replica;
};

server_memory *init_server_memory() {
	
	server_memory *server = malloc(sizeof(server_memory));
	DIE(server == NULL, "malloc server");

	server->key = NULL;
	server->replica = 0;

	return server;
}

void server_store(server_memory *server, char *key, char *value) {
	
	server->key = malloc(strlen(key) + 1);
	DIE(server->key == NULL, "malloc key");

	strcpy(server->key, key);
	server->replica = 1;
}

char *server_retrieve(server_memory *server, char *key) {

	if (strcmp(server->key, key) == 0) {
		return server->key;
	}

	return NULL;
}

void server_remove(server_memory *server, char *key) {
	
	if (strcmp(server->key, key) == 0) {
		free(server->key);
		server->key = NULL;
		server->replica = 0;
	}
}

void free_server_memory(server_memory *server) {
	
	server_remove(server, server->key);
	free(server);
}