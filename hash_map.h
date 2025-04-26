#pragma once
#include <stdlib.h>

#define DEFAULT_HASH_SIZE 8
#define DEFAULT_LOAD_FACTOR 0.75
#define FREE_KEY 1
#define FREE_VALUE 1
#define NO_FREE 0

typedef struct entry {
    void* key;
    void* value;
    struct entry* next;
} entry;

typedef struct hash_map hash_map;

typedef int (*hash_code)(hash_map* map, void* key);

int string_hash_code(hash_map* map, void* key);

typedef int (*compare)(void* a, void* b);

int string_compare(void* a, void* b);

void put_hash_map(hash_map* map, void* key, void* value);

void* get_hash_map(hash_map* map, void* key);

void remove_hash_map(hash_map* map, void* key);

void free_hash_map(hash_map* map, int free_key, int free_value);

void clear_hash_map(hash_map* map, int free_key, int free_value);

typedef struct hash_map {
    entry** table;
    hash_code hash_code;
    compare compare;
    size_t size;
    size_t capacity;
} hash_map;

hash_map* new_hash_map(size_t capacity, hash_code hash_code, compare compare);