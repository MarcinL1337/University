#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <cstring>

using namespace std;

#define ll long long int

int main(){
    int no_of_cases;

    cin >> no_of_cases;

    for(int i = 0; i < no_of_cases; i++){
        int ax, ay, bx, by, cx, cy;
        cin >> ax >> ay >> bx >> by >> cx >> cy;
        
        if(ay == by){
            if(cy < ay){
                printf("%d\n", abs(ax - bx));
                continue;
            }
            else{
                printf("%d\n", 0);
                continue;
            }
        }

        else if(ay == cy){
            if(by < ay){
                printf("%d\n", abs(ax - cx));
                continue;
            }
            else{
                printf("%d\n", 0);
                continue;
            }
        }

        else if(cy == by){
            if(ay < cy){
                printf("%d\n", abs(cx - bx));
                continue;
            }
            else{
                printf("%d\n", 0);
                continue;
            }
        }
        else printf("%d\n", 0);
    }
}