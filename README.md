# c_hashmap

## Quick start

1. Clone the repository
```bash
git clone https://github.com/your-repo/c_hashmap.git
cd c_hashmap
```

2. Complite the code
```c
gcc -o hashmap example.c hashmap.c
```

3. Run the program
```bash
./hashmap
```

## Usage
Here’s an example of how to use the hash map:
```c
#include "hashmap.h"

int main() {
   hash_map* map = new_hash_map(0, NULL, NULL);
    if (map == NULL) {
        return 1;
    }

    char* key = "key1";
    char* value = "value1";

    put_hash_map(map, key, value);

    void* v = get_hash_map(map, key);
    remove_hash_map(map, key);

    free_hash_map(map, FREE_KEY, FREE_VALUE);

    return 0;
}
```