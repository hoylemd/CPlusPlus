#include "hashmap.h"

/*** HashEntry class stuff ***/
// constructor
HashEntry::HashEntry(string iKey, void * iValue) {
	value = iValue;
	key = iKey;
	next = NULL;
}

// destructor
HashEntry::~HashEntry() {
	if (next) {
		delete next;
	}
}

// getter
void * HashEntry::get(string iKey) {
	// grab it if this is the right entry for the key
	if (iKey == key) {
		return value;
	} else {
		// pass to the next entry, if there is one
		if (next) {
			return next->get(iKey);
		} else {
			return NULL;
		}
	}
}

// appender
void HashEntry::append(HashEntry * entry) {
	if (next) {
		next->append(entry);
	} else {
		next = entry;
	}
}

/*** Hashmap class stuff ***/
// constructor
Hashmap::Hashmap(int iSize) {
	size = iSize;
	entries = (HashEntry**)calloc(size, sizeof(HashEntry *));
}

// destructor
Hashmap::~Hashmap() {
	int i = 0;
	for (i = 0; i < size; i += 1) {
		delete entries[i];
	}
	if (entries != NULL) {
		free(entries);
	}
}

// size function
int Hashmap::getSize() {
	return size;
}

// hashing function
int Hashmap::hash(string key) {
	return djb2(key, size);
}

// setter
void Hashmap::set(string key, void * value) {
	int index = hash(key);
	HashEntry * entry = new HashEntry(key, value);
	if (entries[index]) {
		entries[index]->append(entry);
	} else {
		entries[index] = entry;
	}
}

// getter
void * Hashmap::get(string key) {
	int index = hash(key);

	if (entries[index]) {
		return entries[index]->get(key);
	} else {
		return NULL;
	}
}

// remover
void Hashmap::remove(string key) {
	int index = hash(key);
	HashEntry * result = NULL;

	if (entries[index]) {
		result = entries[index]->remove(key);
		if (result) {
			entries[index] = result;
		} else {
			delete (entries[index]);
			entries[index] = NULL;
		}
	}
}
