#include <iostream>
#include <string>

using namespace std;

vector<string> split(const string& s, char delim) {
    vector<string> out;
    string temp;
    for (char c : s) {
        if (c == delim) {
            out.push_back(temp);
            temp.clear();
        } else {
            temp += c;
        }
    }
    if (!temp.empty()) out.push_back(temp);
    return out;
}

vector<int> string2int(const vector<string>& parts) {
    vector<int> out;
    for (const string& part : parts) {
        out.push_back(stoi(part));
    }
    return out;
}

void printVector(const vector<int>& vec) {
    for (int num : vec) {
        cout << num << " ";
    }
    cout << endl;
}


int main(){
    string buff; 
    int N, M;
    getline(cin,buff);
    auto parts = split(buff, ' ');
    
    N = stoi(parts[0]);
    M = stoi(parts[1]);
    
    vector<int> arr;
    vector<int> nums;

    getline(cin, buff);
    parts = split(buff, ' ');
    arr = string2int(parts);
    getline(cin, buff);
    parts = split(buff, ' ');
    nums = string2int(parts);
    
    for(int i : nums) {
        int t = 0;
       for (int j = 0; j < N; j++){
        if (arr[j] == i){
            cout << i << " is in " << j+1 << "." << endl;
            t = 1;
            break;
        }
       }
         if (t == 0) {
          cout << i << " is not in S." << endl;
         }
    }

    
    return 0;
}
