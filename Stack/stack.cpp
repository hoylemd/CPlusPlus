/* C++ Implementation of a stack data structure
   currently only holds strings, but will eventualy hold templates */

#include "stack.h"
#include <iostream>
using namespace std;


ListMember::ListMember (string s)
{
    next = NULL;
    prev = NULL;
    val = s;
}
ListMember::ListMember ()
{
    next = NULL;
    prev = NULL;
    val = "";
}
ListMember::~ListMember ()
{
    next = NULL;
    prev = NULL;
}
void ListMember::Append (ListMember* l)
{
    next = l;
}
ListMember * ListMember::getNext ()
{
    return next;
}
void ListMember::Attach (ListMember* l)
{
    prev = l;
}
ListMember * ListMember::getPrev ()
{
    return prev;
}
void ListMember::setVal (string s)
{
    val = s; 
}
string ListMember::getVal ()
{
    return val;
}

Stack::Stack ()
{
    head = NULL;
    numElements = 0;
}
Stack::~Stack ()
{
    head = NULL;
}
void Stack::Push (ListMember* l)
{
    if (head == NULL)
    {
	head = l;
    }
    else
    {
	l->Append(head);
	head->Attach(l);
	head = l;
    }
    numElements++;
}
ListMember * Stack::Pop ()
{
    ListMember * curr = head;
    head = curr->getNext();
    curr->Append(NULL);
    if (head)
	head->Attach(NULL);
    numElements--;
    return curr;
}