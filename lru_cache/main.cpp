#include <iostream>
#include <sstream>
#include "debug.h"
#include "hashmap.h"

using namespace std;

int test_hash (int argc, char ** argv) {
	int arg = 1, i = 0, hash_size = 0, hash = 0;
	string key;
	istringstream stringstream;
	if (argc > 2) {
		// get the hash size
		stringstream.str(argv[1]);
		if (!(stringstream >> hash_size)) {
			cerr << "Invalid hash size '" << argv[1] << "'\n";
		}

		// initialize the key buffer
		for (i = 0; i < MAX_KEY_LENGTH; i += 1) {
			key[i] = 0;
		}

		// loop over the rest of the keys
		for (arg = 2; arg < argc; arg += 1) {
			key = string(argv[arg]);
			if (key.length() < MAX_KEY_LENGTH) {
				hash = djb2(key, hash_size);

				cout << hash << "\n";
			} else {
				cerr <<  "Error: key '" << key <<
					"' too long. Must be shoter than MAX_KEY_LENGTH." <<
					"Skipping.\n";
			}
		}
	} else {
		cerr << "Invalid input: " <<
			"Require at least a hash size and one key to hash\n" <<
			"  e.g. djb2 32 key_one\n";
	}

	return 0;

}

int test_hashmap(int argc, char ** argv) {
	Hashmap map(32);
	cout << "made successfully\n";
}

int main (int argc, char ** argv) {
	test_hash(argc, argv);
	test_hashmap(argc, argv);
}
