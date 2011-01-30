#include "ListMember.h"

ListMember::ListMember (T * t)
{
    next = NULL;
    prev = NULL;
    val = t;
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
void ListMember::setVal (T * t)
{
    val = t; 
}
T * ListMember::getVal ()
{
    return val;
}