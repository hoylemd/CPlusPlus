#include "cache.h"

Cache::Cache(string data_file, int cache_size) {
	max_size = cache_size;
	source = fopen(data_file.c_str(), "w");
}
