#include <iostream>
#include <sstream>
#include "debug.h"
#include "hashmap.h"
#include "cache.h"

#define HASH djb2

using namespace std;

bool test_hash (int argc, char ** argv) {
	int hash_size = 15;
	bool passed = true;

	if (HASH("hot_dog", hash_size) == 13) {
		cout << "hash('hot_dog') => 13. correct\n";
	} else {
		cerr << "hash('hot_dog') => " << HASH("hot_dog", hash_size) << ". INCORRECT\n";
		passed = false;
	}
	if (HASH("salt_pork", hash_size) == 14) {
		cout << "hash('salt_pork') => 14. correct\n";
	} else {
		cerr << "hash('salt_pork') => " << HASH("salt_pork", hash_size) << ". INCORRECT\n";
		passed = false;
	}
	if (HASH("bacon", hash_size) == 14) {
		cout << "hash('bacon') => 14. correct\n";
	} else {
		cerr << "hash('bacon') => " << HASH("bacon", hash_size) << ". INCORRECT\n";
		passed = false;
	}	if (HASH("pizza", hash_size) == 1) {
		cout << "hash('pizza') => 1. correct\n";
	} else {
		cerr << "hash('pizza') => " << HASH("pizza", hash_size) << ". INCORRECT\n";
		passed = false;
	}

	return passed;

}

bool test_hashmap(int argc, char ** argv) {
	Hashmap * map;
	bool passed = true;

	// initialize test data
	int potato = 5;
	int * carrot = &potato;
	string * salt_pork = new string("popeye");
	string * bacon = new string("porky");
	string * temp = NULL;
	void * reciever = NULL;

	// test instantiation
	map = new Hashmap(15);
	if (map->getSize() == 15) {
		cout << "made hashmap object successfully. correct\n";
	} else {
		cerr << "hashmap created with wrong size (" << map->getSize() << "). INCORRECT\n";
		passed = false;
	}

	// set some things
	map->set("potato", &potato);
	cout << "set 'potato' to *(5)\n";
	map->set("salt_pork", salt_pork);
	cout << "set 'salt_pork' to *('popeye')\n";
	map->set("bacon", bacon);
	cout << "set 'bacon' to *('porky')\n";
	map->set("carrot", carrot);
	cout << "set 'carrot' to *(5)\n";

	// attempt to retrieve them
	reciever = map->get("potato");
	if (reciever) {
		potato = *(int*) reciever;
		if (potato == 5) {
			cout << "'potato' retrieved, value is 5. correct.\n";
		} else {
			cerr << "'potato' retrieved, but value was not 5 (" << potato << "). INCORRECT\n";
			passed = false;
		}
	} else {
		cerr << "nothing retrieved from 'potato'. INCORRECT\n";
		passed = false;
	}
	reciever = map->get("salt_pork");
	if (reciever) {
		temp = (string *) reciever;
		if (temp == salt_pork) {
			cout << "'salt_pork' retrieved, value is 'popeye'. correct.\n";
		} else {
			cerr << "'salt_pork' retrieved, but value was not popeye (" << *temp << "). INCORRECT\n";
			passed = false;
		}
	} else {
		cerr << "nothing retrieved from 'salt_pork'. INCORRECT\n";
	}
	reciever = map->get("bacon");
	if (reciever) {
		temp = (string *) reciever;
		if (temp == bacon) {
			cout << "'bacon' retrieved, value is 'porky'. correct.\n";
		} else {
			cerr << "'bacon' retrieved, but value was not porky (" << *temp << "). INCORRECT\n";
			passed = false;
		}
	} else {
		cerr << "nothing retrieved from 'bacon'. INCORRECT\n";
		passed = false;
	}
	reciever = map->get("carrot");
	if (reciever) {
		carrot = (int*) reciever;
		if (*carrot == 5) {
			cout << "'carrot' retrieved, value is 5. correct.\n";
		} else {
			cerr << "'carrot' retrieved, but value was not 5 (" << *carrot << "). INCORRECT\n";
			passed = false;
		}
	} else {
		cerr << "nothing retrieved from 'carrot'. INCORRECT\n";
		passed = false;
	}

	// attempt to get an unset key
	reciever = map->get("nothing");
	if (reciever) {
		temp = (string *) reciever;
		cerr << "something retrieved from 'nothing' (" << *temp << ". INCORRECT\n";
		passed = false;
	} else {
		cout << "NULL retrieved from 'nothing'. correct.\n";
	}

	cout << map->toString();

	// test the remove function
	map->remove("salt_pork");
	if (map->get("salt_pork")) {
		cerr << "something returned from removed element 'salt_pork'. INCORRECT\n";
		passed = false;
	} else {
		cout << "'salt_pork' removed successfully. correct.\n";
	}
	map->remove("carrot");
	if (map->get("carrot")) {
		cerr << "something returned from removed element 'carrot'. INCORRECT\n";
		passed = false;
	} else {
		cout << "'carrot' removed successfully. correct.\n";
	}

	cout << map->toString();

	// clean up
	delete salt_pork;
	delete bacon;
	delete map;

	return passed;
}

bool test_cache(int argc, char ** argv) {
	string * data_file = new string("food_data.txt");
	Cache * cache = new Cache(*data_file, 256);

	cout << "cache created successfully";

	delete data_file;
}

int main (int argc, char ** argv) {
	bool passed = test_hash(argc, argv);
	if (passed) {
		bool passed = test_hashmap(argc, argv);
	}
	if (passed) {
		bool passed = test_cache(argc, argv);
	}
}

