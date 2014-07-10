#include "debug.h"

using namespace std;

int debug(string message) {
	cerr << message + "\n";
	return 0;
}

int debug(int message) {
	stringstream sstm;
	sstm << message;
	debug(sstm.str());
	return 0;
}
