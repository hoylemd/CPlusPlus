#include "cache.h"

// constructor
Cache::Cache(string data_file, int cache_size) {
	max_size = cache_size;
	source = data_file;
	records = new Hashmap(128);
}

// destructor
Cache::~Cache() {
	// this should drop all records first
	delete records;
}

// getter
string * Cache::get(string key) {
	string * result;

	// cache hit
	if (result = (string *)records->get(key)) {
		if (DEBUG) {
			cout << "cache hit (" << key << ")\n";
		}
		return result;
	// cache miss
	} else {
		if (DEBUG) {
			cout << "cache miss (" << key << ")\n";
		}
		return missGet(key);
	}
}

// miss-getter
string * Cache::missGet(string key) {
	FILE * file = fopen(source.c_str(), "r");
	char line[INPUT_BUFFER];
	string * line_string = NULL, read_key, * value = NULL;
	int space_index = 0;
	bool found_key = false;

	// read the source file until the key is found
	while (fgets(line, INPUT_BUFFER, file) != NULL && !found_key) {
		// read this line's key
		line_string = new string(line);
		space_index = line_string->find_first_of(' ');
		read_key = line_string->substr(0, space_index);

		// check the read key
		if (!key.compare(read_key)) {
			value = new string((line_string->substr(space_index + 1)));
			found_key = true;
		}

		// clean up from the read
		delete line_string;
	}
	fclose(file);

	// add it to the cache
	if (found_key) {
		records->set(key, (void *)value);
	}

	// return it
	return value;
}

// cache-only getter
string * Cache::getFromCache(string key) {
	string * result;

	// try the key
	if (result = (string *)records->get(key)) {
		if (DEBUG) {
			cout << "key '" << key << "' is in cache. (" << result << "\n";
		}
		return result;
	} else {
		if (DEBUG) {
			cout << "key '" << key << "' is not in cache.\n";
		}
		return NULL;
	}
}

// dropper
void Cache::drop(string key) {
	string * value = NULL;
	if (value = (string *)records->pop(key)) {
		delete value;
		if (DEBUG) {
			cout << "key '" << key << "' dropped.\n";
		}
	} else {
		if (DEBUG) {
			cout << "key '" << key << "' is not in the cache. cannot drop.\n";
		}
	}
}

void Cache:: pubDrop(string key) {
	drop(key);
}

// stringer
string Cache::toString() {
	return records->toString();
}
