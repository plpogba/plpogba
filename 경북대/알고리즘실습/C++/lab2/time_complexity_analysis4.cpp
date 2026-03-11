#include <iostream>
#include <math.h>
using namespace std;

typedef long long long_t;

long_t fun(long_t n) {
    if (n == 0)
        return 1;
    else {
        long_t s = 0;
        s = pow(8,int(0.5*log2(n))+1);
        return s;
    }
}

int main() {
    long_t n;
    cin >> n;
    cout << fun(n);
}