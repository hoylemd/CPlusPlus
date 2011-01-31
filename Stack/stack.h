#include <string>
using namespace std;

class ListMember
{
    string val;
    ListMember * next;
    ListMember * prev;
public:
    ListMember (string);
    ListMember ();
    ~ListMember ();
    void Append (ListMember *);
    ListMember * getNext ();
    void Attach (ListMember *);
    ListMember * getPrev ();
    void setVal (string);
    string getVal ();
};

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