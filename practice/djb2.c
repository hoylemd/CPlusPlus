#include <stdio.h>
#include <string.h>

#define MAX_KEY_LENGTH 100

int debug (char * message) {
	fprintf(stderr, "%s\n", message);
}
int debugInt (int message) {
	fprintf(stderr, "%i\n", message);
}

int djb2 (char * key, int length, int hash_size) {
	unsigned long hash = 5381;
	int c = 1;
	int i = 0;

	for (i = 0; i < length && c; i += 1) {
		c = (int)key[i];
		if (c) {
			hash = ((hash << 5) + hash) + key[i];
		}
	}

	return hash % hash_size;
}

int main (int argc, char ** argv) {
	int arg = 1, i = 0, hash_size = 0, hash = 0, key_length;
	char key[MAX_KEY_LENGTH];
	if (argc > 2) {
		// get the hash size
		sscanf(argv[1], "%i", &hash_size);

		// initialize the key buffer
		for (i = 0; i < MAX_KEY_LENGTH; i += 1) {
			key[i] = 0;
		}

		// loop over the rest of the keys
		for (arg = 2; arg < argc; arg += 1) {
			key_length = strlen(argv[arg]);
			if (key_length < MAX_KEY_LENGTH) {
				sscanf(argv[arg], "%s", key);

				hash = djb2(key, key_length, hash_size);

				printf("%i\n", hash);
			} else {
				fprintf(stderr, "error: key '%s' too long. Must me shoter than MAX_KEY_LENGTH. Skipping.\n", argv[arg]);
			}
		}
	} else {
		printf("Invalid input. Require at least a hash size and one key to hash\n  e.g. djb2 32 key_one\n");
	}

	return 0;
}

