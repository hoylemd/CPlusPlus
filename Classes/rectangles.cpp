#include <iostream>
using namespace std;

// Hey look! a class!
class CRectangle
{
    // intrinsic private access, friends.
        int x, y;
        double xd, yd;
    public:
        void set_values (int,int);
        void set_values (double, double);
        // constructor! woohoo!
        CRectangle ();
        CRectangle (int, int);
        CRectangle (double, double);
        // what's that? I can't hear you over the sound of my OVERLOAD OPERATORS.
        CRectangle operator + (CRectangle param)
        {
            CRectangle temp;
            temp.xd = xd + param.xd;
            temp.x = temp.xd;
            temp.yd = yd + param.yd;
            temp.y = temp.yd;
            return (temp);
        }
        int area (void)
        {
            return (x*y);
        }
        double area (bool precise)
        {
            if (precise)
                return (xd*yd);
            else 
                return (double)area();
        }
};

CRectangle::CRectangle (int a, int b)  // look! we can define methods outside the class declaration!
{
    x = a;
    xd = a;
    y = b;
    yd = b;
}
CRectangle::CRectangle (double a, double b) // overloading, dude.
{
    xd = a;
    x = xd;
    yd = b;
    y = yd;
}
CRectangle::CRectangle ()
{
    xd = 0;
    x = 0;
    yd = 0;
    y = 0;
}

int main ()
{
    // test this juicy stuff.
    CRectangle rect(3, 4), rectb(5.23, 6.4), rectc;
    cout << "rect area " << rect.area() << endl;
    cout << "rectb area " << rectb.area(true) << endl;
    rectc = rect + rectb;
    cout << "rectc area " << rectc.area(true) <<endl;
    return 0;
}