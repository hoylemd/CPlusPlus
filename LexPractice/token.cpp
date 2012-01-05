/* C++ implementation of a token */

#include "token.h"
#include <stdlib.h>
using namespace std;

Token::Token()
{
    id = ENDFILE;
    val = "";
}
Token::Token(TokenType i, string v)
{
    id = i;
    val = v;
}
Token::Token(tokenStruct * str )
{
    id = str->id;
    val = str->val;
    free(str);
}
Token::~Token()
{}
void Token::setId(TokenType i)
{
    id = i;
}
TokenType Token::getId()
{
    return id;
}
void Token::setVal(string v)
{
    val = v;
}
string Token::getVal()
{
    return val;
}