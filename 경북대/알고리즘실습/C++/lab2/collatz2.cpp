#include <iostream>
#include <array>

using namespace std;

typedef long long long_t;

long_t collatz_seq(long_t n){
    long_t tmp = n, cnt =0;
    cnt += 1;

    while(tmp != 1){
        if(tmp%2 == 0)
            tmp /= 2;
        else
            tmp = 3*tmp + 1;
        cnt +=1;
    }   
    return cnt;

}


long_t print_collatz_seq(long_t n){
    long_t tmp = n;
    cout << tmp << " ";
    while(tmp != 1){
        if(tmp%2 == 0)
            tmp /= 2;
        else
            tmp = 3*tmp + 1;
        if (tmp == 1)
            cout << tmp;
        else
            cout << tmp << " ";
    }   
    return 1;

}
array<long_t,2> findLongestCollatz(long_t n, long_t m){
    long_t cnt, num, length;
    array<long_t, 2> max = {0,0};
    for(int i =n; i <=m; i++){
        length  = collatz_seq(i);
        if(max[1] <= length){
            max[1] = length;
            max[0] = i;
        }
    }
    return max;
}
int main(){
    long_t n, m;
    cin >> n >> m;
    array<long_t,2> ptr = findLongestCollatz(n,m);
    cout << ptr[0] << " " << ptr[1] << endl;
    print_collatz_seq(ptr[0]);

}