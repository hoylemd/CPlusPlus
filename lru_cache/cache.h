#include <string>
#include <cstdio>
#include <iostream>
#ifndef HASHMAP_H
	#include "hashmap.h"
#endif

#define INPUT_BUFFER 128
#define DEBUG 1

using namespace std;

class Cache {
	private:
		string source;
		int max_size;
		int current_size;
		Hashmap * records;
		string * getFromCache(string key);
		string * missGet(string key);
		void drop(string key);
	public:
		Cache(string data_file, int cache_size);
		~Cache();
		string * get(string key);
		string toString();
		void pubDrop(string key);
};
