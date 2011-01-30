#include "Terminal.h"

Terminal::Terminal(string s)
{
    value = s;
}
Terminal::Terminal()
{
    value = "";
}
Terminal::~Terminal()
{
}
string Terminal::getValue()
{
    return value;
}
void Terminal::setValue( string s)
{
    value = s;
}