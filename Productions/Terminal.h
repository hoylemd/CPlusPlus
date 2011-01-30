#include <string>
using namespace std;

class Terminal
{
  string value;
public:
  Terminal(string);
  Terminal();
  ~Terminal();
  string getValue();
  void setValue(string s);
};