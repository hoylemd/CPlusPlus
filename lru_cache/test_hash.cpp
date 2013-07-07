#include <iostream>
#include <string>
#include "djb2.h"

int main(int argc, char ** argv) {
	int hash_size = 0, i = 0;
	string * key = NULL;
	if (argc > 3) {
		hash_size = atoi(argv[1]);

		for (i = 2; i < argc; i += 1){
			key = new string(argv[i]);
			cout << djb2(*key, hash_size) << "\n";
			delete key;
		}
	}
}
