#include "djb2.h"

int djb2 (string key, int hash_size) {
	unsigned long hash = 5381;
	int c = 1;
	int i = 0;
	int length = key.length();

	for (i = 0; i < length && c; i += 1) {
		c = (int)key[i];
		if (c) {
			hash = ((hash << 5) + hash) + key[i];
		}
	}

	return hash % hash_size;
}
