/* C++ implementation of a token */

#include "token.h"
using namespace std;

Token::Token()
{
    id = "";
    val = "";
}
Token::Token(string i, string v)
{
    id = i;
    val = v;
}
Token::~Token()
{}
void Token::setId(string i)
{
    id = i;
}
string Token::getId()
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