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

    // sort(souvenirs->begin(), souvenirs->end(), sort_by_pair_sum);

    long long int prev_sum, cur_sum = 0, i;
    for(int k = 1; k <= no_of_souvenirs; k++){
        sort(souvenirs->begin(), souvenirs->end(), Comparator(k));
        prev_sum = cur_sum;
        cur_sum = 0;
        for(i = 0; i < k && cur_sum <= budget; i++){
            cur_sum += souvenirs->at(i).first + k * souvenirs->at(i).second;
        }
        if(i >= no_of_souvenirs){
            if(cur_sum > budget){
                cout << k-1 << " " << prev_sum;
                break;
            }
            else{
                cout << no_of_souvenirs << " " << cur_sum;
                break;
            }
        }
        if(cur_sum > budget){
            cout << k-1 << " " << prev_sum;
            break;
        }
    }

}