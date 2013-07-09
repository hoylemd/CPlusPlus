#include <string>
#include <cstdio>
#include "hashmap.h"

using namespace std;

class Cache {
	private:
		FILE * source;
		int max_size;
		int current_size;
		Hashmap * records;
	public:
		Cache(string data_file, int cache_size);
		~Cache();
		string get(string key);
};
