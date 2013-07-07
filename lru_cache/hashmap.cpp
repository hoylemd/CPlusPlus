#include "hashmap.h"

// constructor
Hashmap::Hashmap(int size) {
	size = size;
	entries = (void**)calloc(size, sizeof(void *));
}

// destructor
Hashmap::~Hashmap() {
	if (entries != NULL) {
		free(entries);
	}
}

// hashing function
int Hashmap::hash(string key) {
	return djb2(key, size);
}

// setter
void Hashmap::set(string key, void * value) {

}

// getter
void * Hashmap::get(string key) {
	return NULL;
}
