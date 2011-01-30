/* C++ Implementation of a stack data structure
   currently only holds strings, but will eventualy hold templates */

#include "stack.h"
#include "ListMember.h"
#include <iostream>
using namespace std;

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