#include "ListMember.h"
using namespace std;

class Stack
{
    ListMember * head;
    int numElements;
public:
    Stack ();
    ~Stack ();
    void Push (ListMember *);
    ListMember * Pop ();
};