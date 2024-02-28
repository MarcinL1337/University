#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int main(){
    int n;
    cin >> n;
    long long int cur_num;
    bool flag;
    for(int i = 0; i < n; i++){
        cin >> cur_num;
        flag = false;
        if(pow(floor(sqrt(cur_num)), 2) == cur_num){
            cur_num = floor(sqrt(cur_num));
            if(cur_num <= 1) cout << "NO" << endl;
            else if(cur_num <= 3) cout << "YES" << endl;
            else if((cur_num % 2) == 0 || (cur_num % 3) == 0) cout << "NO" << endl;
            else{
                for(long long int j = 5; j * j <= cur_num; j = j + 6){
                    if((cur_num % j) == 0 || (cur_num % (j + 2)) == 0){
                        cout << "NO" << endl;
                        flag = true;
                        break;
                    }
                }
                if(!flag) cout << "YES" << endl;
            }
        }
        else cout << "NO" << endl;
    }
}
