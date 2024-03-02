/* Copyright 2023 <> */
#include <stdlib.h>
#include <string.h>

#include "load_balancer.h"
#include "utils.h"
#define HMAX 1000

struct load_balancer {
    
    hashtable_t *ht;
    void *data;
};

unsigned int hash_function_servers(void *a) {
    unsigned int uint_a = *((unsigned int *)a);

    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = (uint_a >> 16u) ^ uint_a;
    return uint_a;
}

unsigned int hash_function_key(void *a) {
    unsigned char *puchar_a = (unsigned char *)a;
    unsigned int hash = 5381;
    int c;

    while ((c = *puchar_a++))
        hash = ((hash << 5u) + hash) + c;

    return hash;
}

load_balancer *init_load_balancer() {

    load_balancer *main = malloc(sizeof(load_balancer));
    DIE(main == NULL, "malloc main");

    main->ht = NULL;
    DIE(main->ht == NULL, "malloc ht");

    main->data = NULL;

    return main;
}
/**
 * load_add_server() - Adds a new server to the system.
 * @arg1: Load balancer which distributes the work.
 * @arg2: ID of the new server.
 *
 * The load balancer will generate 3 replica labels and it will
 * place them inside the hash ring. The neighbor servers will
 * distribute some the objects to the added server.
 *
 * Hint:
 * Resize the servers array to add a new one.
 * Add each label in the hashring in its appropiate position.
 * Do not forget to resize the hashring and redistribute the objects
 * after each label add (the operations will be done 3 times, for each replica).
 */
void loader_add_server(load_balancer *main, int server_id) {
    
    unsigned int hash1 = hash_function_servers(&server_id) % HMAX;
    unsigned int hash2 = (hash_function_servers(&server_id) + 100000) % HMAX;
    unsigned int hash3 = (hash_function_servers(&server_id) + 200000) % HMAX;

    hashtable_t *ht = main->ht;
    // server_memory *server1 = malloc(sizeof(server_memory));
    // server_memory *server2 = malloc(sizeof(server_memory));
    // server_memory *server3 = malloc(sizeof(server_memory));

    // server1->server_id = server_id;
    // ht->buckets[hash1] = NULL;
}

void loader_remove_server(load_balancer *main, int server_id) {
    /* TODO 3 */
}

void loader_store(load_balancer *main, char *key, char *value, int *server_id) {
    /* TODO 4 */
}

char *loader_retrieve(load_balancer *main, char *key, int *server_id) {
    /* TODO 5 */
    return NULL;
}

void free_load_balancer(load_balancer *main) {
    /* TODO 6 */
}