#include <cstdlib>
#include "djb2.h"

#define byte unsigned char

using namespace std;

class HashEntry {
	private :
		string key;
		void * value;
		HashEntry * next;
	public :
		HashEntry(string key, void * value);
		~HashEntry();
		void * get (string key);
		HashEntry * remove(string key);
		void append(HashEntry * entry);
};

class Hashmap {
	private:
		int size;
		HashEntry ** entries;
		int hash(string key);
	public :
		Hashmap(int size);
		~Hashmap();
		int getSize();
		void set(string key, void * value);
		void remove(string key);
		void * get(string key);
};
