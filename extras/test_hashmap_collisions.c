#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "dast.h"
#include "hashmap.h"


#define NKEYS 1000
#define KEYLEN 10

int main(){

    srand(time(NULL));

    static char keys[NKEYS][KEYLEN + 1];

    // Generate keys
    for(uint32_t i = 0; i != NKEYS; ++i){
        for(uint32_t j = 0; j != KEYLEN; ++j){
            keys[i][j] = (char)(rand() % 256);
        }
        keys[i][KEYLEN] = '\0';
    }

    hashmap_t map;
    hashmap_init(&map, NKEYS*2);

    // Insert keys
    for(uint32_t i = 0; i != NKEYS; ++i){
        hashmap_setb(&map, keys[i], KEYLEN, NULL);
    }

    // Scan for collisions
    hashmap_entry_t* entry;
    uint32_t collisions = 0;
    for(uint64_t i = 0; i != map.size; ++i){
        entry = map.table[i];
        if(entry && entry->next){
            collisions++;
        }
    }
    hashmap_uninit(&map);

    // Stats
    double loadf = (double)NKEYS / (double)map.size;
    printf("Keys %u and map size %lu (loading factor %.2g)\n", NKEYS, map.size, loadf);
    printf("Collisions: %u\n", collisions);

    return 0;
}