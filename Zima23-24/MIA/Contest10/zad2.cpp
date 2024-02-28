#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <cstring>

using namespace std;

#define ll long long int

int main(){
    string s, t;
    cin >> s >> t;

    int s_letters[26] = {0}, t_letters[26] = {0};

    for(int i = 0; i < s.length(); i++)
        s_letters[s[i] - 'a']++;

    for(int i = 0; i < t.length(); i++)
        t_letters[t[i] - 'a']++;

    int distinct_letters = 0;
    int no_of_letters_in_t_less_than_in_s = 0;

    for(int i = 0; i < 26; i++){
        if(t_letters[i] > s_letters[i]){
            printf("need tree");
            return 0;
        }
    }

    if(s.length() == t.length()){
        printf("array");
        return 0;
    }

    int j = 0;
    int count = 0;
    for(int i = 0; i < s.length() && j < t.length(); i++){
        if(s[i] == t[j]){
            j++;
            count++;
        }
    }

    if(count == t.length()) printf("automaton");
    else printf("both");
}