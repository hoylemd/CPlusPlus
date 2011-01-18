#include <iostream>
#include <string>
using namespace std;

int main(int argc, char** argv)
{
    int i = 0;
    
    for(i=0; i< argc; i++) cout << argv[i] << "\n";
    
    return 0;
}