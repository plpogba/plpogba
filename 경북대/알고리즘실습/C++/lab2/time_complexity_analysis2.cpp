#include <iostream>
#include <math.h>

using namespace std;

typedef long long long_t;

long_t fun(long_t n, long_t m, long_t p) {
    long_t i, j, k, cnt = 0;
    
    cnt =round(double(n)/2) * (round(log2(2*m))+1) * (round(log2(4*p))+1);
    return cnt;
}

int main() {
    long_t n, m, p;
    cin >> n >> m >> p;
    cout << fun(n,m,p);
}