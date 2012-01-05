/* C++ Implementation of a stack data structure
   currently only holds Tokens, but will eventualy hold templates */
#include <iostream>
#include "stack.h"
using namespace std;


ListMember::ListMember (Token * s)
{
    next = NULL;
    prev = NULL;
    val = s;
}
ListMember::ListMember ()
{
    next = NULL;
    prev = NULL;
    val = NULL;
}
ListMember::~ListMember ()
{
    next = NULL;
    prev = NULL;
    val = NULL;
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
void ListMember::setVal (Token * s)
{
    val = s; 
}
Token * ListMember::getVal ()
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
void Stack::MakePush (Token * t)
{
    ListMember * l = new ListMember(t);
    Push(l);
}
ListMember * Stack::Pop ()
{
    ListMember * curr = NULL;
    if (numElements > 0)
    {
	curr = head;
	head = curr->getNext();
	curr->Append(NULL);
	if (head)
	    head->Attach(NULL);
	numElements--;
    }
    return curr;
}
Token * Stack::BreakPop ()
{
    ListMember * l = Pop();
    Token * t = NULL;
    if (l)
    {
	Token * t = l->getVal();
	l->setVal(NULL);
	delete l;
    } 
    return t;
}
int Stack::getCount()
{
    return numElements;
}
Token * Stack::getTopVal()
{
    return head->getVal();
}