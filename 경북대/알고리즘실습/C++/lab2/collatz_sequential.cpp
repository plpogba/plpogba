#include <iostream>
using namespace std;
typedef long long long_t;



long_t collatz_seq(long_t n){
    long_t tmp = n;
    cout << tmp << endl;
    while(tmp != 1){
        if(tmp%2 == 0)
            tmp /= 2;
        else
            tmp = 3*tmp + 1;
        cout << tmp << endl;
    }   
    return 1;

}

int main(){
    long_t n;
    cin >> n;
    if (! collatz_seq(n))
        cout << "err";
    
    return 0;
}