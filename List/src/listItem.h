/* listItem.h */

#ifndef LISTITEM_H
#define LISTITEM_H

using namespace std

class ListItem
{
	void * member;	/* pointer to the actual item */
	ListItem * next;
	ListItem * prev;
	
public:
	/* constrcutors/destructors */
	ListItem (void *);
	ListItem ();
	~ListItem ();
	
	/* accessors */
	void updateMember (void *);
	void setNext (ListItem *);
	void setPrev (ListItem *);
	
	/* mutators */
	void * getMember();
	ListItem * getNext();
	ListItem * getPrev();
}

#endif