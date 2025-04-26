#include <string.h>
#include <stdint.h> 
#include "hash_map.h"

int string_hash_code(hash_map* map, void* key) {
    if (map == NULL || key == NULL) {
        return 0;
    }

    char* str = (char*)key;
    unsigned long h = 0;
    while (*str) {
        h = (h << 4) + *str++;
        unsigned long g = h & 0xF0000000L;
        if (g) {
            h ^= g >> 24;
        }
        h &= ~g;
    }

    return h & (map->capacity-1);
}

int string_compare(void* key1, void* key2) {
    if (key1 == NULL || key2 == NULL) {
        return 1;
    }

    char* s1 = (char*)key1;
    char* s2 = (char*)key2;

    return strcmp(s1, s2);
}

hash_map* new_hash_map(size_t capacity, hash_code hash_code, compare compare) {
    hash_map* map = (hash_map*)malloc(sizeof(hash_map));
    if (map == NULL) {
        return NULL;
    }
    
    map->hash_code = hash_code ? hash_code : string_hash_code;
    map->compare = compare ? compare : string_compare;

    if (capacity <= 0) capacity = DEFAULT_HASH_SIZE;
    if (capacity > (size_t)SIZE_MAX) capacity = SIZE_MAX;

    map->capacity = capacity;
    map->size = 0;
    map->table = (entry**)malloc(sizeof(entry*) * capacity);
    for (size_t i = 0; i < capacity; i++) {
        map->table[i] = NULL;
    }

    return map;
}

static void resize(hash_map* map) {
    if (map == NULL) {
        return;
    }

    size_t new_capacity = map->capacity << 1;
    entry** new_table = (entry**)malloc(sizeof(entry*) * new_capacity);
    if (new_table == NULL) {
        return;
    }
    for (size_t i = 0; i < new_capacity; i++) {
        new_table[i] = NULL;
    }

    size_t old_capacity = map->capacity;
    map->capacity = new_capacity;
    for (size_t i = 0; i < old_capacity; i++) {
        entry* e = map->table[i];
        while (e != NULL) {
            entry* next = e->next;
            int index = map->hash_code(map, e->key);
            e->next = new_table[index];
            new_table[index] = e;
            e = next;
        }
    }

    free(map->table);
    map->table = new_table;
    map->capacity = new_capacity;
}

void put_hash_map(hash_map* map, void* key, void* value) {
    if (map == NULL || key == NULL) {
        return;
    }

    int index = map->hash_code(map, key);
    entry* e = map->table[index];

    while (e != NULL) {
        if (map->compare(e->key, key) == 0) {
            e->value = value;
            return;
        }
        e = e->next;
    }

    if (map->size >= map->capacity * DEFAULT_LOAD_FACTOR) {
        resize(map);
        index = map->hash_code(map, key);
    }
    entry* new_entry = (entry*)malloc(sizeof(entry));
    if (new_entry == NULL) {
        return;
    }
    map->size++;
    new_entry->key = key;
    new_entry->value = value;
    new_entry->next = map->table[index];
    map->table[index] = new_entry;
}

void* get_hash_map(hash_map* map, void* key) {
    if (map == NULL || key == NULL) {
        return NULL;
    }

    int index = map->hash_code(map, key);
    entry* e = map->table[index];

    while (e != NULL) {
        if (map->compare(e->key, key) == 0) {
            return e->value;
        }
        e = e->next;
    }

    return NULL;
}

void remove_hash_map(hash_map* map, void* key) {
    if (map == NULL || key == NULL) {
        return;
    }

    int index = map->hash_code(map, key);
    entry* e = map->table[index];
    entry* prev = NULL;

    while (e != NULL) {
        if (map->compare(e->key, key) == 0) {
            if (prev == NULL) {
                map->table[index] = e->next;
            } else {
                prev->next = e->next;
            }
            free(e);
            map->size--;
            return;
        }
        prev = e;
        e = e->next;
    }
}

void free_hash_map(hash_map* map, int free_key, int free_value) {
    if (map == NULL) {
        return;
    }

    for (size_t i = 0; i < map->capacity; i++) {
        entry* e = map->table[i];
        while (e != NULL) {
            entry* temp = e;
            e = e->next;
            if (free_key) {
                free(temp->key);
            }
            if (free_value) {
                free(temp->value);
            }
            free(temp);
        }
    }

    free(map->table);
    free(map);
}

void clear_hash_map(hash_map* map, int free_key, int free_value) {
    if (map == NULL) {
        return;
    }

    for (size_t i = 0; i < map->capacity; i++) {
        entry* e = map->table[i];
        while (e != NULL) {
            entry* temp = e;
            e = e->next;
            if (free_key) {
                free(temp->key);
            }
            if (free_value) {
                free(temp->value);
            }
            free(temp);
        }
        map->table[i] = NULL;
    }
    map->size = 0;
}