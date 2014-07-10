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

// remover
HashEntry * HashEntry::remove(string iKey) {
	HashEntry * temp_next = NULL;
	if (iKey == key) {
		temp_next = next;
		next = NULL;
		return temp_next;
	} else {
		if (next) {
			temp_next = next->remove(iKey);
			if (temp_next != next) {
				delete(next);
				next = temp_next;
			}
		}
		return this;
	}
}
// stringer
string HashEntry::toString() {
	stringstream ss;
	string out = "";

	ss << "'" << key << "' - {" << value << "}";

	out = ss.str();
	return out;
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

// total destructor. clears out the entries as well
bool Hashmap::destruct() {

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
		if (result != entries[index]) {
			delete (entries[index]);
			entries[index] = result;
		}
	}
}

// stringer
string Hashmap::toString() {
	int i = 0;
	stringstream ss;
	HashEntry * entry;
	for (i = 0; i < size; i += 1) {
		if (entries[i]) {
			for (entry = entries[i]; entry; entry = entry->next) {
				ss << "[" << i << "] - " << entry->toString() << "\n";
			}
		}
	}

	return ss.str();
}

// popper
void * Hashmap::pop(string key) {
	void * value = NULL;
	if (value = get(key)) {
		remove(key);
	}

	return value;
};
