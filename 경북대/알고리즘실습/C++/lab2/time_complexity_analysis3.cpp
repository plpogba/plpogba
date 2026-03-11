#include <iostream>
using namespace std;

typedef long long long_t;

long_t fun(long_t n) {
    if (n == 0)
        return 1;
    else
        return 4*fun(n/2);
}

int main() {
    long_t n;
    cin >> n;
    cout << fun(n);

}