/* C++ header for stack data structure */

#include "token.h"
using namespace std;

class ListMember
{
    Token * val;
    ListMember * next;
    ListMember * prev;
public:
    ListMember (Token *);
    ListMember ();
    ~ListMember ();
    void Append (ListMember *);
    ListMember * getNext ();
    void Attach (ListMember *);
    ListMember * getPrev ();
    void setVal (Token *);
    Token * getVal ();
};

class Stack
{
    ListMember * head;
    int numElements;
public:
    Stack ();
    ~Stack ();
    void Push (ListMember *);
    void MakePush (Token *);
    ListMember * Pop ();
    Token * BreakPop ();
    int getCount();
    Token * getTopVal();
};