
#include "include/array.h"

/* Factor by which the capacity is increased */
#define ARRAY_GROW_FACTOR 1.5

#define ARRAY_INITIAL_CAPACITY 1


/* Global allocation interface - defaults to std library */
/* static void* (*array_malloc)(dast_sz size)               = malloc;  */
/* static void* (*array_realloc)(void* block, dast_sz size) = realloc; */
/* static void  (*array_free)(void* block)                 = free;    */


/* Substitute for calloc to use malloc wrapper */
static void* array_calloc(dast_sz count, dast_sz size){
	void* block = array_malloc(count * size);
	if(block) memset(block, 0, count * size);
	return block;
}


/* Returns the nearest highest power of two of an integer */
static uint32_t nearest_pow2(uint32_t n){
    if(n <= 1) return 1;
    uint32_t x = 2;
    n--;
	while (n >>= 1) x <<= 1;
	return x;
}


/* Increments capacity of the array to the next power of two */
static array_t* extend_capacity(array_t* array){
	if(!array) return NULL;

	uint32_t capacity = array->capacity;
	void* data;

	if(capacity == 0) capacity++;
	else capacity *= 2;

	data = array_realloc(array->data, capacity * array->element_size);
	if(!data) return NULL;

	array->data = data;
	array->capacity = capacity;
	return array;
}


// -- INITIALIZATIONS -- //

/* Provide custom allocator functions */
/*
void array_set_alloc(
	void* (*user_alloc)  (dast_sz size),
	void* (*user_realloc)(void* block, dast_sz size),
	void  (*user_free)   (void* block)
) {
	if(user_alloc)   array_malloc  = user_alloc;
	if(user_realloc) array_realloc = user_realloc;
	if(user_free)    array_free    = user_free;
}
*/


/*
Initialises an array via a given pointer.
Should be later freed using `array_uninit`.
*/
int array_init(array_t* array, uint32_t element_size){
	if(!array) return 0;
	*array = (array_t){0};
	if(element_size == 0) return 0;
	array->element_size = element_size;
	return 1;
}

/*
Deallocates and resets the array data without freeing the array object itself.
*/
void array_uninit(array_t* array){
	if(!array) return;
	if(array->data) array_free(array->data);
	*array = (array_t){0};
}


/* Creates a new array of size zero */
array_t* array_create(uint32_t element_size){
	if(element_size == 0) return NULL;
	array_t* array = array_calloc(1, sizeof(array_t));
	if(!array) return NULL;
	array->element_size = element_size;
	return array;
}

/* Frees all the elements of an array */
void array_destroy(array_t* array){
	if(!array) return;
	if(array->data) array_free(array->data);
	array_free(array);
}

/** @brief Copies an array into another.
 * The source array must be initialised, and
 * the destination array must *not* be already initialised.
 *	@param dest the resulting copy.
 *  @param src original array to copy.
 *  @returns `dest` if successful, and NULL otherwise.
*/
array_t* array_copy(array_t* dest, array_t* src){
	if(!dest || !src) return NULL;
	
	int success = array_init(dest, src->element_size);
	if(!success) return NULL;
	if(src->size == 0) return dest;

	if(!array_resize(dest, src->size)){
		array_uninit(dest);
		return NULL;
	}

	memcpy(dest->data, src->data, dest->size * dest->element_size);
	return dest;
}

array_t* array_new_copy(array_t* orig){
	if(!orig) return NULL;

	array_t* new = array_create(orig->element_size);
	if(!new) return NULL;
	if(orig->size == 0) return new;

	if(!array_resize(new, orig->size)){
		array_destroy(new);
		return NULL;
	}

	memcpy(new->data, orig->data, new->size * new->element_size);
	return new;
}

/* Pre-allocates a given number of elements but does not initialise them */
array_t* array_resize(array_t* array, uint32_t size){
	if(!array || array->element_size == 0) return NULL;

	if(size <= array->size){
		// Shrinking
		// No need to delete anything. Old data will eventually be overwritten.
		array->size = size;
		array->end = array_end(array);
		array->begin = array_front(array);
		return array;
	}
	
	// Round up new capacity to the highest power of two closest to the size
	uint32_t capacity = nearest_pow2(size);
	if(capacity > array->capacity){
		void* data = array_realloc(array->data, capacity * array->element_size);
		if(!data) return NULL;
		array->capacity = capacity;
		array->data = data;
	}

	array->size = size;
	array->begin = array->data;
	array->end = array_end(array);
	return array;
}

// -- SETTERS
/* Overwrites an element at the given index with the given data */
void* array_set(array_t* array, void* element, uint32_t index){
	if(!array || array->element_size == 0 || index >= array->size) return NULL;
	char* addr = (char*)array->data + index * array->element_size;
	if(!element){
		memset(addr, 0, array->element_size);
	} else {
		memmove(addr, element, array->element_size);
	}
	return addr;
}

// -- RETRIEVALS
/* Returns a pointer to the element at the specified index */
void* array_get(array_t* array, uint32_t index){
	if(!array || array->element_size == 0 || index >= array->size) return NULL;
	char* addr = (char*)array->data + index * array->element_size;
	return addr;
}

/* Returns a pointer to the first element */
void* array_front(array_t* array){
	if(!array || array->size == 0) return NULL;
	return array->data;
}

/* Returns a pointer to the last element */
void* array_back(array_t* array){
	if(!array || array->size == 0) return NULL;
	return array_get(array, array->size-1);
}

/* Returns a pointer to first byte after the end of the array */
void* array_end(array_t* array){
	if(!array || !array->data || array->element_size == 0 || array->size == 0){
		return NULL;
	}
	return (char*)array_back(array) + array->element_size;
}

// -- INSERTING
/* Inserts an element at the given index */
void* array_insert(array_t* array, void* element, uint32_t index){
	if(!array || array->element_size == 0 || index > array->size){
		return NULL;
	}

	if(array->size >= array->capacity || !array->data){
		array_t* r = extend_capacity(array);
		if(!r) return NULL;
	}
	
	char* addr = (char*)array->data + index * array->element_size;
	uint32_t move_bytes = (array->size - index) * array->element_size;
	
	if(move_bytes > 0){
		// displace elements to make space for new one
		// this operation is invalid if you want to insert at the end of the array
		memmove(addr + array->element_size, addr, move_bytes);
	}

	// setting value
	if(!element){
		memset(addr, 0, array->element_size);
	} else {
		memmove(addr, element, array->element_size);
	}
	array->size++;
	array->begin = array->data;
	array->end = array_end(array);
	return addr;
}

/* Inserts the element to the end of the array */
void* array_push_back(array_t* array, void* element){
	return array_insert(array, element, array->size);
}

/* Inserts the element to the beginning of the array */
void* array_push_front(array_t* array, void* element){
	return array_insert(array, element, 0);
}

// -- DELETING
/* Removes the element at the given index */
array_t* array_remove(array_t* array, uint32_t index){
	if(!array || !array->data || index >= array->size){
		return NULL;
	}

	if (index == array->size - 1){
		// pop back, no need to shuffle data around
		array->size--;
		return array;
	}
	
	char* dest = (char*)array->data + index * array->element_size;
	char* orig = dest + array->element_size;
	uint32_t move_bytes = (array->size - index) * array->element_size;

	memmove(dest, orig, move_bytes);
	array->size--;
	array->begin = array->data;
	array->end = array_end(array);

	return array;
}

/* Removes the element last element of the array */
array_t* array_pop_back(array_t* array){
	if(array->size == 0) return NULL;
	return array_remove(array, array->size-1);
}

/* Removes the element first element of the array */
array_t* array_pop_front(array_t* array){
	return array_remove(array, 0);
}

/* Removes all elements on the array */
array_t* array_clear(array_t* array){
	if(!array) return NULL;
	array->size = 0;
	array->begin = NULL;
	array->end   = NULL;
	return array;
}

