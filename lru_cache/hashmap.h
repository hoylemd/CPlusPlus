#include <cstdlib>
#include "djb2.h"

#define byte unsigned char

using namespace std;

class Hashmap {
	private:
		int size;
		void ** entries;
		int hash(string key);
	public :
		Hashmap(int size);
		~Hashmap();
		void set(string key, void * value);
		void * get(string key);
};
