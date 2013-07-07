#include <iostream>
#include <sstream>
#include "debug.h"
#include "hashmap.h"

#define HASH djb2

using namespace std;

int test_hash (int argc, char ** argv) {
	int hash_size = 15;

	if (HASH("hot_dog", hash_size) == 13) {
		cout << "hash('hot_dog') => 13. correct\n";
	} else {
		cerr << "hash('hot_dog') => " << HASH("hot_dog", hash_size) << ". INCORRECT\n";
	}
	if (HASH("salt_pork", hash_size) == 14) {
		cout << "hash('salt_pork') => 14. correct\n";
	} else {
		cerr << "hash('salt_pork') => " << HASH("salt_pork", hash_size) << ". INCORRECT\n";
	}
	if (HASH("bacon", hash_size) == 14) {
		cout << "hash('bacon') => 14. correct\n";
	} else {
		cerr << "hash('bacon') => " << HASH("bacon", hash_size) << ". INCORRECT\n";
	}	if (HASH("pizza", hash_size) == 1) {
		cout << "hash('pizza') => 1. correct\n";
	} else {
		cerr << "hash('pizza') => " << HASH("pizza", hash_size) << ". INCORRECT\n";
	}

	return 0;

}

int test_hashmap(int argc, char ** argv) {
	Hashmap * map;


	// initialize test data
	int potato = 5;
	int * carrot = &potato;
	string * salt_pork = new string("popeye");
	string * bacon = new string("porky");
	string * temp = NULL;
	void * reciever = NULL;

	// test instantiation
	map = new Hashmap(15);
	cout << "made hashmap object successfully\n";

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
		}
	} else {
		cerr << "nothing retrieved from 'potato'. INCORRECT\n";
	}
	reciever = map->get("salt_pork");
	if (reciever) {
		temp = (string *) reciever;
		if (temp == salt_pork) {
			cout << "'salt_pork' retrieved, value is 'popeye'. correct.\n";
		} else {
			cerr << "'salt_pork' retrieved, but value was not popeye (" << *temp << "). INCORRECT\n";
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
		}
	} else {
		cerr << "nothing retrieved from 'bacon'. INCORRECT\n";
	}
	reciever = map->get("carrot");
	if (reciever) {
		carrot = (int*) reciever;
		if (*carrot == 5) {
			cout << "'carrot' retrieved, value is 5. correct.\n";
		} else {
			cerr << "'carrot' retrieved, but value was not 5 (" << *carrot << "). INCORRECT\n";
		}
	} else {
		cerr << "nothing retrieved from 'carrot'. INCORRECT\n";
	}

	// attempt to get an unset key
	reciever = map->get("nothing");
	if (reciever) {
		temp = (string *) reciever;
		cerr << "something retrieved from 'nothing' (" << *temp << ". INCORRECT\n";
	} else {
		cout << "NULL retrieved from 'nothing'. correct.\n";
	}

	// clean up
	delete salt_pork;
	delete bacon;
	delete map;
}

int main (int argc, char ** argv) {
	test_hash(argc, argv);
	test_hashmap(argc, argv);
}

