template <class T>
class ListMember
{
    T * val;
    ListMember * next;
    ListMember * prev;
public:
    ListMember (T *);
    ListMember ();
    ~ListMember ();
    void setNext (ListMember *);
    ListMember * getNext ();
    void setPrev (ListMember *);
    ListMember * getPrev ();
    void setVal ( T*);
    T* getVal ();
};