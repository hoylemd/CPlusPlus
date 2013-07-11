#include "cache.h"

Cache::Cache(string data_file, int cache_size) {
	max_size = cache_size;
	source = data_file;
	records = new Hashmap(128);
}

Cache::~Cache() {
	delete records;
}

string * Cache::get(string key) {
	string * result;

	// cache hit
	if (result = (string *)records->get(key)) {
		cout << "cache hit (" << key << ")\n";
		return result;
	} else {
		cout << "cache miss (" << key << ")\n";
		return missGet(key);
	}
}

string * Cache::missGet(string key) {
	FILE * file = fopen(source.c_str(), "r");
	char line[INPUT_BUFFER];
	string * line_string = NULL, read_key, * value = NULL;
	int space_index = 0;
	bool found_key = false;

	// get the data from the source file
	while (fgets(line, INPUT_BUFFER, file) != NULL && !found_key) {
		line_string = new string(line);

		space_index = line_string->find_first_of(' ');
		read_key = line_string->substr(0, space_index);
		if (!key.compare(read_key)) {
			value = new string((line_string->substr(space_index + 1)));
			found_key = true;
		}

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

string Cache::toString() {
	return records->toString();
}
