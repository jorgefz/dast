#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "dast.h"
#include "hashmap.h"


#define NKEYS 1000
#define KEYLEN 10

int main(){

    srand((unsigned int)time(NULL));

    static char keys[NKEYS][KEYLEN + 1];

    // Generate keys
    for(uint32_t i = 0; i != NKEYS; ++i){
        for(uint32_t j = 0; j != KEYLEN; ++j){
            keys[i][j] = (char)(  rand() % ('z' + 1 - 'a') + 'a' );
        }
        keys[i][KEYLEN] = '\0';
    }
    printf("Keys generated\n");

    hashmap_t map;
    void* p = hashmap_init(&map, NKEYS*10);
    printf("Map initialised\n");

    // Insert keys
    for(uint32_t i = 0; i != NKEYS; ++i){
        hashmap_setb(&map, keys[i], KEYLEN+1, NULL);
    }
    printf("Keys inserted\n");

    // Scan for collisions
    hashmap_entry_t* entry;
    uint32_t collisions = 0;
    for(uint64_t i = 0; i != map.size; ++i){
        entry = map.table[i];
        if(entry && entry->next){
            printf("Collision of %s with ", entry->key);
            entry = entry->next;
            while (entry) {
                printf(" %s, ", entry->key);
                entry = entry->next;
            }
            printf("\n");
            collisions++;
        }
    }

    // Stats
    dast_sz mapsz = map.size;
    double loadf = (double)NKEYS / (double)mapsz;
    printf("Keys %u and map size %u (loading factor %.2g)\n", NKEYS, (unsigned int)map.size, loadf);
    printf("Collisions: %u\n", collisions);

    hashmap_uninit(&map);

    // Hashing test
    dast_u64 h1 = hashmap_FNV1a64_hash("danhuugadk", KEYLEN+1) % mapsz;
    dast_u64 h2 = hashmap_FNV1a64_hash("ympzximqqj", KEYLEN+1) % mapsz;
    printf("%llu = %llu (%d)\n", h1, h2, h1==h2);

    return 0;
}