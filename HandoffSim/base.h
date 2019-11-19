#include <iostream>
using namespace std;

class base {
    public:
    int x,y;
    public:
    base (int a, int b) {
        x = a;
        y = b;
    }
    void printtest () {
        cout << "x:" <<x<< " y: " << y<<endl;
    }
};
