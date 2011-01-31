#include <iostream>
#include "stack.h"

using namespace std;

int main(void)
{
    int i = 0;
    Stack * leStack = new Stack();
    ListMember * lm = new ListMember("one");
    
    cout << "Initial setup complete\n";
    
    leStack->Push(lm);
    cout << "Pushed \"one\"\n";
    lm = new ListMember("two");
    leStack->Push(lm);
    cout << "Pushed \"two\"\n";
    lm = new ListMember("three");
    leStack->Push(lm);
    cout << "Pushed \"three\"\n";
    
    lm = NULL;
    cout << "lm nullified\n";
    for(i= 0; i < 3; i++)
    {
	lm = leStack->Pop();
	cout << lm->getVal() << "\n""";
	delete lm;
    }

    delete leStack;

    return 0;
}