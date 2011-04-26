/* list.cpp */

#include "list.h"

/* code for List members */

/* constructors/destructor */
/* empty constructor */
List::List()
{
	head = NULL;
	tail = NULL;
}
/* destructor */
List::~List()
{
	head = NULL;
	tail = NULL;
}

/* accessors */
ListItem * List::getHead()
{
	return head;
}
ListItem * List::getTail()
{
	return tail;
}
int List::count()
{
	return numElements;
}
ListItem * List::getAt(int index)
{
	ListItem * temp = NULL;
	int i = 0;
	
	/* validate argument */
	if (index < numElements)
	{
		if (index >= 0)
		{
			/* traverse the list to the precribed index */
			temp =  head;
			for(i = 0; i < index; i++)
				temp = temp->next;
		}
		else
			cerr << "List->getAt Error: index is negative.\n");
	}
	else
		cerr << "List->getAt Error: index greater than number of elements.\n";

	/* return temp ( will be NULL on error ) */
	return temp;	
}

/* inserters */
void List::append (Listitem * li)
{
	/* validate argument */
	if (li)
	{
		/* standard case */
		if (tail)
		{
			tail->next = li;
			li->prev = tail;
			tail = li;
			numElements++;
		}
		/* empty case */
		else
		{
			head = li;
			tail = li;
			numElements = 1;
		}
	}
	else
		cerr << "List->append Error: null list item provided.\n"
}