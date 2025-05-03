# dast
DAta STructures (DAST).  A collection of some commonly used data structures written in C.

* Dynamic array (`array_t`): a resizeable contiguous array containing items of the same type.
* String (`string_t`): a thin wrapper for a character array plus a length.
* Hashmap (`hashmap_t`): a hash table mapping one one data type to another.

Features:

* Fast
* Lightweight
* Written in pure C89
* Optional usage of C standard library
* Support for user-defined memory management functions (malloc, realloc, free).

## array

```c
array_t data;
array_init(&data, sizeof(float));

float value = 1.0f;
array_push_back(&data, &value);

float ret = *(float*)array_get(&data, 0);
assert(ret == value);
assert(data.size == 1);

array_uninit(&data);
```

## string

```c
// Allocated on the heap, must be freed
string_t s1 = string_from_literal("Allocated string");
string_free(&s1);

// Allocated on the stack, automatically freed at the end of the current scope
string_t s2 = string_scoped_lit("Local string");

// Formatted using variadic arguments
string_t s3 = string_from_fmt("%f %d %s", 10.0f, -1, "hello");
string_free(&s3);

// Using custom allocation functions
dast_allocator_t my_alloc = {.alloc = my_malloc, .realloc = my_realloc, .free = my_free};
string_t s4 = string_from_literal_custom("Custom allocated string", my_alloc); // will use `my_alloc`
string_free(&s4); // will use 'my_free'
```

## hashmap

```c
hashmap_t map;
hashmap_init(&map, 10); // Starting capacity will be next prime number after given value

// Use string_t as key
string_t key = string_scoped_lit("key");
float value = 100.0f;
hashmap_set(&map, &key, &value);

float ret = *(float*)hashmap_get(&map, key);
assert(ret == value);

// Use anything as a key
uint64_t custom_key = 100;
size_t custom_key_len = sizeof(uint64_t);
float value2 = 200.0f;
hashmap_setb(&map, &custom_key, custom_key_len, value2);

float ret2 = *(float*)hashmap_getb(&map, &custom_key, custom_key_len);
assert(ret2 == value2);
hashmap_uninit(&map);

// Custom allocation functions
dast_allocator_t my_alloc = {.alloc = my_malloc, .realloc = my_realloc, .free = my_free};
hashmap_t map2;
hashmap_init_custom(&map, 10, my_alloc, dast_null, dast_null);
hashmap_uninit(&map2);

// Custom hashing and key comparison functions (for custom keys)
dast_bool my_cmp(const void* a, const void* b, dast_sz len) { return *(uint64_t*)a == *(uint64_t*)b; }
dast_u64  my_hash(const void* key, dast_sz key_len) { ... }

hashmap_t map3;
hashmap_init_custom(&map3, 10, my_alloc, my_hash, my_cmp);
hashmap_uninit(&map3);
```