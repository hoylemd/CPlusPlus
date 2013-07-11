#include <cstdlib>
#include <sstream>
#include "djb2.h"

#define byte unsigned char
#define HASHMAP_H TRUE

using namespace std;

class HashEntry {
	private :
		string key;
		void * value;
	public :
		HashEntry(string iKey, void * iValue);
		~HashEntry();
		HashEntry * next;
		void * get (string iKey);
		HashEntry * remove(string iKey);
		void append(HashEntry * entry);
		string toString();
};

class Hashmap {
	private:
		int size;
		HashEntry ** entries;
		int hash(string key);
	public :
		Hashmap(int iSize);
		~Hashmap();
		int getSize();
		void set(string key, void * value);
		void remove(string key);
		void * get(string key);
		string toString();
		void * pop(string key);
};
