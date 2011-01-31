/* C++ header for token*/

#include <string>
using namespace std;

class Token
{
    string id;
    string val;
public:
    Token ();
    Token (string, string);
    ~Token ();
    void setId(string);
    string getId();
    void setVal(string);
    string getVal();
};

