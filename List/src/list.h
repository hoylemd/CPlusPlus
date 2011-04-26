/* list.h */

#ifndef LIST_H
#define LIST_H

#include "listItem.h"

using namespace std;

class List
{
	ListItem * head;
	Listitem * tail;
	int numElements;
public:
	/* constructors/destructors */
	List ();
	~List ();
	
	/* accessors */
	ListItem * getHead ();
	ListItem * getTail ();
	int count ();
	ListItem * getAt (int);
	
	/* inserters */
	void append (ListItem *);
	void insertAt (Listitem *, int);
}

#endif