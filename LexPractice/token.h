/* C++ header for token*/

#include <string>
#include "globals.h"
using namespace std;

class Token
{
    TokenType id;
    string val;
public:
    Token ();
    Token (TokenType, string);
    Token (tokenStruct *);
    ~Token ();
    void setId(TokenType);
    TokenType getId();
    void setVal(string);
    string getVal();
};

