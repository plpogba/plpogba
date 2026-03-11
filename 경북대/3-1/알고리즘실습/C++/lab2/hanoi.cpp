#include <iostream>

using namespace std;

typedef long long long_t;

void hanoi(long_t n, char start, char via, char dest, long_t k, long_t* arr){
    arr[1] +=1;
    if (n == 1){
        arr[0] += 1;
        if (arr[0] == k)
            cout << start << " -> " << dest << endl;
        return;
    }
    hanoi(n-1, start, dest, via, k, arr);
    hanoi(1, start, via, dest, k, arr);
    hanoi(n-1, via, start, dest, k, arr);
}
int main(){
    long_t num, k;
    long_t a[] = {0,0};
    cin >> num >> k;
    hanoi(num, 'A', 'B', 'C', k, a);
    cout << a[1];
    return 0;
}