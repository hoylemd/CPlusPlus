#include <iostream>
#include "Terminal.h"
using namespace std;

int main(void)
{
    Terminal * t = new Terminal("Blarg!");
    cout << "new Terminal: "<<t->getValue()<<"\n";
    t->setValue("wort!");
    cout << "Modified Terminal: "<<t->getValue()<<"\n";
    delete t;
    cout << "Terminal deleted successfully\n";
    return 0;
    
}