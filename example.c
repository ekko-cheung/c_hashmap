#include <stdio.h>
#include "hash_map.h"

int main() {
    hash_map* map = new_hash_map(0, string_hash_code, string_compare);
    if (map == NULL) {
        return 1;
    }

    char* key = "key1";
    char* value = "value1";

    printf("Adding Key: %s, Value: %s\n", key, value);
    put_hash_map(map, key, value);

    // Output Key: key1, Value: value1
    printf("Key: %s, Value: %s\n", (char*)key, (char*)get_hash_map(map, key));
    printf("Removing Key: %s\n", key);
    remove_hash_map(map, key);
    void* result = get_hash_map(map, key);
    printf("Key: %s, Value: %s\n", (char*)key, result ? (char*)result : "NULL");

    free_hash_map(map, FREE_KEY, FREE_VALUE);

    return 0;
}