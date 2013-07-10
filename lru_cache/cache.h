#include <string>
#include <cstdio>
#ifndef HASHMAP_H
#include "hashmap.h"
#endif

#define INPUT_BUFFER 128

using namespace std;

class Cache {
	private:
		string source;
		int max_size;
		int current_size;
		Hashmap * records;
		string * missGet(string key);
	public:
		Cache(string data_file, int cache_size);
		~Cache();
		string * get(string key);
		string toString();
};
