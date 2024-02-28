#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <cstring>

using namespace std;

#define ll long long int


ll fact(int n){
    if(n <= 1) return 1;
    return n * fact(n - 1);
}

double npok(int n, int k){
    if(n >= k)
        return fact(n) / (fact(n - k) * fact(k));
    else
        return 0.0;
}

int main(){
    string complete, incomplete;
    cin >> complete >> incomplete;

    int distance_cmp = 0, distance_incmp = 0, question_marks = 0;
    for(int i = 0; i < complete.length(); i++){
        if(complete[i] == '+') distance_cmp++;
        if(complete[i] == '-') distance_cmp--;
        if(incomplete[i] == '+') distance_incmp++;
        if(incomplete[i] == '-') distance_incmp--;
        if(incomplete[i] == '?') question_marks++;        
    }

    if(distance_cmp == distance_incmp && question_marks == 0)
        printf("%.12lf", 1.0);
    else if(distance_cmp != distance_incmp && question_marks == 0)
        printf("%.12lf", 0.0);
    else if(question_marks < abs(distance_cmp - distance_incmp))
        printf("%.12lf", 0.0);
    else if((question_marks + abs(distance_cmp - distance_incmp)) % 2)
        printf("%.12lf", 0.0);
    else{
        double res = npok(question_marks, (int)(question_marks + abs(distance_cmp - distance_incmp)) / 2);
        res /= pow(2, question_marks);
        printf("%.12lf\n", res);
    }

}