/* Copyright 2023 <> */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "load_balancer.h"
#include "utils.h"
#include "server.h"

#define POWER 100000

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
    load_balancer *balancer = malloc(sizeof(load_balancer));
    DIE(balancer == NULL, "please malloc balancer");

    balancer->servers = NULL;

    balancer->nr_servers = 0;

    return balancer;
}

void sort_servers_by_hash(load_balancer *main) {
    server_memory *temp;

    for (int i = 0; i < main->nr_servers - 1; i++) {
        for (int j = i + 1; j < main->nr_servers; j++) {
            long hash1 = main->servers[i]->server_id +
            main->servers[i]->replica_id * POWER;
            long hash2 = main->servers[j]->server_id +
            main->servers[j]->replica_id * POWER;
            if (hash_function_servers(&hash1) > hash_function_servers(&hash2)) {
                temp = main->servers[i];
                main->servers[i] = main->servers[j];
                main->servers[j] = temp;
            }
        }
    }
}

void loader_add_server(load_balancer *main, int server_id) {
    main->nr_servers += 3;

    main->servers = realloc(main->servers,
    (main->nr_servers) * sizeof(struct server_memory));
    DIE(main->servers == NULL, "malloc failed");

    main->servers[main->nr_servers - 3] = init_server_memory();
    main->servers[main->nr_servers - 3]->server_id = server_id;
    main->servers[main->nr_servers - 3]->replica_id = 0;

    main->servers[main->nr_servers - 2] = init_server_memory();
    main->servers[main->nr_servers - 2]->server_id = server_id;
    main->servers[main->nr_servers - 2]->replica_id = 1;

    main->servers[main->nr_servers - 1] = init_server_memory();
    main->servers[main->nr_servers - 1]->server_id = server_id;
    main->servers[main->nr_servers - 1]->replica_id = 2;

    sort_servers_by_hash(main);

    for (int i = 0; i < main->nr_servers; i++) {
        hashtable_t *ht = main->servers[i]->ht;
        for (int j = 0; j < ht->size; j++) {
            if (ht->buckets[j] != NULL) {
                linked_list_t *list = ht->buckets[j];
				ll_node_t *elem = list->head;
                while (elem != NULL) {
                    info data_info = *(info *)elem->data;
                    char *key = data_info.key;
                    char *value = data_info.value;

                    int k = 0;
                    while (1) {
                        if (k == main->nr_servers)
                            break;

                        long hash1 = main->servers[k]->server_id +
                        main->servers[k]->replica_id * POWER;
                        if (hash_function_key(key) >
                        hash_function_servers(&hash1)) {
                            k++;
                        } else {
                            break;
                        }
                    }

                    if (k == main->nr_servers)
                        k = 0;

                    if (k != i) {
                        server_store(main->servers[k], key, value);
                    }
                    elem = elem->next;
                }
            }
        }
    }
}

void loader_remove_server(load_balancer *main, int server_id) {
    int k = 0;
    while (k < main->nr_servers) {
        if (main->servers[k]->server_id == server_id) {
            server_memory *temp1 = main->servers[k];
            server_memory *temp2 = (k == main->nr_servers - 1)
            ? main->servers[0] : main->servers[k + 1];

            hashtable_t *ht = main->servers[k]->ht;
            for (int i = 0; i < ht->size; i++) {
                if (ht->buckets[i] != NULL) {
                    linked_list_t *list = ht->buckets[i];
                    ll_node_t *curr_node = list->head;
                    while (curr_node) {
                        info *data_info = curr_node->data;
                        char *key = data_info->key;
                        char *value = data_info->value;

                        server_store(temp2, key, value);
                        curr_node = curr_node->next;
                    }
                }
            }

            free_server_memory(temp1);
            for (int i = k; i < main->nr_servers - 1; i++) {
                main->servers[i] = main->servers[i + 1];
            }

            main->nr_servers--;
            main->servers = realloc(main->servers,
            (main->nr_servers) * sizeof(struct server_memory));
            DIE(main->servers == NULL, "malloc failed");
        } else {
            k++;
        }
    }
}

void loader_store(load_balancer *main, char *key, char *value, int *server_id) {
    int k = 0;
    while (1) {
        if (k == main->nr_servers)
            break;

        long hash1 = main->servers[k]->server_id +
        main->servers[k]->replica_id * POWER;
        if (hash_function_key(key) > hash_function_servers(&hash1)) {
            k++;
        } else {
            break;
        }
    }
    if (k == main->nr_servers)
        k = 0;

    server_memory *temp = main->servers[k];
    server_store(temp, key, value);
    *server_id = temp->server_id;
}

char *loader_retrieve(load_balancer *main, char *key, int *server_id) {
    int k = 0;
    while (1) {
        if (k == main->nr_servers)
            break;

        long hash1 = main->servers[k]->server_id +
        main->servers[k]->replica_id * POWER;
        if (hash_function_key(key) > hash_function_servers(&hash1)) {
            k++;
        } else {
            break;
        }
    }
    if (k == main->nr_servers)
        k = 0;

    *server_id = main->servers[k]->server_id;
    return server_retrieve(main->servers[k], key);
}

void free_load_balancer(load_balancer *main) {
    for (int i = 0; i < main->nr_servers; i++) {
        free_server_memory(main->servers[i]);
    }
    free(main->servers);
    free(main);
}
