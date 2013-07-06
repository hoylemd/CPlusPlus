#include <stdio.h>
#include <string.h>

#define MAX_KEY_LENGTH 100

int debug (char * message) {
	fprintf(stderr, "%s\n", message);
}
int debugInt (int message) {
	fprintf(stderr, "%i\n", message);
}

int murmur (char * key, int length, int hash_size) {
	/*
	Murmur3_32(key, len, seed)
    // Note: In this version, all integer arithmetic is performed with unsigned 32 bit integers.
    //       In the case of overflow, the result is constrained by the application of modulo  arithmetic.

    c1  0xcc9e2d51
    c2  0x1b873593
    r1  15
    r2  13
    m  5
    n  0xe6546b64

    hash  seed

    for each fourByteChunk of key
        k  fourByteChunk

        k  k * c1
        k  (k << r1) OR (k >> (32-r1))
        k  k * c2

        hash  hash XOR k
        hash  (hash << r2) OR (hash >> (32-r2))
        hash  hash * m + n

    with any remainingBytesInKey
        remainingBytes  SwapEndianOrderOf(remainingBytesInKey)
        // Note: Endian swapping is only necessary on big-endian machines.
        //       The purpose is to place the meaningful digits towards the low end of the value,
        //       so that these digits have the greatest potential to affect the low range digits
        //       in the subsequent multiplication.  Consider that locating the meaningful digits
        //       in the high range would produce a greater effect upon the high digits of the
        //       multiplication, and notably, that such high digits are likely to be discarded
        //       by the modulo arithmetic under overflow.  We don't want that.

        remainingBytes  remainingBytes * c1
        remainingBytes  (remainingBytes << r1) OR (remainingBytes >> (32 - r1))
        remainingBytes  remainingBytes * c2

        hash  hash XOR remainingBytes

    hash  hash XOR len

    hash  hash XOR (hash >> 16)
    hash  hash * 0x85ebca6b
    hash  hash XOR (hash >> 13)
    hash  hash * 0xc2b2ae35
    hash  hash XOR (hash >> 16)
	*/
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

				hash = murmur(key, key_length, hash_size);

				printf("%i\n", hash);
			} else {
				fprintf(stderr, "error: key '%s' too long. Must be shorter than MAX_KEY_LENGTH. Skipping.\n", argv[arg]);
			}
		}
	} else {
		printf("Invalid input. Require at least a hash size and one key to hash\n  e.g. murmur 32 key_one\n");
	}

	return 0;
}

