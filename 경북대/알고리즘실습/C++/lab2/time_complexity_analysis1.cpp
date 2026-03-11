#include <iostream>

using namespace std;

typedef long long long_t;

long_t fun(long_t n) {
    long_t i, j, cnt = 0;
    cnt = 2*n*n;
    return cnt;
}

int main() {
    long_t n;
    cin >> n;
    cout << fun(n) << endl;
}