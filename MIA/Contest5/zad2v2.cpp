#include <iostream>
#include <bits/stdc++.h>

using namespace std;

bool sort_by_pair_sum(const pair<int,int> &a, const pair<int,int> &b, int k){
    return (a.first + k * a.second) < (b.first + k * b.second);
}

class Comparator{
    int cur_souvenirs;
public:
    Comparator(int k){
        cur_souvenirs = k;
    }
    bool operator()(const pair<int,int> &a, const pair<int,int> &b) const {
        return sort_by_pair_sum(a, b, cur_souvenirs);
    }
};

int main(){
    long long int no_of_souvenirs, budget;
    cin >> no_of_souvenirs >> budget;

    vector<pair<int, int>> souvenirs[no_of_souvenirs];
    int cur_souvenir;
    for(int i = 0; i < no_of_souvenirs; i++){
        cin >> cur_souvenir;
        souvenirs->push_back({cur_souvenir, i + 1});
    }

    if(no_of_souvenirs == 1){
        if(souvenirs->at(0).first + souvenirs->at(0).second <= budget)
            cout << "1 " << souvenirs->at(0).first + souvenirs->at(0).second;
        else
            cout << "0 0";
        return 0;
    }

    int cur_sum = 0, prev_sum, prev_num_of_souvenirs = -1, i;
    int l = 0, r = no_of_souvenirs - 1;
    int cur_souvenirs;
    while(l <= r){
        cur_souvenirs = (l + r) / 2;
        cur_sum = 0;
        sort(souvenirs->begin(), souvenirs->end(), Comparator(cur_souvenirs));
        for(i = 0; i <= cur_souvenirs; i++){
            cur_sum += souvenirs->at(i).first + (cur_souvenirs + 1) * souvenirs->at(i).second;
        }

        if(cur_sum == budget){
            l = r + 1;
            prev_num_of_souvenirs = cur_souvenirs+1;
            prev_sum = cur_sum;
        }
        if(cur_sum < budget){
            l = cur_souvenirs + 1;
            prev_num_of_souvenirs = cur_souvenirs+1;
            prev_sum = cur_sum;
        }
        else{
            r = cur_souvenirs - 1;
        }
        // cout << "l = " << l << ", r = " << r;
        // cout << " cur_souvenirs = " << cur_souvenirs << " cur_sum = " << cur_sum << endl;
    }
    if(prev_num_of_souvenirs == -1) cout << 0 << " " << 0;
    else cout << prev_num_of_souvenirs << " " << prev_sum;
}